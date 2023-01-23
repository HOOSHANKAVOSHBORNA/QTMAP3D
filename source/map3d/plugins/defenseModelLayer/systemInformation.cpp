#include "systemInformation.h"
#include "plugininterface.h"
#include <QQmlEngine>

SystemInfoModel::SystemInfoModel(QObject *parent) : QAbstractListModel(parent)
{

}


int SystemInfoModel::rowCount(const QModelIndex &/*parent*/) const {
    return 1;
}


QVariant SystemInfoModel::data(const QModelIndex &/*index*/, int role) const{
    switch (role) {
        case Name: return QVariant::fromValue<QString>(mSystemInfo.Name);
        case Terminal: return QVariant::fromValue<QString>(mSystemInfo.Terminal);
        case Numberr: return QVariant::fromValue<double>(mSystemInfo.Number);
        case Type: return QVariant::fromValue<QString>(mSystemInfo.Type);
        case Latitude: return QVariant::fromValue<double>(mSystemInfo.Latitude);
        case Longitude: return QVariant::fromValue<double>(mSystemInfo.Longitude);
        case Altitude: return QVariant::fromValue<double>(mSystemInfo.Altitude);
        case ViewRange: return QVariant::fromValue<double>(mSystemInfo.ViewRange);
        case MezRange: return QVariant::fromValue<double>(mSystemInfo.MezRange);
        case ReceiveTime: return QVariant::fromValue<QString>(mSystemInfo.ReceiveTime);
        case Simulation: return QVariant::fromValue<QString>(mSystemInfo.Simulation);
        case BCCStatus: return QVariant::fromValue<QString>(mSystemInfo.BCCStatus);
        case RadarSearchStatus: return QVariant::fromValue<QString>(mSystemInfo.RadarSearchStatus);
        case Operational: return QVariant::fromValue<QString>(mSystemInfo.Operational);
        case MissileCount: return QVariant::fromValue<QString>(mSystemInfo.MissileCount);
        case RadarMode: return QVariant::fromValue<QString>(mSystemInfo.RadarMode);
        case TN: return QVariant::fromValue<double>(mSystemInfo.TN);
        case Acceptance: return QVariant::fromValue<QString>(mSystemInfo.Acceptance);
        case Phase: return QVariant::fromValue<QString>(mSystemInfo.Phase);
        case Antenna: return QVariant::fromValue<double>(mSystemInfo.Antenna);
        case ChanelNo: return QVariant::fromValue<QString>(mSystemInfo.ChanelNo);
        case Inrange: return QVariant::fromValue<QString>(mSystemInfo.Inrange);
        case Active: return QVariant::fromValue<bool>(mSystemInfo.Active);
        case MainInfo: return QVariant::fromValue<QStringList>(getMainInfo());
        case MainInfoHeaders: return QVariant::fromValue<QStringList>(getMainInfoHeaders());
        case LocationInfo: return QVariant::fromValue<QStringList>(getLocationInfo());
        case LocationInfoHeaders: return QVariant::fromValue<QStringList>(getLocationInfoHeaders());
        case StatusInfo: return QVariant::fromValue<QStringList>(getStatusInfo());
        case StatusInfoHeaders: return QVariant::fromValue<QStringList>(getStatusInfoHeaders());
        case CombatInfo: return QVariant::fromValue<QStringList>(getCombatInfo());
        case CombatInfoHeaders: return QVariant::fromValue<QStringList>(getCombatInfoHeaders());
        default  : return QVariant::fromValue<QString>(mSystemInfo.Name);

    }
}


QHash<int, QByteArray> SystemInfoModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[Name] = "Name";
    hash[Terminal] = "Terminal";
    hash[Numberr] = "Numberr";
    hash[Type] = "Type";
    hash[Latitude] = "Latitude";
    hash[Longitude] = "Longitude";
    hash[Altitude] = "Altitude";
    hash[ViewRange] = "ViewRange";
    hash[MezRange] = "MezRange";
    hash[ReceiveTime] = "ReceiveTime";
    hash[Simulation] = "Simulation";
    hash[BCCStatus] = "BCCStatus";
    hash[RadarSearchStatus] = "RadarSearchStatus";
    hash[Operational] = "Operational";
    hash[MissileCount] = "MissileCount";
    hash[RadarMode] = "RadarMode";
    hash[TN] = "TN";
    hash[Acceptance] = "Acceptance";
    hash[Phase] = "Phase";
    hash[Antenna] = "Antenna";
    hash[ChanelNo] = "ChanelNo";
    hash[Inrange] = "Inrange";
    hash[Active] = "Active";
    hash[MainInfo] = "MainInfo";
    hash[MainInfoHeaders] = "MainInfoHeaders";
    hash[LocationInfo] = "LocationInfo";
    hash[LocationInfoHeaders] = "LocationInfoHeaders";
    hash[StatusInfo] = "StatusInfo";
    hash[StatusInfoHeaders] = "StatusInfoHeaders";
    hash[CombatInfo] = "CombatInfo";
    hash[CombatInfoHeaders] = "CombatInfoHeaders";
    return hash;
}


void SystemInfoModel::setInformtion(const SystemInfo &systemInfo)
{
    mSystemInfo = systemInfo;
}

QStringList SystemInfoModel::getMainInfo() const
{
    return QStringList {mSystemInfo.Name, QString::number(mSystemInfo.Number),
                        mSystemInfo.Type, mSystemInfo.Terminal};
}

QStringList SystemInfoModel::getMainInfoHeaders() const
{
    return QStringList {"Name", "Number", "Type", "Terminal"};
}

QStringList SystemInfoModel::getLocationInfo() const
{
    return QStringList {QString::number(mSystemInfo.Latitude),
                        QString::number(mSystemInfo.Longitude),
                        QString::number(mSystemInfo.Altitude),
                        QString::number(mSystemInfo.ViewRange),
                        QString::number(mSystemInfo.MezRange)};
}

QStringList SystemInfoModel::getLocationInfoHeaders() const
{
    return QStringList {"Latitude", "Longitude", "Altitude", "ViewRange", "MezRange"};
}

QStringList SystemInfoModel::getStatusInfo() const
{
    return QStringList {mSystemInfo.ReceiveTime, mSystemInfo.Simulation, mSystemInfo.BCCStatus,
                        mSystemInfo.RadarSearchStatus, mSystemInfo.Operational,
                        mSystemInfo.MissileCount, mSystemInfo.RadarMode};
}

QStringList SystemInfoModel::getStatusInfoHeaders() const
{
    return QStringList {"ReceiveTime", "Simulation", "BCCStatus", "RadarSearch Status", "Operational", "MissileCount", "RadarMode"};
}

QStringList SystemInfoModel::getCombatInfo() const
{
    return QStringList {QString::number(mSystemInfo.TN), mSystemInfo.Acceptance, mSystemInfo.Phase,
                        QString::number(mSystemInfo.Antenna), mSystemInfo.ChanelNo, mSystemInfo.Inrange};
}

QStringList SystemInfoModel::getCombatInfoHeaders() const
{
    return QStringList {"TN", "Acceptance", "Phase", "Antenna", "ChanelNo", "Inrange"};
}


SystemInformation::SystemInformation(QQmlEngine *qmlEngine, UIHandle *uiHandle, SystemInfo systemInfo, QObject *parent) :
    QObject(parent), mUiHandle(uiHandle), mInformation(systemInfo)
{
    QQmlComponent *comp = new QQmlComponent(qmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){

        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mInfoModel = new SystemInfoModel;

            mInfoModel->setInformtion(mInformation);
            mItem->setProperty("model", QVariant::fromValue<SystemInfoModel*>(mInfoModel));
            QQmlEngine::setObjectOwnership(mItem, QQmlEngine::JavaScriptOwnership);
        }

    });

    comp->loadUrl(QUrl("qrc:/modelplugin/SystemInfoView.qml"));
}

void SystemInformation::show()
{
    mUiHandle->iwShow(mItem);
}


