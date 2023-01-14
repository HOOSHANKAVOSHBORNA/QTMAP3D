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
    return hash;
}


void SystemInfoModel::setInformtion(const SystemInfo &systremInfo)
{
    mSystemInfo = systremInfo;
}


SystemInformation::SystemInformation(QQmlEngine *qmlEngine, UIHandle *uiHandle, SystemInfo systemInfo, QObject *parent) :
    QObject(parent), mUiHandle(uiHandle), mInformation(systemInfo)
{
    QQmlComponent *comp = new QQmlComponent(qmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
        qDebug() << comp->errorString();

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


