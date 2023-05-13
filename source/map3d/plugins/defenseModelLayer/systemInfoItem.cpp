#include "systemInfoItem.h"
#include "plugininterface.h"
#include <QQmlEngine>
#include "aircraftModelNode.h"
#include "aircraftDataManager.h"
#include "systemDataManager.h"

SystemInfoModel::SystemInfoModel(QObject *parent) : QAbstractListModel(parent)
{
}


int SystemInfoModel::rowCount(const QModelIndex &/*parent*/) const {
    return 1;
}


QVariant SystemInfoModel::data(const QModelIndex &/*index*/, int role) const{
    switch (role) {
        case Numberr: return QVariant::fromValue<int>(mSystemInfo->information->systemInfo.Number);
        case Active: return QVariant::fromValue<bool>(mSystemInfo->information->systemStatusInfo.RadarSearchStatus == SystemStatusInfo::S ? true : false);
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
        case SystemColor: return QVariant::fromValue<QString>(mSystemInfo->information->systemCombatInfo.phaseToColor());
        case BCCStatusColor: return QVariant::fromValue<QColor>(mSystemInfo->information->systemStatusInfo.statusToColor(mSystemInfo->information->systemStatusInfo.BCCStatus));
        case RadarStatusColor: return QVariant::fromValue<QColor>(mSystemInfo->information->systemStatusInfo.statusToColor(mSystemInfo->information->systemStatusInfo.RadarSearchStatus));
        default  : return QVariant::fromValue<QString>(mSystemInfo->information->systemInfo.Name);
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
    hash[BCCStatusColor] = "BCCStatusColor";
    hash[RadarStatusColor] = "RadarStatusColor";
    return hash;
}


void SystemInfoModel::setInformtion(const System::Data *info)
{
    mSystemInfo = info;
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));

}

QStringList SystemInfoModel::getMainInfo() const
{
    return QStringList {mSystemInfo->information->systemInfo.Name,
                        QString::number(mSystemInfo->information->systemInfo.Number),
                        mSystemInfo->information->systemInfo.Type,
                        mSystemInfo->information->systemInfo.Terminal};
}

QStringList SystemInfoModel::getMainInfoHeaders() const
{
    return QStringList {"Name", "Number", "Type", "Terminal"};
}

QStringList SystemInfoModel::getLocationInfo() const
{
    return QStringList {std::abs(mSystemInfo->information->systemInfo.Latitude + 1) < 0.00001 ? "------" : QString("%L1").arg(mSystemInfo->information->systemInfo.Latitude),
                        std::abs(mSystemInfo->information->systemInfo.Longitude + 1) < 0.00001 ? "------" : QString("%L1").arg(mSystemInfo->information->systemInfo.Longitude),
                        std::abs(mSystemInfo->information->systemInfo.Altitude + 1) < 0.00001 ? "------" : QString("%L1").arg(mSystemInfo->information->systemInfo.Altitude),
                        std::abs(mSystemInfo->information->systemInfo.ViewRange + 1) < 0.00001 ? "------" : QString("%L1").arg(mSystemInfo->information->systemInfo.ViewRange) + " m",
                        std::abs(mSystemInfo->information->systemInfo.MezRange + 1) < 0.00001 ? "------" : QString("%L1").arg(mSystemInfo->information->systemInfo.MezRange) + " m"};
}

QStringList SystemInfoModel::getLocationInfoHeaders() const
{
    return QStringList {"Latitude", "Longitude", "Altitude", "ViewRange", "MezRange"};
}

QStringList SystemInfoModel::getStatusInfo() const
{
    return QStringList {mSystemInfo->information->systemStatusInfo.ReceiveTime,
                        mSystemInfo->information->systemStatusInfo.Simulation,
                        mSystemInfo->information->systemStatusInfo.radarStatusToString(mSystemInfo->information->systemStatusInfo.BCCStatus),
                        mSystemInfo->information->systemStatusInfo.radarStatusToString(mSystemInfo->information->systemStatusInfo.RadarSearchStatus),
                        mSystemInfo->information->systemStatusInfo.operationalToString(),
                        mSystemInfo->information->systemStatusInfo.MissileCount == -1 ? "------" : QString::number(mSystemInfo->information->systemStatusInfo.MissileCount),
                        mSystemInfo->information->systemStatusInfo.RadarMode};
}

QStringList SystemInfoModel::getStatusInfoHeaders() const
{
    return QStringList {"ReceiveTime", "Simulation", "BCCStatus", "RadarSearch Status", "Operational", "MissileCount", "RadarMode"};
}

QStringList SystemInfoModel::getCombatInfo() const
{
    return QStringList {mSystemInfo->information->systemCombatInfo.TN == -1 ? "------" : QString::number(mSystemInfo->information->systemCombatInfo.TN),
                        mSystemInfo->information->systemCombatInfo.Acceptance,
                        mSystemInfo->information->systemCombatInfo.phaseToString(),
                        std::abs(mSystemInfo->information->systemCombatInfo.Antenna + 1) < 0.001 ? "------" : QString::number(mSystemInfo->information->systemCombatInfo.Antenna),
                        mSystemInfo->information->systemCombatInfo.ChanelNo,
                        mSystemInfo->information->systemCombatInfo.Inrange};
}

QStringList SystemInfoModel::getCombatInfoHeaders() const
{
    return QStringList {"TN", "Acceptance", "Phase", "Antenna", "ChanelNo", "Inrange"};
}

QStringList SystemInfoModel::getAssignmentsName() const
{
    QStringList aircrafts;
    for (auto& i : mSystemInfo->assignments) {
        aircrafts.push_back(QString::number(i->info->TN));
    }
    return aircrafts;
}

QStringList SystemInfoModel::getAssignmentsType() const
{
    QStringList aircrafts;
    for (auto& i : mSystemInfo->assignments) {
        aircrafts.push_back(i->info->aircraftTypeToString());
    }
    return aircrafts;
}

SystemInfoItem::SystemInfoItem(QQmlEngine *qmlEngine, UIHandle *uiHandle, const System::Data* systemInfo,
                                     QObject *parent) :
    QObject(parent), mUiHandle(uiHandle), mInformation(systemInfo)
{
    QQmlComponent *comp = new QQmlComponent(qmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){

        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mInfoModel = new SystemInfoModel;

            mInfoModel->setInformtion(mInformation);
            mItem->setProperty("model", QVariant::fromValue<SystemInfoModel*>(mInfoModel));
        }

    });

    comp->loadUrl(QUrl("qrc:/modelplugin/SystemInfoView.qml"));
}

void SystemInfoItem::setInfo(const System::Data *systemInfo)
{
    mInformation = systemInfo;
    mInfoModel->setInformtion(systemInfo);
}

void SystemInfoItem::show()
{
    mUiHandle->iwShow(mItem, QString::number(mInformation->information->systemInfo.Number));
}


