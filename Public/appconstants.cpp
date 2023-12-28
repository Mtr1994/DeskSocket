#include "appconstants.h"

#include <QJsonObject>
#include <QJsonParseError>
#include <QListView>
#include <QFile>

QJsonDocument AppConstants::mJsonDocument;

AppConstants::AppConstants()
{

}

void AppConstants::init()
{
    // 填充状态码
    QFile file(":/Resource/json/constants.json");
    if (!file.open(QFile::ReadOnly)) return;
    mJsonDocument = QJsonDocument::fromJson(file.readAll());
}

void AppConstants::fillComboBox(ItemType type, QComboBox *cb)
{
    if (mJsonDocument.isEmpty())
    {
        init();
    }

    QJsonObject obj = mJsonDocument.object().value(QString::number(type)).toObject();
    QStringList keys = obj.keys();
    for (int i = 1; i <= keys.count(); i++)
    {
       int index = keys.indexOf(QString::number(i));
       if (index < 0) continue;
       cb->addItem(obj.value(keys.at(index)).toString(), keys.at(index).toInt());
    }
    cb->setView(new QListView());
}
