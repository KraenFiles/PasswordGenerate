#include "filerw.h"

#include <QFile>

#include<QJsonDocument>

FileRW::FileRW(QString name)
	:fileName(name), allInformation()
{
	if(!read())
	{
		write();
	}
}

FileRW::~FileRW()
{
}

bool FileRW::read()
{
	QFile thisFile(fileName);
	if(thisFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString all = thisFile.readAll();
		thisFile.close();

		if(!all.isEmpty())
		{
			QJsonDocument document = QJsonDocument::fromJson(all.toUtf8());
			allInformation = document.object();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool FileRW::write()
{
	if(!allInformation.isEmpty())
	{
		QFile thisFile(fileName);
		thisFile.open(QIODevice::WriteOnly | QIODevice::Text);
		QJsonDocument document;
		document.setObject(allInformation);
		thisFile.write(document.toJson());
		thisFile.close();
		return true;
	}
	else
	{
		return false;
	}
}
