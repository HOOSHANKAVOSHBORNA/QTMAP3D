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
                        QString::number(mSystemInfo.MissileCount), mSystemInfo.RadarMode};
}

QStringList SystemInfoModel::getStatusInfoHeaders() const
{
    return QStringList {"ReceiveTime", "Simulation", "BCCStatus", "RadarSearch Status", "Operational", "MissileCount", "RadarMode"};
}

QStringList SystemInfoModel::getCombatInfo() const
{
    return QStringList {QString::number(mSystemInfo.TN), mSystemInfo.Acceptance, mSystemInfo.phaseToString(),
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


