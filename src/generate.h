#ifndef GENERATE_H
#define GENERATE_H

#include <QWidget>

#include "filerw.h"

namespace Ui {
class Generate;
}

class Generate : public QWidget
{
	Q_OBJECT

public:
	explicit Generate(QWidget *parent = nullptr);
	~Generate();

	void refreshTagsComboBox();
	void refreshLoginsComboBox();

private slots:
	void on_generateButton_clicked();

	void on_saveButton_clicked();

	void on_backButton_clicked();

private:
	Ui::Generate *ui;

	FileRW file;
};

#endif // GENERATE_H
