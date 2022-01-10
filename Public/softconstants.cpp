#include "softconstants.h"

#include <QJsonObject>
#include <QJsonParseError>
#include <QListView>

QString SoftConstants::mItemStr = "{\"0\": {\"1\": \"UTF-8\",\"2\": \"GBK\"},"
                                   "\"1\": {\"1\": \"RGB\",\"2\": \"HSV\",\"3\": \"LAB\",\"4\": \"DIVERGING\"},"
                                   "\"2\": {\"1\": \"RGB\",\"2\": \"HSV\"},"
                                   "\"3\": {\"1\": \"线性算法\",\"2\": \"Loop 算法\",\"3\": \"蝶形算法\"},"
                                   "\"4\": {\"1\": \"简单抽取\",\"2\": \"二次线性抽取\"}"
                                  "}";

QJsonDocument SoftConstants::mJsonDocument = QJsonDocument::fromJson(mItemStr.toUtf8());

SoftConstants::SoftConstants()
{

}

void SoftConstants::fillComboBox(ItemType type, QComboBox *cb)
{
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
