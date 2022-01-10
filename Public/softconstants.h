#ifndef SOFTCONSTANTS_H
#define SOFTCONSTANTS_H

#include <QString>
#include <QComboBox>
#include <QJsonDocument>

class SoftConstants
{
public:
    enum ItemType { Item_CodeType };
    SoftConstants();

    static void fillComboBox(ItemType type, QComboBox* cb);

private:
    static QString mItemStr;
    static QJsonDocument mJsonDocument;
};

#endif // SOFTCONSTANTS_H
