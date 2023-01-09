
#include "datamanager.h"
#include "plugininterface.h"
#include "aircrafttablemodel.h"
#include "stationtablemodel.h"
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
//        qDebug() << comp->errorString();

        if (comp->status() == QQmlComponent::Ready) {
            QQuickItem *aircraftTab = (QQuickItem*) comp->create(nullptr);
            mAircraftTableModel = new AircraftTableModel;

            QObject::connect(aircraftTab,
                             SIGNAL(filterTextChanged(const QString&)),
                             mAircraftTableModel,
                             SLOT(setFilterWildcard(const QString&)));

            QObject::connect(aircraftTab,
                             SIGNAL(aircraftDoubleClicked(const QString&)),
                             this,
                             SIGNAL(aircraftDoubleClicked(const QString&)));


            aircraftTab->setProperty("model", QVariant::fromValue<AircraftTableModel*>(mAircraftTableModel));
            mUiHandle->lwAddTab("Aircrafts", aircraftTab);
        }

    });

    comp->loadUrl(QUrl("qrc:///modelplugin/AircraftTableView.qml"));

    QQmlComponent *comp2 = new QQmlComponent(mQmlEngine);
    QObject::connect(comp2, &QQmlComponent::statusChanged, [this, comp2](){
//        qDebug() << comp2->errorString();

        if (comp2->status() == QQmlComponent::Ready) {
            QQuickItem *stationTab = (QQuickItem*) comp2->create(nullptr);
            mStationTableModel = new StationTableModel;

            QObject::connect(stationTab,
                             SIGNAL(filterTextChanged(const QString&)),
                             mStationTableModel,
                             SLOT(setFilterWildcard(const QString&)));

            QObject::connect(stationTab,
                             SIGNAL(stationDoubleClicked(const QString&)),
                             this,
                             SIGNAL(stationDoubleClicked(const QString&)));


            stationTab->setProperty("model", QVariant::fromValue<StationTableModel*>(mStationTableModel));
            mUiHandle->lwAddTab("Stations", stationTab);
        }

    });

    comp2->loadUrl(QUrl("qrc:///modelplugin/StationTableView.qml"));

    QQmlComponent *comp3 = new QQmlComponent(mQmlEngine);
    QObject::connect(comp3, &QQmlComponent::statusChanged, [this, comp3](){
//        qDebug() << comp3->errorString();

        if (comp3->status() == QQmlComponent::Ready) {
            QQuickItem *systemTab = (QQuickItem*) comp3->create(nullptr);
            mSystemTableModel = new SystemTableModel;

            QObject::connect(systemTab,
                             SIGNAL(filterTextChanged(const QString&)),
                             mSystemTableModel,
                             SLOT(setFilterWildcard(const QString&)));

            QObject::connect(systemTab,
                             SIGNAL(systemDoubleClicked(const QString&)),
                             this,
                             SIGNAL(systemDoubleClicked(const QString&)));


            systemTab->setProperty("model", QVariant::fromValue<SystemTableModel*>(mSystemTableModel));
            mUiHandle->lwAddTab("Systems", systemTab);
        }

    });

    comp3->loadUrl(QUrl("qrc:///modelplugin/SystemTableView.qml"));


}

void DataManager::setAircraftInfo(const AircraftInfo &aircraftInof)
{
    if (mAircraftTableModel) {
        mAircraftTableModel->updateItemData(aircraftInof);
    }
}

void DataManager::setStationInfo(const StationInfo &stationInfo)
{
    if (mStationTableModel) {
        mStationTableModel->updateItemData(stationInfo);
    }
}

void DataManager::setSystemInfo(const SystemInfo &systemInfo)
{
    if (mSystemTableModel) {
        mSystemTableModel->updateItemData(systemInfo);
    }
}

