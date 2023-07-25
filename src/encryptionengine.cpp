#include "encryptionengine.h"

#include <QFile>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include <QDataStream>
#include <QTextStream>

EncryptionEngine::EncryptionEngine( QString cipher,
                                    QString pas,
                                    QString path,
                                    QString encFile,
                                    QString publicKey,
                                    QString privateKey )
  : cipherName( cipher )
  , password( pas )
  , filesPath( path )
  , encryptionFileName( encFile )
  , publicKeyName( publicKey )
  , privateKeyName( privateKey )
{
    password.remove( password.length() );
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

    publicKeyFile = fopen( ( filesPath + publicKeyName ).toUtf8().data(), "wb" );
    privateKeyFile = fopen( ( filesPath + privateKeyName ).toUtf8().data(), "wb" );

    rsa = RSA_generate_key( keySize, RSA_F4, NULL, NULL );
    //Контекст алгоритма шифрования
    cipher = EVP_get_cipherbyname( "RC4-SHA" );

    char pas[password.length()];
    char * ar = password.toUtf8().data();
    for ( int i = 0; i < password.length(); i++ ) {
        pas[i] = ar[i];
    }
    //Ключей в файлы и шифрация секретного паролем
    PEM_write_RSAPrivateKey( privateKeyFile, rsa, cipher, NULL, 0, NULL, pas );
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

    publicKeyFile = fopen( ( filesPath + publicKeyName ).toUtf8().data(), "rb" );

    publicKey = PEM_read_RSAPublicKey( publicKeyFile, NULL, NULL, NULL );
    fclose( publicKeyFile );

    int keySize = RSA_size( publicKey );
    ctext = (char *)malloc( keySize );
    ptext = (char *)malloc( keySize );

    OpenSSL_add_all_algorithms();

    QFile outputText( filesPath + encryptionFileName );
    outputText.open( QFile::WriteOnly );

    int textPos = 0;
    bool end = false;
    // Шифруем содержимое входного файла
    while ( 1 ) {
        if ( textPos + keySize - 11 < text.length() - 1 ) {
            ptext = text.mid( textPos, keySize - 11 ).toUtf8().data();
            textPos += keySize - 11;
            inlen = keySize - 11;
        } else {
            inlen = text.mid( textPos ).length();
            ptext = text.mid( textPos ).toUtf8().data();
            end = true;
        }
        if ( inlen <= 0 ) break;
        outlen =
          RSA_public_encrypt( inlen, (unsigned char *)ptext, (unsigned char *)ctext, publicKey, RSA_PKCS1_PADDING );
        if ( outlen != RSA_size( publicKey ) ) exit( -1 );
        outputText.write( ctext, outlen );
        if ( end ) break;
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

    privateKeyFile = fopen( ( filesPath + privateKeyName ).toUtf8().data(), "rb" );

    char pas[password.length()];
    char * ar = password.toUtf8().data();
    for ( int i = 0; i < password.length(); i++ ) {
        pas[i] = ar[i];
    }

    privateKey = PEM_read_RSAPrivateKey( privateKeyFile, NULL, NULL, pas );
    fclose( privateKeyFile );

    int keySize = RSA_size( privateKey );
    ptext = (char *)malloc( keySize );
    ctext = (char *)malloc( keySize );

    QString decryptionText;

    QFile inputText( filesPath + encryptionFileName );
    inputText.open( QFile::ReadOnly );

    QDataStream stream( &inputText );

    while ( 1 ) {
        inlen = stream.readRawData( ctext, keySize );
        if ( inlen <= 0 ) break;
        outlen =
          RSA_private_decrypt( inlen, (unsigned char *)ctext, (unsigned char *)ptext, privateKey, RSA_PKCS1_PADDING );
        if ( outlen < 0 ) exit( 0 );
        decryptionText.append( QString( ptext ).remove( outlen + 1, keySize - outlen - 1 ) );
    }

    inputText.close();

    decryptionText.remove( decryptionText.length() - 1, 1 );

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

bool EncryptionEngine::CheckKeys() const
{
    QFile publicFile( filesPath + publicKeyName );
    publicFile.open( QFile::ReadOnly );
    QString all = publicFile.readAll();
    publicFile.close();

    if ( all.isEmpty() ) return false;

    QFile privateFile( filesPath + privateKeyName );
    privateFile.open( QFile::ReadOnly );
    all = privateFile.readAll();
    privateFile.close();

    if ( all.isEmpty() ) return false;

    return true;
}
