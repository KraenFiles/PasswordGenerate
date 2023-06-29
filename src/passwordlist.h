#ifndef PASSWORDLIST_H
#define PASSWORDLIST_H

#include "filerw.h"

#include <QJsonArray>
#include <QWidget>

namespace Ui {
class PasswordList;
}

class PasswordList : public QWidget
{
    Q_OBJECT

  public:
    explicit PasswordList( QString password, QWidget * parent = nullptr );
    ~PasswordList();

    void GenerateList();

  private slots:
    void on_backButton_clicked();
    void copy_text();
    void delete_element();

    void on_clearButton_clicked();

    void on_loginsComboBox_currentIndexChanged( int index );

    void on_tagsEdit_textChanged( const QString & arg1 );

    void on_TagDeleteButton_clicked();

    void on_loginDeleteButton_clicked();

  private:
    Ui::PasswordList * ui;

    FileRW file;
    QJsonArray passwords;
    bool loginOverwrite = false;
    bool tagOverwrite = false;
};

#endif // PASSWORDLIST_H
