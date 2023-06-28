#ifndef ENCRYPTIONENGINE_H
#define ENCRYPTIONENGINE_H

//#include <string>
#include <QString>

class EncryptionEngine
{
  public:
    EncryptionEngine( QString cipher, QString pas );
    EncryptionEngine( QString cipher,
                      QString pas,
                      QString path = "./data",
                      QString encFile = "pas.enc",
                      QString publicKey = "public.key",
                      QString privateKey = "private.key" );

    EncryptionEngine( char * cipher, char * pas );
    EncryptionEngine( char * cipher,
                      char * pas,
                      char * path = "./data",
                      char * encFile = "pas.enc",
                      char * publicKey = "public.key",
                      char * privateKey = "private.key" );

    void RSAKeyGeneration();
    void RSAEncryptionText( QString text );
    QString RSADecryptionToText();

    void RSAEncryptionInFile( QString inFile );
    void RSADecryptionInFile( QString outFile );

    inline void SetCipherName( QString cipher ) { cipherName = const_cast<char *>( cipher.toStdString().c_str() ); }
    inline QString GetCipherName() { return QString( cipherName ); }

    inline void SetPassword( QString pas ) { password = const_cast<char *>( pas.toStdString().c_str() ); }
    inline QString GetPassword() { return QString( password ); }

    inline void SetFilesPath( QString path ) { filesPath = const_cast<char *>( path.toStdString().c_str() ); }
    inline QString GetFilesPath() { return QString( filesPath ); }

    inline void SetEncryptionFileName( QString encFile )
    {
        encryptionFileName = const_cast<char *>( encFile.toStdString().c_str() );
    }
    inline QString GetEncryptionFileName() { return QString( encryptionFileName ); }

    inline void SetPublicKeyName( QString publicKey )
    {
        publicKeyName = const_cast<char *>( publicKey.toStdString().c_str() );
    }
    inline QString GetPublicKeyName() { return QString( publicKeyName ); }

    inline void SetPrivateKeyName( QString privateKey )
    {
        privateKeyName = const_cast<char *>( privateKey.toStdString().c_str() );
    }
    inline QString GetPrivateKeyName() { return QString( privateKeyName ); }

  private:
    char * cipherName;
    char * password;
    char * filesPath = "./data";
    char * encryptionFileName = "pas.enc";
    char * publicKeyName = "public.key";
    char * privateKeyName = "private.key";
};

#endif // ENCRYPTIONENGINE_H
