#include "systemInfoItem.h"
#include "plugininterface.h"
#include <QQmlEngine>
#include "aircraftModelNode.h"

SystemInfoModel::SystemInfoModel(QObject *parent) : QAbstractListModel(parent)
{

}


int SystemInfoModel::rowCount(const QModelIndex &/*parent*/) const {
    return 1;
}


QVariant SystemInfoModel::data(const QModelIndex &/*index*/, int role) const{
    switch (role) {
        case Numberr: return QVariant::fromValue<int>(mSystemInfo.Number);
        case Active: return QVariant::fromValue<bool>(mSystemStatusInfo.RadarSearchStatus == SystemStatusInfo::S ? true : false);
        case MainInfo: return QVariant::fromValue<QStringList>(getMainInfo());
        case MainInfoHeaders: return QVariant::fromValue<QStringList>(getMainInfoHeaders());
        case LocationInfo: return QVariant::fromValue<QStringList>(getLocationInfo());
        case LocationInfoHeaders: return QVariant::fromValue<QStringList>(getLocationInfoHeaders());
        case StatusInfo: return QVariant::fromValue<QStringList>(getStatusInfo());
        case StatusInfoHeaders: return QVariant::fromValue<QStringList>(getStatusInfoHeaders());
        case CombatInfo: return QVariant::fromValue<QStringList>(getCombatInfo());
        case CombatInfoHeaders: return QVariant::fromValue<QStringList>(getCombatInfoHeaders());
        case AssignAircraftsName: return QVariant::fromValue<QStringList>(getAssignmentsName());
        case AssignAircraftsType: return QVariant::fromValue<QStringList>(getAssignmentsType());
        case SystemColor: return QVariant::fromValue<QString>(mSystemCombatInfo.phaseToColor());
        default  : return QVariant::fromValue<QString>(mSystemInfo.Name);

    }
}


QHash<int, QByteArray> SystemInfoModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[Numberr] = "Numberr";
    hash[Active] = "Active";
    hash[MainInfo] = "MainInfo";
    hash[MainInfoHeaders] = "MainInfoHeaders";
    hash[LocationInfo] = "LocationInfo";
    hash[LocationInfoHeaders] = "LocationInfoHeaders";
    hash[StatusInfo] = "StatusInfo";
    hash[StatusInfoHeaders] = "StatusInfoHeaders";
    hash[CombatInfo] = "CombatInfo";
    hash[CombatInfoHeaders] = "CombatInfoHeaders";
    hash[AssignAircraftsType] = "AssignAircraftsType";
    hash[AssignAircraftsName] = "AssignAircraftsName";
    hash[SystemColor] = "SystemColor";
    return hash;
}


void SystemInfoModel::setInformtion(const SystemInfo &systemInfo, const SystemStatusInfo &systemStatusInfo, const SystemCombatInfo &systemCombatInfo)
{
    mSystemInfo = systemInfo;
    mSystemStatusInfo = systemStatusInfo;
    mSystemCombatInfo = systemCombatInfo;
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));

}

void SystemInfoModel::setCombatInfo(const SystemCombatInfo &combatInfo)
{
    mSystemCombatInfo = combatInfo;
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

void SystemInfoModel::setInfo(const SystemInfo &Info)
{
    mSystemInfo = Info;
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

void SystemInfoModel::setStatusInfo(const SystemStatusInfo &statusInfo)
{
    mSystemStatusInfo = statusInfo;
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

QStringList SystemInfoModel::getMainInfo() const
{
    return QStringList {mSystemInfo.Name,
                        QString::number(mSystemInfo.Number),
                        mSystemInfo.Type,
                        mSystemInfo.Terminal};
}

QStringList SystemInfoModel::getMainInfoHeaders() const
{
    return QStringList {"Name", "Number", "Type", "Terminal"};
}

QStringList SystemInfoModel::getLocationInfo() const
{
    return QStringList {std::abs(mSystemInfo.Latitude + 1) < 0.00001 ? "------" : QString("%L1").arg(mSystemInfo.Latitude),
                        std::abs(mSystemInfo.Longitude + 1) < 0.00001 ? "------" : QString("%L1").arg(mSystemInfo.Longitude),
                        std::abs(mSystemInfo.Altitude + 1) < 0.00001 ? "------" : QString("%L1").arg(mSystemInfo.Altitude),
                        std::abs(mSystemInfo.ViewRange + 1) < 0.00001 ? "------" : QString("%L1").arg(mSystemInfo.ViewRange) + " m",
                        std::abs(mSystemInfo.MezRange + 1) < 0.00001 ? "------" : QString("%L1").arg(mSystemInfo.MezRange) + " m"};
}

QStringList SystemInfoModel::getLocationInfoHeaders() const
{
    return QStringList {"Latitude", "Longitude", "Altitude", "ViewRange", "MezRange"};
}

QStringList SystemInfoModel::getStatusInfo() const
{
    return QStringList {mSystemStatusInfo.ReceiveTime,
                        mSystemStatusInfo.Simulation,
                        mSystemStatusInfo.radarStatusToString(mSystemStatusInfo.BCCStatus),
                        mSystemStatusInfo.radarStatusToString(mSystemStatusInfo.RadarSearchStatus),
                        mSystemStatusInfo.operationalToString(),
                        mSystemStatusInfo.MissileCount == -1 ? "------" : QString::number(mSystemStatusInfo.MissileCount),
                        mSystemStatusInfo.RadarMode};
}

QStringList SystemInfoModel::getStatusInfoHeaders() const
{
    return QStringList {"ReceiveTime", "Simulation", "BCCStatus", "RadarSearch Status", "Operational", "MissileCount", "RadarMode"};
}

QStringList SystemInfoModel::getCombatInfo() const
{
    return QStringList {mSystemCombatInfo.TN == -1 ? "------" : QString::number(mSystemCombatInfo.TN),
                        mSystemCombatInfo.Acceptance,
                        mSystemCombatInfo.phaseToString(),
                        std::abs(mSystemCombatInfo.Antenna + 1) < 0.001 ? "------" : QString::number(mSystemCombatInfo.Antenna),
                        mSystemCombatInfo.ChanelNo,
                        mSystemCombatInfo.Inrange};
}

QStringList SystemInfoModel::getCombatInfoHeaders() const
{
    return QStringList {"TN", "Acceptance", "Phase", "Antenna", "ChanelNo", "Inrange"};
}

QStringList SystemInfoModel::getAssignmentsName() const
{
    QStringList aircrafts;
    for (auto i : mAircraftsAssigned) {
        aircrafts.push_back(QString::number(i->getInformation().TN));
    }
    return aircrafts;
}

QStringList SystemInfoModel::getAssignmentsType() const
{
    QStringList aircrafts;
    for (auto i : mAircraftsAssigned) {
        aircrafts.push_back(i->getInformation().aircraftTypeToString());
    }
    return aircrafts;
}

void SystemInfoModel::addAssignment(int number, AircraftModelNode *aircraft)
{
    mAircraftsAssigned[number] = aircraft;
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

void SystemInfoModel::removeAssignment(int number)
{
    mAircraftsAssigned.remove(number);
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

SystemInfoItem::SystemInfoItem(QQmlEngine *qmlEngine, UIHandle *uiHandle, SystemInfo systemInfo,
                                     SystemStatusInfo systemStatusInfo, SystemCombatInfo systemCambatInfo,


                                     QObject *parent) :
    QObject(parent), mUiHandle(uiHandle), mInformation(systemInfo)
{
    QQmlComponent *comp = new QQmlComponent(qmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp, systemStatusInfo, systemCambatInfo](){

        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mInfoModel = new SystemInfoModel;

            mInfoModel->setInformtion(mInformation, systemStatusInfo, systemCambatInfo);
            mItem->setProperty("model", QVariant::fromValue<SystemInfoModel*>(mInfoModel));
//            QQmlEngine::setObjectOwnership(mItem, QQmlEngine::JavaScriptOwnership);
        }

    });

    comp->loadUrl(QUrl("qrc:/modelplugin/SystemInfoView.qml"));
}

void SystemInfoItem::setInfo(const SystemInfo &systemInfo)
{
    mInformation = systemInfo;
    mInfoModel->setInfo(systemInfo);
}

void SystemInfoItem::setStatusInfo(const SystemStatusInfo &systemStatusInfo)
{
    mInfoModel->setStatusInfo(systemStatusInfo);
}

void SystemInfoItem::setCombatInfo(const SystemCombatInfo &systemCombatInfo)
{
    mInfoModel->setCombatInfo(systemCombatInfo);
}

void SystemInfoItem::addAssignment(int number, AircraftModelNode *aircraft)
{
    mInfoModel->addAssignment(number, aircraft);
}

void SystemInfoItem::removeAssignment(int number)
{
    mInfoModel->removeAssignment(number);
}

void SystemInfoItem::show()
{
    mUiHandle->iwShow(mItem, QString::number(mInformation.Number));
}


