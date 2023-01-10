#include "stationinformation.h"
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
        case Type: return QVariant::fromValue<QString>(mStationInfo.Type);
        case PrimSec: return QVariant::fromValue<QString>(mStationInfo.PrimSec);
        case Latitude: return QVariant::fromValue<double>(mStationInfo.Latitude);
        case Longitude: return QVariant::fromValue<double>(mStationInfo.Longitude);
        case Numberr: return QVariant::fromValue<double>(mStationInfo.Number);
        case Radius: return QVariant::fromValue<double>(mStationInfo.Radius);
        case CycleTime: return QVariant::fromValue<int>(mStationInfo.CycleTime);
        case Active: return QVariant::fromValue<bool>(mStationInfo.Active);
        default: return mStationInfo.Name;
    }
}


QHash<int, QByteArray> StationInfoModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[Name] = "Name";
    hash[Type] = "Type";
    hash[PrimSec] = "PrimSec";
    hash[Numberr] = "Numberr";
    hash[Latitude] = "Latitude";
    hash[Longitude] = "Longitude";
    hash[Radius] = "Radius";
    hash[CycleTime] = "CycleTime";
    hash[Active] = "Active";
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
        qDebug() << comp->errorString();

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
