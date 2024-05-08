#include "appconfig.h"

#include <QTextCodec>
#include <QFile>

AppConfig::AppConfig(QObject *parent) : QObject(parent)
{

}

bool AppConfig::init()
{
    bool exits = QFile::exists("conf.ini");
    if (!exits)
    {
        QFile file("conf.ini");
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) return false;
    }

    mSetting = new QSettings("conf.ini", QSettings::IniFormat);
    mSetting->setIniCodec(QTextCodec::codecForName("utf-8"));

    if (!exits)
    {
        // 默认编码类型
        setValue("Setting", "format", "UTF-8");
        setValue("Setting", "log", "0");
    }

    return true;
}

AppConfig *AppConfig::getInstance()
{
    static AppConfig config;
    return &config;
}

QString AppConfig::getValue(const QString& entry, const QString& item)
{
    if (nullptr == mSetting) return "";

    QString value = mSetting->value(entry + "/" + item).toString();
    return value;
}

void AppConfig::setValue(const QString& pEntry, const QString& pItem, const QString& pValue)
{
    if (nullptr == mSetting) return;
    mSetting->setValue(pEntry + "/" + pItem, pValue);
}
