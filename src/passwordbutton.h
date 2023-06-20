#ifndef PASSWORDBUTTON_H
#define PASSWORDBUTTON_H

#include <QPushButton>

class PasswordButton: public QPushButton
{
	Q_OBJECT
public:
	explicit PasswordButton(QWidget *parent = 0, QString saveText = nullptr);
	~PasswordButton();

	inline QString GetText() { return text; }

private:
	QString text;
};

#endif // PASSWORDBUTTON_H
