#include "stationInformation.h"
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
        case Name: return QVariant::fromValue<QString>(mStationInfo.Name);
        case LocationInfo: return QVariant::fromValue<QStringList>(getLocationInfo());
        case LocationInfoHeaders: return QVariant::fromValue<QStringList>(getLocationInfoHeaders());
        case Active: return QVariant::fromValue<bool>(mStationInfo.Active);
        case MainInfo: return QVariant::fromValue<QStringList>(getMainInfo());
        case MainInfoHeaders: return QVariant::fromValue<QStringList>(getMainInfoHeaders());
        default: return mStationInfo.Name;
    }
}

QStringList StationInfoModel::getMainInfo() const {
    QStringList tmp;
    tmp << mStationInfo.Name << QString::number(mStationInfo.Number) << mStationInfo.Type << mStationInfo.PrimSec;
    return tmp;
}

QStringList StationInfoModel::getMainInfoHeaders() const
{
    return QStringList {"Name" , "Number", "Type", "Prim/Sec"};
}

QStringList StationInfoModel::getLocationInfo() const
{
    return QStringList {QString::number(mStationInfo.Latitude),
                        QString::number(mStationInfo.Longitude),
                        QString::number(mStationInfo.Radius),
                        QString::number(mStationInfo.CycleTime)};
}

QStringList StationInfoModel::getLocationInfoHeaders() const
{
    return QStringList {"Latitude", "Longitude", "Radius", "CycleTime"};
}

QHash<int, QByteArray> StationInfoModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[Name] = "Name";
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
}

StationInformtion::StationInformtion(QQmlEngine *qmlEngine, UIHandle *uiHandle, StationInfo stationInfo, QObject *parent) :
    QObject(parent), mInformation(stationInfo), mUiHandle(uiHandle)
{
    QQmlComponent *comp = new QQmlComponent(qmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){

        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mInfoModel = new StationInfoModel;

            mInfoModel->setInformtion(mInformation);
            mItem->setProperty("model", QVariant::fromValue<StationInfoModel*>(mInfoModel));
            QQmlEngine::setObjectOwnership(mItem, QQmlEngine::JavaScriptOwnership);
        }

    });

    comp->loadUrl(QUrl("qrc:/modelplugin/StationInfoView.qml"));
}

void StationInformtion::show()
{
    mUiHandle->iwShow(mItem);
}
