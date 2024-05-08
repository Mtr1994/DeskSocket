#ifndef SOFTCONFIG_H
#define SOFTCONFIG_H

#include <QObject>
#include <QSettings>

class AppConfig : public QObject
{
    Q_OBJECT
public:
    static AppConfig *getInstance();

    bool init();

    QString getValue(const QString &entry, const QString &item);

    void setValue(const QString &pEntry, const QString &pItem, const QString &pValue);

private:
    explicit AppConfig(QObject *parent = nullptr);


private:
    QSettings *mSetting = nullptr;
};

#endif // SOFTCONFIG_H
