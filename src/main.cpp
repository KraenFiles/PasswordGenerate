#include "mainwindow.h"
#include "synchroniser.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Synchroniser proc;
	MainWindow w;
	w.show();
	return a.exec();
}
