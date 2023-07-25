#include "synchroniser.h"

Synchroniser::Synchroniser()
    :script()
{
    script->start("./scripts/download_from_drive.py");
    script->waitForFinished();
}

Synchroniser::~Synchroniser()
{
    script->start("./scripts/upload_to_drive.py");
    script->waitForFinished();
}
