#ifndef FILERW_H
#define FILERW_H

#include "encryptionengine.h"
#include <QJsonObject>
#include <QString>

class FileRW
{
  public:
    FileRW( QString pas );

    ~FileRW();

    bool read();
    bool write();

    inline QJsonValue GetValue( QString valueName ) { return allInformation[valueName]; }

    inline void Append( QString name, QJsonValue & value ) { allInformation.insert( name, value ); }
    inline void Append( QString name, QJsonArray & array ) { allInformation.insert( name, array ); }
    inline void Append( QString name, QJsonObject & array ) { allInformation.insert( name, array ); }

    inline void Overwrite( QString name, QJsonValue & value )
    {
        Delete( name );
        Append( name, value );
    }
    inline void Overwrite( QString name, QJsonArray & value )
    {
        Delete( name );
        Append( name, value );
    }
    inline void Overwrite( QString name, QJsonObject & value )
    {
        Delete( name );
        Append( name, value );
    }

    inline void Delete( QString name ) { allInformation.remove( name ); }

  private:
    QJsonObject allInformation;

    EncryptionEngine * engine;
};

#endif // FILERW_H
