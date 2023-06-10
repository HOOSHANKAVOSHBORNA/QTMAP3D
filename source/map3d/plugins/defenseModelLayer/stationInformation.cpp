#include "stationInformation.h"
#include "stationDataManager.h"
#include "plugininterface.h"
#include <iostream>
#include <QQmlEngine>

StationInfoModel::StationInfoModel(QObject *parent) : QAbstractListModel(parent)
{

}


int StationInfoModel::rowCount(const QModelIndex &/*parent*/) const {
    return 1;
}

QVariant StationInfoModel::data(const QModelIndex &/*index*/, int role) const{
    switch (role) {
        case Numberr: return QVariant::fromValue<int>(mStationInfo.Number);
        case LocationInfo: return QVariant::fromValue<QStringList>(getLocationInfo());
        case LocationInfoHeaders: return QVariant::fromValue<QStringList>(getLocationInfoHeaders());
        case Active: return QVariant::fromValue<bool>(mStationInfo.RadarSearchStatus == StationInfo::S);
        case MainInfo: return QVariant::fromValue<QStringList>(getMainInfo());
        case MainInfoHeaders: return QVariant::fromValue<QStringList>(getMainInfoHeaders());
        default: return mStationInfo.Name;
    }
}

QStringList StationInfoModel::getMainInfo() const {
    QStringList tmp;
    tmp << mStationInfo.Name << QString::number(mStationInfo.Number)
        << mStationInfo.Type <<mStationInfo.radarStatusToString(mStationInfo.RadarSearchStatus) << mStationInfo.PrimSec;
    return tmp;
}

QStringList StationInfoModel::getMainInfoHeaders() const
{
    return QStringList {"Name" , "Number", "Type", "RadarSearch Status", "Prim/Sec"};
}

QStringList StationInfoModel::getLocationInfo() const
{
    return QStringList {QString("%L1").arg(mStationInfo.Latitude),
                        QString("%L1").arg(mStationInfo.Longitude),
                        QString("%L1").arg(mStationInfo.Altitude),
                        QString("%L1").arg(mStationInfo.Radius) + " m",
                        QString("%L1").arg(mStationInfo.CycleTime) + " s"};
}

QStringList StationInfoModel::getLocationInfoHeaders() const
{
    return QStringList {"Latitude", "Longitude",  "Altitude", "Radius", "CycleTime"};
}

QHash<int, QByteArray> StationInfoModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[Numberr] = "Numberr";
    hash[Active] = "Active";
    hash[MainInfo] = "MainInfo";
    hash[MainInfoHeaders] = "MainInfoHeaders";
    hash[LocationInfo] = "LocationInfo";
    hash[LocationInfoHeaders] = "LocationInfoHeaders";
    return hash;
}

void StationInfoModel::setInformtion(const StationInfo &stationInfo)
{
    mStationInfo = stationInfo;
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

StationInformtion::StationInformtion(DefenseModelLayer *defenseModelLayer, Station::Data *data, QObject *parent) :
	QObject(parent),
	mDefenseModelLayer(defenseModelLayer),
	mData(data)
{
    QQmlComponent *comp = new QQmlComponent(defenseModelLayer->qmlEngine());
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){

        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mInfoModel = new StationInfoModel;

			mInfoModel->setInformtion(mData->info);
            mItem->setProperty("model", QVariant::fromValue<StationInfoModel*>(mInfoModel));
//            QQmlEngine::setObjectOwnership(mItem, QQmlEngine::JavaScriptOwnership);
        }

    });

    comp->loadUrl(QUrl("qrc:/modelplugin/StationInfoView.qml"));
}

void StationInformtion::show()
{
    mDefenseModelLayer->uiHandle()->iwShow(mItem, QString::number(mData->info.Number));
}
