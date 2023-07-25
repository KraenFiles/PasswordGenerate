#include "passwordlist.h"
#include "generate.h"
#include "passwordbutton.h"
#include "ui_passwordlist.h"

#include <QClipboard>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>

PasswordList::PasswordList( QString password, QWidget * parent )
  : QWidget( parent )
  , ui( new Ui::PasswordList )
  , file( password )
{
    ui->setupUi( this );

    QJsonArray logins = file.GetValue( "logins" ).toArray();

    ui->loginsComboBox->addItem( "Нет" );
    if ( !logins.isEmpty() ) {
        foreach ( const QJsonValue & value, logins ) {
            ui->loginsComboBox->addItem( value.toString() );
            ui->loginDeleteComboBox->addItem( value.toString() );
        }
    }
    QJsonArray tags = file.GetValue( "tags" ).toArray();
    if ( !tags.isEmpty() ) {
        foreach ( const QJsonValue & value, tags ) {
            ui->TagDeleteComboBox->addItem( value.toString() );
        }
    }

    passwords = file.GetValue( "passwords" ).toArray();
    GenerateList();
}

PasswordList::~PasswordList()
{
    delete ui;
}

void PasswordList::GenerateList()
{
    if ( !passwords.isEmpty() ) {
        qDeleteAll( ui->passwordsContentArea->children() );
        QVBoxLayout * mainLayout = new QVBoxLayout();

        QJsonArray logins = file.GetValue( "logins" ).toArray();
        QJsonArray tags = file.GetValue( "tags" ).toArray();
        foreach ( const QJsonValue & value, passwords ) {
            QHBoxLayout * layout = new QHBoxLayout();

            QLabel * label = new QLabel();
            label->setText( tags[value["tag"].toInt()].toString() );
            layout->addWidget( label );

            QLineEdit * editLogin = new QLineEdit();
            editLogin->setText( logins[value["login"].toInt()].toString() );
            editLogin->setReadOnly( true );
            layout->addWidget( editLogin );

            PasswordButton * loginButton = new PasswordButton( nullptr, editLogin->text() );
            loginButton->setText( "cp" );
            loginButton->setMaximumWidth( 25 );
            connect( loginButton, &PasswordButton::clicked, this, &PasswordList::copy_text );
            layout->addWidget( loginButton );

            QLineEdit * editPassword = new QLineEdit();
            editPassword->setText( value["password"].toString() );
            editPassword->setReadOnly( true );
            layout->addWidget( editPassword );

            PasswordButton * passwordButton = new PasswordButton( nullptr, editPassword->text() );
            passwordButton->setText( "cp" );
            passwordButton->setMaximumWidth( 25 );
            connect( passwordButton, &PasswordButton::clicked, this, &PasswordList::copy_text );
            layout->addWidget( passwordButton );

            PasswordButton * delElement = new PasswordButton( nullptr, label->text() + ";" + loginButton->GetText() );
            delElement->setText( "Удалить" );
            connect( delElement, &PasswordButton::clicked, this, &PasswordList::delete_element );
            layout->addWidget( delElement );

            mainLayout->addLayout( layout );
        }
        QSpacerItem * spacer = new QSpacerItem( 15, 15, QSizePolicy::Expanding, QSizePolicy::Expanding );
        mainLayout->addSpacerItem( spacer );
        ui->passwordsContentArea->setLayout( mainLayout );
    }
}

void PasswordList::on_backButton_clicked()
{
    QFile pas( "./data/password" );
    pas.open( QFile::ReadOnly );
    QString password = pas.readAll();
    pas.close();

    Generate * window = new Generate( password );
    window->show();
    this->hide();
}

void PasswordList::copy_text()
{
    PasswordButton * button = (PasswordButton *)sender();

    QClipboard * clipboard = QApplication::clipboard();

    clipboard->setText( button->GetText() );
}

void PasswordList::delete_element()
{
    PasswordButton * button = (PasswordButton *)sender();

    QStringList list = button->GetText().split( ";" );

    QJsonArray logins = file.GetValue( "logins" ).toArray();

    int indexLogin = 0;
    if ( !logins.isEmpty() ) {
        foreach ( const QJsonValue & value, logins ) {
            if ( value.toString() == list[1] ) {
                break;
            }
            indexLogin++;
        }
    }

    QJsonArray tags = file.GetValue( "tags" ).toArray();

    int indexTag = 0;
    if ( !tags.isEmpty() ) {
        foreach ( const QJsonValue & value, tags ) {
            if ( value.toString() == list[0] ) {
                break;
            }
            indexTag++;
        }
    }

    passwords = file.GetValue( "passwords" ).toArray();
    int indexPassword = 0;
    if ( !passwords.isEmpty() ) {
        foreach ( const QJsonValue & value, passwords ) {
            if ( value["tag"].toInt() == indexTag && value["login"].toInt() == indexLogin ) {
                break;
            }
            indexPassword++;
        }
        passwords.removeAt( indexPassword );
        file.Overwrite( "passwords", passwords );
        file.write();
        GenerateList();
    }
}

void PasswordList::on_clearButton_clicked()
{
    ui->tagsEdit->clear();
    ui->loginsComboBox->setCurrentIndex( 0 );
    passwords = file.GetValue( "passwords" ).toArray();
    GenerateList();
}

void PasswordList::on_loginsComboBox_currentIndexChanged( int index )
{
    QString tagText = ui->tagsEdit->text();
    if ( index != 0 ) {
        if ( tagText.isEmpty() || !tagOverwrite ) {
            passwords = file.GetValue( "passwords" ).toArray();
        } else {
            if ( tagOverwrite ) {
                loginOverwrite = false;
                on_tagsEdit_textChanged( tagText );
            } else {
                return;
            }
        }
        int indexPassword = 0;
        if ( !passwords.isEmpty() ) {
            foreach ( const QJsonValue & value, passwords ) {
                if ( value["login"].toInt() != ( index - 1 ) ) {
                    passwords.removeAt( indexPassword );
                } else {
                    indexPassword++;
                }
            }
            loginOverwrite = true;
            if ( passwords.isEmpty() ) {
                goto Empty;
            }
            GenerateList();
        } else {
        Empty:
            QVBoxLayout * mainLayout = new QVBoxLayout();
            QLabel * label = new QLabel();
            label->setText( "Нет паролей!" );
            label->setAlignment( Qt::Alignment( Qt::AlignmentFlag::AlignCenter ) );
            mainLayout->addWidget( label );
            qDeleteAll( ui->passwordsContentArea->children() );
            ui->passwordsContentArea->setLayout( mainLayout );
        }
    } else {
        if ( !tagText.isEmpty() ) {
            on_tagsEdit_textChanged( tagText );
        } else {
            passwords = file.GetValue( "passwords" ).toArray();
            GenerateList();
        }
    }
}

void PasswordList::on_tagsEdit_textChanged( const QString & arg1 )
{
    int loginIndex = ui->loginsComboBox->currentIndex();
    if ( !arg1.isEmpty() ) {
        if ( loginIndex == 0 || !loginOverwrite ) {
            passwords = file.GetValue( "passwords" ).toArray();
        } else {
            if ( loginOverwrite ) {
                tagOverwrite = false;
                on_loginsComboBox_currentIndexChanged( loginIndex );
            }
        }
        QRegularExpression reg( arg1 );

        QJsonArray tags = file.GetValue( "tags" ).toArray();

        int err = -1;
        QList<int> indexsTags;
        if ( !tags.isEmpty() ) {
            int indexTag = 0;
            foreach ( const QJsonValue & value, tags ) {
                QRegularExpressionMatch match = reg.match( value.toString() );
                if ( match.hasMatch() ) {
                    indexsTags.append( indexTag );
                    err = 0;
                }
                indexTag++;
            }
        }
        if ( err != -1 ) {
            int indexPassword = 0;
            if ( !passwords.isEmpty() ) {
                foreach ( const QJsonValue & value, passwords ) {
                    bool checkValue = false;
                    for ( int i = 0; i < indexsTags.count(); i++ ) {
                        if ( value["tag"].toInt() == indexsTags[i] ) {
                            checkValue = true;
                            break;
                        }
                    }
                    if ( !checkValue ) {
                        passwords.removeAt( indexPassword );
                    } else {
                        indexPassword++;
                    }
                }
                tagOverwrite = true;
                if ( passwords.isEmpty() ) {
                    goto Empty;
                }
                GenerateList();
            }
        } else {
        Empty:
            QVBoxLayout * mainLayout = new QVBoxLayout();
            QLabel * label = new QLabel();
            label->setText( "Нет паролей!" );
            label->setAlignment( Qt::Alignment( Qt::AlignmentFlag::AlignCenter ) );
            mainLayout->addWidget( label );
            qDeleteAll( ui->passwordsContentArea->children() );
            ui->passwordsContentArea->setLayout( mainLayout );
        }
    } else {
        if ( loginIndex != 0 ) {
            on_loginsComboBox_currentIndexChanged( loginIndex );
        } else {
            passwords = file.GetValue( "passwords" ).toArray();
            GenerateList();
        }
    }
}

void PasswordList::on_TagDeleteButton_clicked()
{
    int indexTag = ui->TagDeleteComboBox->currentIndex();
    bool availTagInPasswords = false;

    passwords = file.GetValue( "passwords" ).toArray();
    if ( !passwords.isEmpty() ) {
        foreach ( const QJsonValue & value, passwords ) {
            if ( value["tag"].toInt() == indexTag ) {
                availTagInPasswords = true;
                break;
            }
        }

        if ( !availTagInPasswords ) {
            int removeIndex = 0;
            foreach ( const QJsonValue & value, passwords ) {
                if ( value["tag"].toInt() > indexTag ) {
                    QJsonObject newPassword =
                      QJsonObject( { qMakePair( QString( "login" ), value["login"].toInt() ),
                                     qMakePair( QString( "password" ), value["password"].toString() ),
                                     qMakePair( QString( "tag" ), value["tag"].toInt() - 1 ) } );
                    passwords.removeAt( removeIndex );
                    passwords.push_front( newPassword );
                    file.Overwrite( "passwords", passwords );
                }
                removeIndex++;
            }

            QJsonArray tags = file.GetValue( "tags" ).toArray();
            tags.removeAt( indexTag );
            ui->TagDeleteComboBox->clear();
            if ( !tags.isEmpty() ) {
                foreach ( const QJsonValue & value, tags ) {
                    ui->TagDeleteComboBox->addItem( value.toString() );
                }
            }
            file.Overwrite( "tags", tags );
            file.write();
            on_tagsEdit_textChanged( ui->tagsEdit->text() );
        } else {
            QMessageBox::critical( this, "Ошибка удаления", "Данное имя используется паролем!" );
        }
    }
}

void PasswordList::on_loginDeleteButton_clicked()
{
    int indexLogin = ui->loginDeleteComboBox->currentIndex();
    bool availLoginInPasswords = false;

    passwords = file.GetValue( "passwords" ).toArray();
    if ( !passwords.isEmpty() ) {
        foreach ( const QJsonValue & value, passwords ) {
            if ( value["login"].toInt() == indexLogin ) {
                availLoginInPasswords = true;
                break;
            }
        }

        if ( !availLoginInPasswords ) {
            int removeIndex = 0;
            foreach ( const QJsonValue & value, passwords ) {
                if ( value["login"].toInt() > indexLogin ) {
                    QJsonObject newPassword =
                      QJsonObject( { qMakePair( QString( "login" ), value["login"].toInt() - 1 ),
                                     qMakePair( QString( "password" ), value["password"].toString() ),
                                     qMakePair( QString( "tag" ), value["tag"].toInt() ) } );
                    passwords.removeAt( removeIndex );
                    passwords.push_front( newPassword );
                    file.Overwrite( "passwords", passwords );
                    file.write();
                }
                removeIndex++;
            }

            QJsonArray logins = file.GetValue( "logins" ).toArray();
            logins.removeAt( indexLogin );
            ui->loginsComboBox->clear();
            ui->loginDeleteComboBox->clear();
            if ( !logins.isEmpty() ) {
                ui->loginsComboBox->addItem( "Нет" );
                foreach ( const QJsonValue & value, logins ) {
                    ui->loginsComboBox->addItem( value.toString() );
                    ui->loginDeleteComboBox->addItem( value.toString() );
                }
            }
            file.Overwrite( "logins", logins );
            file.write();
            on_loginsComboBox_currentIndexChanged( ui->loginsComboBox->currentIndex() );
        } else {
            QMessageBox::critical( this, "Ошибка удаления", "Данный логин используется паролем!" );
        }
    }
}
