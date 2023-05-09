#include "aircraftInfoItem.h"
#include <QDebug>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include "aircraftModelNode.h"
#include "plugininterface.h"
#include <time.h>
#include "systemModelNode.h"
#include "aircraftDataManager.h"
#include "systemDataManager.h"


AircraftInfoModel::AircraftInfoModel(QObject* parent): QAbstractListModel(parent)
{

}

int AircraftInfoModel::rowCount(const QModelIndex &/*parent*/) const {
    return 1;
}

QVariant AircraftInfoModel::data(const QModelIndex &/*index*/, int role) const{
    switch (role) {
        case TN: return QVariant::fromValue<int>(mAircraftInfo->info.TN);
        case MainInfo: return QVariant::fromValue<QStringList>(getMainInfo());
        case LocationInfo: return QVariant::fromValue<QStringList>(getLocationInfo());
        case LocationInfoHeaders: return QVariant::fromValue<QStringList>(getLocationInfoHeader());
        case MainInfoHeaders: return QVariant::fromValue<QStringList>(getmainInfoHeaders());
        case DetectionSystems: return QVariant::fromValue<QStringList>(mAircraftInfo->info.DetectionSystems);
        case Sends: return QVariant::fromValue<QStringList>(mAircraftInfo->info.Sends);
        case AssignedSystemsName: return QVariant::fromValue<QStringList>(getSystemsName());
        case AssignedSystemsNumber: return QVariant::fromValue<QStringList>(getSystemsNumber());
        case AssignedSystemsPhase: return QVariant::fromValue<QStringList>(getSystemsPhase());
        case SystemColor: return QVariant::fromValue<QStringList>(getSystemColor());
        default: return mAircraftInfo->info.TN;
    }
}

void AircraftInfoModel::setAircraftInfo(const Aircraft::Data& a)
{
    mAircraftInfo = &a;
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

void AircraftInfoModel::updateAircraftInfo()
{
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

QStringList AircraftInfoModel::getMainInfo() const
{
    time_t datetime = mAircraftInfo->info.Time;
    char buffer[256];
    std::tm* currTm = localtime(&datetime);
    strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M", currTm);
    return QStringList {QString::number(mAircraftInfo->info.TN), mAircraftInfo->info.identifyToString(), mAircraftInfo->info.CallSign,
                mAircraftInfo->info.IFFCode, mAircraftInfo->info.aircraftTypeToString(), mAircraftInfo->info.MasterRadar,
                mAircraftInfo->info.IdentificationMethod, buffer};
}

QStringList AircraftInfoModel::getmainInfoHeaders() const
{
    return QStringList {"TN", "Identification", "CallSign", "IFFCode", "Type", "Master"
        , "IdentificationMethod", "Time"};
}

QStringList AircraftInfoModel::getLocationInfo() const
{
    return QStringList {QString("%L1").arg(mAircraftInfo->info.Latitude),
                        QString("%L1").arg(mAircraftInfo->info.Longitude),
                        QString("%L1").arg(mAircraftInfo->info.Altitude),
                        mAircraftInfo->info.Pos,
                        QString::number(mAircraftInfo->info.Heading),
                        QString("%L1").arg(mAircraftInfo->info.Speed) + " m/s"};
}

QStringList AircraftInfoModel::getSystemColor() const
{
    QStringList colors;
    for (auto& i : mAircraftInfo->assignments)
        colors.append(i.info->systemCombatInfo.phaseToColor());
    return colors;
}

QStringList AircraftInfoModel::getSystemsName() const
{
    QStringList systems;
    for (auto i : mAircraftInfo->assignments) {
        systems.append(i.info->systemInfo.Name);
    }
    return systems;
}

QStringList AircraftInfoModel::getSystemsNumber() const
{
    QStringList systems;
    for (auto i : mAircraftInfo->assignments) {
        systems.append(QString::number(i.info->systemInfo.Number));
    }
    return systems;
}

QStringList AircraftInfoModel::getSystemsPhase() const
{
    QStringList systems;
    for (auto i : mAircraftInfo->assignments) {
        systems.push_back(i.info->systemCombatInfo.phaseToString());
    }
    return systems;
}

QStringList AircraftInfoModel::getLocationInfoHeader() const
{
    return QStringList {"Latitude", "Longitude", "Altitude", "Pos", "Heading", "Speed"};
}

void AircraftInfoModel::addAssignment(int number, SystemModelNode *system)
{
//    mAssignedSystems[number] = system;
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

void AircraftInfoModel::removeAssignment(int systemNumber)
{
//    mAssignedSystems.remove(systemNumber);
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

QColor AircraftInfoModel::getAircraftColor()
{
    return mAircraftInfo->info.aircraftColor();
}

QHash<int, QByteArray> AircraftInfoModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[TN] = "TN";
    hash[MainInfo] = "MainInfo";
    hash[MainInfoHeaders] = "MainInfoHeaders";
    hash[LocationInfo] = "LocationInfo";
    hash[LocationInfoHeaders] = "LocationInfoHeaders";
    hash[DetectionSystems] = "DetectionSystems";
    hash[Sends] = "Sends";
    hash[AssignedSystemsName] = "AssignedSystemsName";
    hash[AssignedSystemsNumber] = "AssignedSystemsNumber";
    hash[AssignedSystemsPhase] = "AssignedSystemsPhase";
    hash[SystemColor] = "SystemColor";
    return hash;
}

AircraftInfoItem::AircraftInfoItem(DefenseModelLayer *defenseModelLayer, const Aircraft::Data& minformation, QObject *parent)
    :QObject(parent),
    mInformation(&minformation), mUiHandle(defenseModelLayer->mUIHandle)
{
    QQmlComponent *comp = new QQmlComponent(defenseModelLayer->mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){

        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mInfomodel = new AircraftInfoModel;

            mInfomodel->setAircraftInfo(*mInformation);

            mItem->setProperty("model", QVariant::fromValue<AircraftInfoModel*>(mInfomodel));
//            QQmlEngine::setObjectOwnership(mItem, QQmlEngine::JavaScriptOwnership);
        }

    });

    comp->loadUrl(QUrl("qrc:/modelplugin/AircraftInfoView.qml"));
}

void AircraftInfoItem::updateAircraft()
{
    mInfomodel->updateAircraftInfo();
}
void AircraftInfoItem::show() {
    mUiHandle->iwShow(mItem, QString::number(mInformation->info.TN));
}

void AircraftInfoItem::addAssignment(int number, SystemModelNode *system)
{
    mInfomodel->addAssignment(number, system);
}

void AircraftInfoItem::removeAssignment(int systemNumber)
{
    mInfomodel->removeAssignment(systemNumber);
}

void AircraftInfoItem::setTrackOff()
{
    if(mItem)
        QMetaObject::invokeMethod(mItem, "setTrackOff");
}
