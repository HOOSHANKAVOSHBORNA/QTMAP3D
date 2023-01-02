
#include "datamanager.h"
#include "plugininterface.h"
#include "aircrafttablemodel.h"
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QDebug>


DataManager::DataManager(QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent) : QObject(parent),
    mQmlEngine(qmlEngine),
    mUiHandle(uiHandle)
{

    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
        qDebug() << comp->errorString();

        if (comp->status() == QQmlComponent::Ready) {
            QQuickItem *item = (QQuickItem*) comp->create(nullptr);
            mAircraftTableModel = new AircraftTableModel;

            QObject::connect(item,
                             SIGNAL(filterTextChanged(const QString&)),
                             mAircraftTableModel,
                             SLOT(setFilterWildcard(const QString&)));

            QObject::connect(item,
                             SIGNAL(aircraftDoubleClicked(const QString&)),
                             this,
                             SIGNAL(aircraftDoubleClicked(const QString&)));


            item->setProperty("model", QVariant::fromValue<AircraftTableModel*>(mAircraftTableModel));
            mUiHandle->lwAddTab("Aircrafts", item);
        }

    });

    comp->loadUrl(QUrl("qrc:///modelplugin/AircraftTableView.qml"));

}

void DataManager::setAircraftInfo(const AircraftInfo &aircraftInof)
{
    if (mAircraftTableModel) {
        mAircraftTableModel->updateItemData(aircraftInof);
    }
}

