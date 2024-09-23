#ifndef APPCONSTANTS_H
#define APPCONSTANTS_H

#include <QString>
#include <QComboBox>
#include <QJsonDocument>

class AppConstants
{
public:
    enum ItemType { Item_CodeType, Item_Log_Type, Item_Version_Text };
    AppConstants();

private:
    static void init();

public:
    static void fillComboBox(ItemType type, QComboBox* cb);
    static QString getValue(ItemType type, const QString& key);

private:
    static QJsonDocument mJsonDocument;
};

#endif // APPCONSTANTS_H
