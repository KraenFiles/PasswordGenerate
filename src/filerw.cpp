#include "filerw.h"

#include "encryptionengine.h"
#include <QFile>

#include <QJsonDocument>

FileRW::FileRW( QString pas )
  : allInformation()
  , engine( new EncryptionEngine( "RC4-SHA", pas ) )
{
    if ( !engine->CheckKeys() ) {
        engine->RSAKeyGeneration();
    }

    if ( !read() ) {
        write();
    }
}

FileRW::~FileRW()
{
}

bool FileRW::read()
{
    QString decryptionData = engine->RSADecryptionToText();

    if ( !decryptionData.isEmpty() ) {
        QJsonDocument document = QJsonDocument::fromJson( decryptionData.toUtf8() );
        allInformation = document.object();
        return true;
    } else {
        return false;
    }
}

bool FileRW::write()
{
    QJsonDocument document;
    document.setObject( allInformation );
    engine->RSAEncryptionText( document.toJson() );
    return true;
}
