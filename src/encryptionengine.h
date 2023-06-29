#ifndef ENCRYPTIONENGINE_H
#define ENCRYPTIONENGINE_H

//#include <string>
#include <QString>

class EncryptionEngine
{
  public:
    EncryptionEngine( QString cipher,
                      QString pas,
                      QString path = "./data/",
                      QString encFile = "pas.enc",
                      QString publicKey = "public.key",
                      QString privateKey = "private.key" );

    ~EncryptionEngine();

    void RSAKeyGeneration();
    void RSAEncryptionText( QString text );
    QString RSADecryptionToText();

    void RSAEncryptionInFile( QString inFile );
    void RSADecryptionInFile( QString outFile );

    bool CheckKeys() const;

    inline void SetCipherName( QString cipher ) { cipherName = cipher; }
    inline QString GetCipherName() { return cipherName; }

    inline void SetPassword( QString pas ) { password = pas; }
    inline QString GetPassword() { return password; }

    inline void SetFilesPath( QString path ) { filesPath = path; }
    inline QString GetFilesPath() { return filesPath; }

    inline void SetEncryptionFileName( QString encFile ) { encryptionFileName = encFile; }
    inline QString GetEncryptionFileName() { return encryptionFileName; }

    inline void SetPublicKeyName( QString publicKey ) { publicKeyName = publicKey; }
    inline QString GetPublicKeyName() { return publicKeyName; }

    inline void SetPrivateKeyName( QString privateKey ) { privateKeyName = privateKey; }
    inline QString GetPrivateKeyName() { return privateKeyName; }

  private:
    QString cipherName;
    QString password;
    QString filesPath;
    QString encryptionFileName;
    QString publicKeyName;
    QString privateKeyName;
};

#endif // ENCRYPTIONENGINE_H
