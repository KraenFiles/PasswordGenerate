#include "encryptionengine.h"

#include <QFile>
#include <QTextStream>
#include <openssl/pem.h>
#include <openssl/rsa.h>

EncryptionEngine::EncryptionEngine( QString cipher, QString pas )
{
    SetCipherName( cipher );
    SetPassword( pas );
}

EncryptionEngine::EncryptionEngine( QString cipher,
                                    QString pas,
                                    QString path,
                                    QString encFile,
                                    QString publicKey,
                                    QString privateKey )
{
    SetCipherName( cipher );
    SetPassword( pas );
    SetFilesPath( path );
    SetEncryptionFileName( encFile );
    SetPublicKeyName( publicKey );
    SetPrivateKeyName( privateKey );
}

EncryptionEngine::EncryptionEngine( char * cipher, char * pas )
  : cipherName( cipher )
  , password( pas )
{
}

EncryptionEngine::EncryptionEngine( char * cipher,
                                    char * pas,
                                    char * path,
                                    char * encFile,
                                    char * publicKey,
                                    char * privateKey )
  : cipherName( cipher )
  , password( pas )
  , filesPath( path )
  , encryptionFileName( encFile )
  , publicKeyName( publicKey )
  , privateKeyName( privateKey )
{
}

void EncryptionEngine::RSAKeyGeneration()
{
    //Указатель на структуру ключей
    RSA * rsa = NULL;
    FILE * publicKeyFile = NULL;
    FILE * privateKeyFile = NULL;
    unsigned long keySize = 1024; //Длина ключа
    //Контекст алгоритма шифрования
    const EVP_CIPHER * cipher = NULL;

    char * publicFileName = new char[strlen( filesPath ) + strlen( publicKeyName )];
    strcat( publicFileName, filesPath );
    strcat( publicFileName, publicKeyName );
    publicKeyFile = fopen( publicFileName, "wb" );
    delete[] publicFileName;

    char * privateFileName = new char[strlen( filesPath ) + strlen( privateKeyName )];
    strcat( privateFileName, filesPath );
    strcat( privateFileName, privateKeyName );
    privateKeyFile = fopen( privateFileName, "wb" );
    delete[] privateFileName;

    rsa = RSA_generate_key( keySize, RSA_F4, NULL, NULL );
    //Контекст алгоритма шифрования
    cipher = EVP_get_cipherbyname( cipherName );

    //Ключей в файлы и шифрация секретного паролем
    PEM_write_RSAPrivateKey( privateKeyFile, rsa, cipher, NULL, 0, NULL, password );
    PEM_write_RSAPublicKey( publicKeyFile, rsa );

    RSA_free( rsa );
    fclose( publicKeyFile );
    fclose( privateKeyFile );
}

void EncryptionEngine::RSAEncryptionText( QString text )
{
    RSA * publicKey = NULL;
    FILE * publicKeyFile = NULL;
    char *ctext, *ptext;
    int inlen, outlen;

    char * publicFileName = new char[strlen( filesPath ) + strlen( publicKeyName )];
    strcat( publicFileName, filesPath );
    strcat( publicFileName, publicKeyName );
    publicKeyFile = fopen( publicFileName, "rb" );
    delete[] publicFileName;

    publicKey = PEM_read_RSAPublicKey( publicKeyFile, NULL, NULL, NULL );
    fclose( publicKeyFile );

    int keySize = RSA_size( publicKey );
    ctext = (char *)malloc( keySize );
    ptext = (char *)malloc( keySize );

    OpenSSL_add_all_algorithms();

    char * encFileName = new char[strlen( filesPath ) + strlen( encryptionFileName )];
    strcat( encFileName, filesPath );
    strcat( encFileName, encryptionFileName );
    QFile outputText( encFileName );
    outputText.open( QFile::WriteOnly );

    int textPos = 0;
    /* Шифруем содержимое входного файла */
    while ( 1 ) {
        if ( textPos + keySize - 11 < text.length() - 1 )
            ptext = const_cast<char *>( text.mid( textPos, keySize - 11 ).toStdString().c_str() );
        else
            ptext = const_cast<char *>( text.mid( textPos ).toStdString().c_str() );
        inlen = sizeof( ptext );
        if ( inlen <= 0 ) break;
        outlen =
          RSA_public_encrypt( inlen, (unsigned char *)ptext, (unsigned char *)ctext, publicKey, RSA_PKCS1_PADDING );
        if ( outlen != RSA_size( publicKey ) ) exit( -1 );
        outputText.write( ctext, outlen );
    }
    outputText.close();
}

QString EncryptionEngine::RSADecryptionToText()
{
    RSA * privateKey = NULL;
    FILE * privateKeyFile = NULL;
    char *ptext, *ctext;
    int inlen, outlen;

    OpenSSL_add_all_algorithms();
    char * privateFileName = new char[strlen( filesPath ) + strlen( privateKeyName )];
    strcat( privateFileName, filesPath );
    strcat( privateFileName, privateKeyName );
    privateKeyFile = fopen( privateFileName, "wb" );
    delete[] privateFileName;

    privateKey = PEM_read_RSAPrivateKey( privateKeyFile, NULL, NULL, password );
    fclose( privateKeyFile );

    int keySize = RSA_size( privateKey );
    ptext = (char *)malloc( keySize );
    ctext = (char *)malloc( keySize );

    QString decryptionText;

    char * encFileName = new char[strlen( filesPath ) + strlen( encryptionFileName )];
    strcat( encFileName, filesPath );
    strcat( encFileName, encryptionFileName );
    QFile inputText( encFileName );
    inputText.open( QFile::ReadOnly );

    while ( 1 ) {
        inlen = inputText.read( ctext, keySize );
        if ( inlen <= 0 ) break;
        outlen =
          RSA_private_decrypt( inlen, (unsigned char *)ctext, (unsigned char *)ptext, privateKey, RSA_PKCS1_PADDING );
        if ( outlen < 0 ) exit( 0 );
        decryptionText.append( ptext );
    }

    inputText.close();

    return decryptionText;
}

void EncryptionEngine::RSAEncryptionInFile( QString inFile )
{
    QFile inputFile( inFile );
    inputFile.open( QFile::ReadOnly );
    QString text = inputFile.readAll();
    inputFile.close();

    RSAEncryptionText( text );
}

void EncryptionEngine::RSADecryptionInFile( QString outFile )
{
    QString text = RSADecryptionToText();

    QFile outputFile( outFile );
    outputFile.open( QFile::WriteOnly );
    QTextStream out( &outputFile );
    out << text;
    outputFile.close();
}
