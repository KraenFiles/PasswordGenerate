#include "generate.h"
#include "generatepassword.h"
#include "passwordlist.h"
#include "ui_generate.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QMessageBox>

Generate::Generate( QString password, QWidget * parent )
  : QWidget( parent )
  , ui( new Ui::Generate )
  , file( password )
{
    ui->setupUi( this );

    refreshTagsComboBox();
    refreshLoginsComboBox();
}

Generate::~Generate()
{
    delete ui;
}

void Generate::refreshTagsComboBox()
{
    ui->tagsComboBox->clear();

    QJsonArray tags = file.GetValue( "tags" ).toArray();
    if ( !tags.isEmpty() ) {
        foreach ( const QJsonValue & value, tags ) {
            ui->tagsComboBox->addItem( value.toString() );
        }
    }
}

void Generate::refreshLoginsComboBox()
{
    ui->loginComboBox->clear();

    QJsonArray logins = file.GetValue( "logins" ).toArray();
    if ( !logins.isEmpty() ) {
        foreach ( const QJsonValue & value, logins ) {
            ui->loginComboBox->addItem( value.toString() );
        }
    }
}

void Generate::on_generateButton_clicked()
{
    if ( ui->radioButtonNumbers->isChecked() ) {
        GeneratePassword * gen = new GeneratePassword( GeneratePassword::LetAndNum );
        ui->lineEditPassword->setText( QString::fromStdString( gen->generate() ) );
    } else {
        if ( ui->radioButtonAllSimbols->isChecked() ) {
            GeneratePassword * gen = new GeneratePassword( GeneratePassword::All );
            ui->lineEditPassword->setText( QString::fromStdString( gen->generate() ) );
        } else {
            GeneratePassword * gen = new GeneratePassword( GeneratePassword::ExceptionSpecialSymbols,
                                                           ui->lineEditSimbols->text().toStdString() );
            ui->lineEditPassword->setText( QString::fromStdString( gen->generate() ) );
        }
    }
}

void Generate::on_saveButton_clicked()
{
    QString tag = ui->tagsComboBox->currentText();

    if ( !tag.isEmpty() ) {
        QString login = ui->loginComboBox->currentText();
        if ( !login.isEmpty() ) {
            QJsonArray tags = file.GetValue( "tags" ).toArray();

            bool checkTagInList = false;
            foreach ( const QJsonValue & value, tags ) {
                if ( value.toString() == tag ) {
                    checkTagInList = true;
                    break;
                }
            }
            if ( !checkTagInList ) {
                tags.append( QJsonValue( tag ) );
                file.Overwrite( "tags", tags );

                refreshTagsComboBox();
                ui->tagsComboBox->setCurrentIndex( ui->tagsComboBox->count() - 1 );
            }

            QJsonArray logins = file.GetValue( "logins" ).toArray();

            bool checkLoginInList = false;
            foreach ( const QJsonValue & value, logins ) {
                if ( value.toString() == login ) {
                    checkLoginInList = true;
                    break;
                }
            }
            if ( !checkLoginInList ) {
                logins.append( QJsonValue( login ) );
                file.Overwrite( "logins", logins );

                refreshLoginsComboBox();
                ui->loginComboBox->setCurrentIndex( ui->loginComboBox->count() - 1 );
            }

            int tagIndex = ui->tagsComboBox->currentIndex();
            int loginIndex = ui->loginComboBox->currentIndex();
            QJsonArray passwords = file.GetValue( "passwords" ).toArray();
            bool checkTagLoginOverlap = false;

            if ( !passwords.isEmpty() ) {
                int i = 0;
                foreach ( const QJsonValue & value, passwords ) {
                    QJsonObject obj = value.toObject();
                    if ( obj["tag"].toInt() == tagIndex && obj["login"].toInt() == loginIndex ) {
                        checkTagLoginOverlap = true;
                        if ( QMessageBox::question(
                               this, "Пароль существует", "Данный пароль уже существует хотите изменить?" ) ) {
                            checkTagLoginOverlap = false;
                            passwords.removeAt( i );
                        }
                        break;
                    }
                    i++;
                }
            }

            if ( !checkTagLoginOverlap ) {
                QJsonObject newPassword =
                  QJsonObject( { qMakePair( QString( "tag" ), tagIndex ),
                                 qMakePair( QString( "login" ), loginIndex ),
                                 qMakePair( QString( "password" ), ui->lineEditPassword->text() ) } );

                passwords.push_front( newPassword );
                file.Overwrite( "passwords", passwords );
                file.write();
            }
        } else {
            QMessageBox::critical( this, "Ошибка логина", "Логин не выбран!" );
        }
    } else {
        QMessageBox::critical( this, "Ошибка имени", "Имя не введено!" );
    }
}

void Generate::on_backButton_clicked()
{
    QFile pas( "./data/password" );
    pas.open( QFile::ReadOnly );
    QString password = pas.readAll();
    pas.close();

    PasswordList * window = new PasswordList( password );
    window->show();
    this->hide();
}
