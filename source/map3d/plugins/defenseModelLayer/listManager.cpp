
#include "listManager.h"
#include "plugininterface.h"
#include "aircraftTableModel.h"
#include "stationTableModel.h"
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QDebug>


ListManager::ListManager(QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent) : QObject(parent),
    mQmlEngine(qmlEngine),
    mUiHandle(uiHandle)
{

//    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
//    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
////        qDebug() << comp->errorString();

//        if (comp->status() == QQmlComponent::Ready) {
//            QQuickItem *aircraftTab = (QQuickItem*) comp->create(nullptr);
//            mAircraftTableModel = new AircraftTableModel;
//            mAircraftTableModel->setMode("TableModel");

//            QObject::connect(aircraftTab,
//                             SIGNAL(filterTextChanged(const QString&)),
//                             mAircraftTableModel,
//                             SLOT(setFilterWildcard(const QString&)));

//            QObject::connect(aircraftTab,
//                             SIGNAL(aircraftDoubleClicked(const int&)),
//                             this,
//                             SIGNAL(aircraftDoubleClicked(const int&)));

//            QObject::connect(aircraftTab,
//                             SIGNAL(sortWithHeader(int)),
//                             mAircraftTableModel,
//                             SLOT(sortWithHeader(int)));
//            aircraftTab->setProperty("model", QVariant::fromValue<AircraftTableModel*>(mAircraftTableModel));
//            mUiHandle->lwAddTab("Aircrafts", aircraftTab);
//        }

//    });

//    comp->loadUrl(QUrl("qrc:///modelplugin/AircraftTableView.qml"));

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

//    QQmlComponent *comp3 = new QQmlComponent(mQmlEngine);
//    QObject::connect(comp3, &QQmlComponent::statusChanged, [this, comp3](){
////        qDebug() << comp3->errorString();

//        if (comp3->status() == QQmlComponent::Ready) {
//            QQuickItem *systemTab = (QQuickItem*) comp3->create(nullptr);
//            mSystemTableModel = new SystemTableModel;
//            mSystemTableModel->setMode("TableModel");

//            QObject::connect(systemTab,
//                             SIGNAL(filterTextChanged(const QString&)),
//                             mSystemTableModel,
//                             SLOT(setFilterWildcard(const QString&)));

//            QObject::connect(systemTab,
//                             SIGNAL(systemDoubleClicked(const int&)),
//                             this,
//                             SIGNAL(systemDoubleClicked(const int&)));

//            systemTab->setProperty("model", QVariant::fromValue<SystemTableModel*>(mSystemTableModel));
//            mUiHandle->lwAddTab("Systems", systemTab);
//        }

//    });

//    comp3->loadUrl(QUrl("qrc:///modelplugin/SystemTableView.qml"));

//    QQmlComponent *comp4 = new QQmlComponent(mQmlEngine);
//    QObject::connect(comp4, &QQmlComponent::statusChanged, [this, comp4](){
////        qDebug() << comp3->errorString();

//        if (comp4->status() == QQmlComponent::Ready) {
//            QQuickItem *assignTab = (QQuickItem*) comp4->create(nullptr);
////            mAssignModel = new AssignmentModel;
//            mAircraftAssignmentTableModel = new AircraftTableModel;
//            mAircraftAssignmentTableModel->setMode("Assignment");
//            mSystemAssignmentTableModel = new SystemTableModel;
//            mSystemAssignmentTableModel->setMode("Assignment");
//            QObject::connect(assignTab,
//                             SIGNAL(systemDoubleClicked(const int&)),
//                             this,
//                             SIGNAL(systemDoubleClicked(const int&)));

////            QObject::connect(assignTab,
////                             SIGNAL(aircraftDoubleClicked(const int&)),
////                             this,
////                             SIGNAL(aircraftDoubleClicked(const int&)));

//            assignTab->setProperty("aircraftModel", QVariant::fromValue<AircraftTableModel*>(mAircraftAssignmentTableModel));
//            assignTab->setProperty("systemModel", QVariant::fromValue<SystemTableModel*>(mSystemAssignmentTableModel));
//            mUiHandle->lwAddTab("Assignments", assignTab);
//        }

//    });

//    comp4->loadUrl(QUrl("qrc:/modelplugin/AssignmentView.qml"));
//    connect(mUiHandle, &UIHandle::listwindowTabChanged, mAircraftAssignmentTableModel, &AircraftTableModel::refresh);
//    connect(mUiHandle, &UIHandle::listwindowTabChanged, mSystemAssignmentTableModel, &SystemTableModel::refresh);
//    connect(mSystemAssignmentTableModel, &SystemTableModel::systemClicked, mAircraftAssignmentTableModel, &AircraftTableModel::onSystemClicked);
//    connect(mAircraftAssignmentTableModel, &AircraftTableModel::aircraftClicked, mSystemAssignmentTableModel, &SystemTableModel::onAircraftClicked);
}

//void ListManager::setAircraftInfo(const AircraftInfo &aircraftInof)
//{
//    if (mAircraftTableModel) {
//        mAircraftTableModel->updateItemData(aircraftInof);
//    }
//    if (mAircraftAssignmentTableModel) {
//        mAircraftAssignmentTableModel->updateItemData(aircraftInof);
//    }
//}

//void ListManager::deleteAircraftInfo(int TN)
//{
//    if (mAircraftTableModel) {
//        mAircraftTableModel->deleteItem(TN);
//    }
//    if (mAircraftAssignmentTableModel)
//        mAircraftAssignmentTableModel->deleteItem(TN);
//}

void ListManager::setStationInfo(const StationInfo &stationInfo)
{
    if (mStationTableModel) {
        mStationTableModel->updateItemData(stationInfo);
    }
}

void ListManager::setSystemInfo(const SystemInfo &systemInfo)
{
//    if (mSystemTableModel) {
//        mSystemTableModel->updateItemData(systemInfo);
//    }
//    if (mSystemAssignmentTableModel) {
//        mSystemAssignmentTableModel->updateItemData(systemInfo);
//    }
}

void ListManager::setSystemCombatInfo(const SystemCombatInfo &systemCombatInfo)
{
//    if (mSystemTableModel) {
//        mSystemTableModel->updateItemData(systemCombatInfo);
//    }
//    if (mSystemAssignmentTableModel)
//        mSystemAssignmentTableModel->updateItemData(systemCombatInfo);
}

void ListManager::setSystemStatusInfo(const SystemStatusInfo &systemStatusInfo)
{
//    if (mSystemTableModel) {
//        mSystemTableModel->updateItemData(systemStatusInfo);
//    }
//    if (mSystemAssignmentTableModel)
//        mSystemAssignmentTableModel->updateItemData(systemStatusInfo);
}

void ListManager::assignAirToSystem(int TN, int Number)
{
//    mAssignModel->assignAirToSystem(aircraft, system);
    mAircraftAssignmentTableModel->assign(TN, Number);
    mSystemAssignmentTableModel->assign(Number, TN);
}

void ListManager::cancelAssign(int TN, int Number)
{
    mAircraftAssignmentTableModel->cancelAssign(TN, Number);
    mSystemAssignmentTableModel->cancelAssign(TN, Number);
}

void ListManager::acceptAssign(int TN, int Number, bool result)
{
    mAircraftAssignmentTableModel->acceptAssign(TN, Number, result);
    mSystemAssignmentTableModel->acceptAssign(TN, Number, result);
}

void ListManager::cancelSystemAssignmentsExcept(int ExceptTN, int Number)
{
    mAircraftAssignmentTableModel->cancelAircraftsAssigned(ExceptTN, Number);
    for (auto iter : mSystemAssignmentTableModel->getAssignmentMap().keys()) {
        if (iter != ExceptTN) {
            mSystemAssignmentTableModel->cancelAssign(iter, Number);
        }
    }
}

void ListManager::cancelAircraftAssignmentsExcept(int TN, int ExceptNum)
{
    mSystemAssignmentTableModel->cancelSystemsAssigned(TN, ExceptNum);
    for (auto iter : mAircraftAssignmentTableModel->getAssignmentMap().keys()) {
        if (iter != ExceptNum) {
            mAircraftAssignmentTableModel->cancelAssign(iter, ExceptNum);
        }
    }
}

void ListManager::clearAllAssigns()
{
    mAircraftAssignmentTableModel->cancelAllAssigns();
    mSystemAssignmentTableModel->cancelAllAssigns();
}

void ListManager::clearAll()
{
//    if (mAircraftTableModel)
//        mAircraftTableModel->clearList();
//    if (mSystemTableModel)
//        mSystemTableModel->clearList();
    if (mStationTableModel)
        mStationTableModel->clear();
    if (mSystemAssignmentTableModel)
        mSystemAssignmentTableModel->clearList();
//    if (mAircraftAssignmentTableModel)
//        mAircraftAssignmentTableModel->clearList();
}

