#include "widgetpagecontainer.h"
#include "ui_widgetpagecontainer.h"
#include "Public/appsignal.h"
#include "Widget/widgetsocketcontent.h"
#include "Widget/widgetservercontent.h"
#include "Widget/widgetversioncontent.h"
#include "Widget/widgetappsetting.h"
#include "Widget/widgetformatpos.h"
#include "Log/logger.h"
#include "Widget/Tools/widgetformatjson.h"

#include <QTabBar>

WidgetPageContainer::WidgetPageContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPageContainer)
{
    ui->setupUi(this);

    init();
}

WidgetPageContainer::~WidgetPageContainer()
{
    delete ui;
}

void WidgetPageContainer::init()
{
    ui->tabWidget->tabBar()->hide();
    connect(AppSignal::getInstance(), &AppSignal::sgl_create_network_object_finish, this, &WidgetPageContainer::slot_create_network_object_finish);
    connect(AppSignal::getInstance(), &AppSignal::sgl_recv_new_network_object, this, &WidgetPageContainer::slot_recv_new_network_object);
    connect(AppSignal::getInstance(), &AppSignal::sgl_current_network_object_change, this, &WidgetPageContainer::slot_current_network_object_change);
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_network_object, this, &WidgetPageContainer::slot_delete_network_object);
}

void WidgetPageContainer::createCustomContent(const QString &name)
{
    if (mListToken.contains(name))
    {
        int index = mListToken.indexOf(name);
        ui->tabWidget->setCurrentIndex(index);
    }
    else
    {
        int index = -1;
        if (name == "Version")
        {
            WidgetVersionContent *content = new WidgetVersionContent(this);
            index = ui->tabWidget->addTab(content, "-----");
        }
        else if (name == "Setting")
        {
            WidgetAppSetting *content = new WidgetAppSetting(this);
            index = ui->tabWidget->addTab(content, "-----");
        }
        else if (name == "FormatJson")
        {
            WidgetFormatJson *content = new WidgetFormatJson(this);
            index = ui->tabWidget->addTab(content, "-----");
        }
        else if (name == "FormatPos")
        {
            WidgetFormatPos *content = new WidgetFormatPos(this);
            index = ui->tabWidget->addTab(content, "-----");
        }

        mListToken.append(name);
        ui->tabWidget->setCurrentIndex(index);
    }
}

void WidgetPageContainer::slot_create_network_object_finish(NetworkObject *netObj)
{
    if (nullptr == netObj) return;
    int type = netObj->getObjectDetail().type;
    if (type <= Udp_Server)
    {
        WidgetServerContent *content = new WidgetServerContent(netObj, this);
        int index = ui->tabWidget->addTab(content, "-----");
        ui->tabWidget->setCurrentIndex(index);
    }
    else if (type <= Udp_Client_Cast)
    {
        WidgetSocketContent *content = new WidgetSocketContent(netObj, this);
        int index = ui->tabWidget->addTab(content, "-----");
        ui->tabWidget->setCurrentIndex(index);
    }
    else
    {
        LOG_DEBUG("unknow object type");
        return;
    }

    mListToken.append(netObj->getObjectDetail().token);
}

void WidgetPageContainer::slot_recv_new_network_object(NetworkObject *netObj, uint16_t dwConnID)
{
    if (nullptr == netObj) return;

    WidgetSocketContent *content = new WidgetSocketContent(netObj, this);
    content->setDwConnID(dwConnID);
    int index = ui->tabWidget->addTab(content, "-----");
    ui->tabWidget->setCurrentIndex(index);

    LOG_DEBUG("new network object {} {}", dwConnID, netObj->getObjectDetail(dwConnID).token.toStdString());
    mListToken.append(netObj->getObjectDetail(dwConnID).token);
}

void WidgetPageContainer::slot_current_network_object_change(const QString &token)
{
    uint16_t size = mListToken.length();
    for (int i = 0; i < size; i++)
    {
        QString value = mListToken.at(i);
        if (value != token) continue;
        ui->tabWidget->setCurrentIndex(i);
        break;
    }

    // 如果跳转到 网络界面，清理其它的所有不是网络的界面 （有待改进）
     for (int i = 0; i < mListToken.size(); i++)
    {
        if (mListToken.at(i).length() >= 32) continue;
        mListToken.removeAt(i);

        auto tab = ui->tabWidget->widget(i);
        ui->tabWidget->removeTab(i);
        tab->deleteLater();
        i--;
    }
}

void WidgetPageContainer::slot_delete_network_object(const QString &token)
{
    uint16_t size = mListToken.length();
    for (int i = 0; i < size; i++)
    {
        QString value = mListToken.at(i);
        if (value != token) continue;

        auto tab = ui->tabWidget->widget(i);
        ui->tabWidget->removeTab(i);
        tab->deleteLater();
        mListToken.removeAt(i);
        break;
    }
}
