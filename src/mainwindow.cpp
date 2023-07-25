#include "mainwindow.h"
#include "generate.h"
#include "passwordlist.h"
#include "ui_mainwindow.h"
#include <QFile>

MainWindow::MainWindow( QWidget * parent )
  : QMainWindow( parent )
  , ui( new Ui::MainWindow )
{
    ui->setupUi( this );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateButton_clicked()
{
    QFile pas( "./data/password" );
    pas.open( QFile::ReadOnly );
    QString password = pas.readAll();
    pas.close();

    Generate * window = new Generate( password );
    window->show();
    this->hide();
}


void MainWindow::on_showAllButton_clicked()
{
    QFile pas( "./data/password" );
    pas.open( QFile::ReadOnly );
    QString password = pas.readAll();
    pas.close();

    PasswordList * window = new PasswordList( password );
    window->show();
    this->hide();
}

