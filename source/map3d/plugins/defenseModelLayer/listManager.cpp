
#include "listManager.h"
#include "plugininterface.h"
#include "aircraftTableModel.h"
#include "stationTableModel.h"
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
                             SIGNAL(aircraftDoubleClicked(const int&)),
                             this,
                             SIGNAL(aircraftDoubleClicked(const int&)));


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
                             SIGNAL(stationDoubleClicked(const int&)),
                             this,
                             SIGNAL(stationDoubleClicked(const int&)));


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
                             SIGNAL(systemDoubleClicked(const int&)),
                             this,
                             SIGNAL(systemDoubleClicked(const int&)));

            systemTab->setProperty("model", QVariant::fromValue<SystemTableModel*>(mSystemTableModel));
            mUiHandle->lwAddTab("Systems", systemTab);
        }

    });

    comp3->loadUrl(QUrl("qrc:///modelplugin/SystemTableView.qml"));

    QQmlComponent *comp4 = new QQmlComponent(mQmlEngine);
    QObject::connect(comp4, &QQmlComponent::statusChanged, [this, comp4](){
//        qDebug() << comp3->errorString();

        if (comp4->status() == QQmlComponent::Ready) {
            QQuickItem *assignTab = (QQuickItem*) comp4->create(nullptr);
//            mAssignModel = new AssignmentModel;

            QObject::connect(assignTab,
                             SIGNAL(systemDoubleClicked(const int&)),
                             this,
                             SIGNAL(systemDoubleClicked(const int&)));

            QObject::connect(assignTab,
                             SIGNAL(aircraftDoubleClicked(const int&)),
                             this,
                             SIGNAL(aircraftDoubleClicked(const int&)));

            assignTab->setProperty("aircraftModel", QVariant::fromValue<AircraftTableModel*>(mAircraftTableModel));
            assignTab->setProperty("systemModel", QVariant::fromValue<SystemTableModel*>(mSystemTableModel));
            mUiHandle->lwAddTab("Assignments", assignTab);
        }

    });

    comp4->loadUrl(QUrl("qrc:/modelplugin/AssignmentView.qml"));
    connect(mUiHandle, &UIHandle::listwindowTabChanged, mAircraftTableModel, &AircraftTableModel::refresh);
    connect(mUiHandle, &UIHandle::listwindowTabChanged, mSystemTableModel, &SystemTableModel::refresh);
    connect(mSystemTableModel, &SystemTableModel::systemClicked, mAircraftTableModel, &AircraftTableModel::onSystemClicked);
    connect(mAircraftTableModel, &AircraftTableModel::aircraftClicked, mSystemTableModel, &SystemTableModel::onAircraftClicked);
}

void DataManager::setAircraftInfo(const AircraftInfo &aircraftInof)
{
    if (mAircraftTableModel) {
        mAircraftTableModel->updateItemData(aircraftInof);
    }
}

void DataManager::deleteAircraftInfo(int TN)
{
    if (mAircraftTableModel) {
        mAircraftTableModel->deleteItem(TN);
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

void DataManager::setSystemCombatInfo(const SystemCambatInfo &systemCombatInfo)
{
    if (mSystemTableModel) {
        mSystemTableModel->updateItemData(systemCombatInfo);
    }
}

void DataManager::setSystemStatusInfo(const SystemStatusInfo &systemStatusInfo)
{
    if (mSystemTableModel) {
        mSystemTableModel->updateItemData(systemStatusInfo);
    }
}

void DataManager::assignAirToSystem(int TN, int Number)
{
//    mAssignModel->assignAirToSystem(aircraft, system);
    mAircraftTableModel->assign(TN, Number);
    mSystemTableModel->assign(Number, TN);
}

void DataManager::cancelAssign(int TN, int Number)
{
    mAircraftTableModel->cancelAssign(TN, Number);
    mSystemTableModel->cancelAssign(Number, TN);
}

void DataManager::clearAll()
{
    if (mAircraftTableModel)
        mAircraftTableModel->clear();
    if (mSystemTableModel)
        mSystemTableModel->clear();
    if (mStationTableModel)
        mStationTableModel->clear();
}

