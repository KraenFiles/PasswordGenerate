#ifndef SYNCHRONISER_H
#define SYNCHRONISER_H

#include <QProcess>

class Synchroniser
{
public:
    explicit Synchroniser();
    ~Synchroniser();

private:
    QProcess * script;
};

#endif // SYNCHRONISER_H
