#ifndef APPCONSTANTS_H
#define APPCONSTANTS_H

#include <QString>
#include <QComboBox>
#include <QJsonDocument>

class AppConstants
{
public:
    enum ItemType { Item_CodeType };
    AppConstants();

private:
    static void init();

public:
    static void fillComboBox(ItemType type, QComboBox* cb);

private:
    static QJsonDocument mJsonDocument;
};

#endif // APPCONSTANTS_H
