#include "appsignal.h"
#include <qmetatype.h>

AppSignal::AppSignal(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<uint16_t>("uint16_t");
    qRegisterMetaType<uint32_t>("uint32_t");
    qRegisterMetaType<int32_t>("int32_t");
    qRegisterMetaType<std::string>("std::string");
}

AppSignal *AppSignal::getInstance()
{
    static AppSignal appSignal;
    return &appSignal;
}
