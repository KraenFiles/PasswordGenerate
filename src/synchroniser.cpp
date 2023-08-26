#include "synchroniser.h"
#include <QDebug>

Synchroniser::Synchroniser()
    :script(new QProcess)
{
#if defined(Q_OS_WINDOWS)
    script->start("python.exe", {"./scripts/download_from_drive.py"});
#elif define(Q_OS_UNIX)
    script->start("python", {"./scripts/download_from_drive.py"});
#endif
    script->waitForStarted();
    script->waitForFinished();
    qDebug()<< script->readAllStandardOutput();
}

Synchroniser::~Synchroniser()
{
#if defined(Q_OS_WINDOWS)
    script->start("python.exe", {"./scripts/upload_to_drive.py"});
#elif define(Q_OS_UNIX)
    script->start("python", {"./scripts/upload_to_drive.py"});
#endif
    script->waitForStarted();
    script->waitForFinished();
    qDebug()<< script->readAllStandardOutput();

    delete script;
}
