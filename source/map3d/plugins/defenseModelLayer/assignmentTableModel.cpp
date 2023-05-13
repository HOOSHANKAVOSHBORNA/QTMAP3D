#include "assignmentTableModel.h"
#include "aircraftDataManager.h"
#include "systemDataManager.h"

AssignmentTableModel::AssignmentTableModel(AircraftDataManager *aircraftDatamanager, SystemDataManager *systemDataManager, DefenseModelLayer *defenseModelLayer, QObject *parent):
    QObject(parent),
    mDefenseModelLayer(defenseModelLayer),
    mAircraftDataManager(aircraftDatamanager),
    mSystemDataManager(systemDataManager)
{
    mAircraftTableModel = new AircraftTableModel;
    mAircraftTableModel->setAircraftInfos(mAircraftDataManager->getAircraftsData());
    mSystemTableModel = new SystemTableModel;
    mSystemTableModel->setSystemInfos(mSystemDataManager->getSystemsData());

    QQmlComponent *comp4 = new QQmlComponent(mDefenseModelLayer->mQmlEngine);
    QObject::connect(comp4, &QQmlComponent::statusChanged, [this, comp4](){
        //        qDebug() << comp3->errorString();

        if (comp4->status() == QQmlComponent::Ready) {
            QQuickItem *assignTab = static_cast<QQuickItem*>(comp4->create(nullptr));
            QObject::connect(assignTab,
                             SIGNAL(systemDoubleClicked(const int&)),
                             mSystemTableModel,
                             SLOT(onDoubleClicked(const int&)));

            QObject::connect(assignTab,
                             SIGNAL(aircraftDoubleClicked(const int&)),
                             mAircraftTableModel,
                             SLOT(onDoubleClicked(const int&)));
            QObject::connect(mAircraftDataManager, &AircraftDataManager::infoChanged, mAircraftTableModel, &AircraftTableModel::onInfoChanged);
            QObject::connect(mAircraftDataManager, &AircraftDataManager::removed, mAircraftTableModel, &AircraftTableModel::onRemoveData);
            connect(mSystemDataManager, &SystemDataManager::infoChanged, mSystemTableModel, &SystemTableModel::onInfoChanged);

            assignTab->setProperty("aircraftModel", QVariant::fromValue<AircraftTableModel*>(mAircraftTableModel));
            assignTab->setProperty("systemModel", QVariant::fromValue<SystemTableModel*>(mSystemTableModel));
            mDefenseModelLayer->mUIHandle->lwAddTab("Assignments", assignTab);
        }

    });

    comp4->loadUrl(QUrl("qrc:/modelplugin/AssignmentView.qml"));

}
