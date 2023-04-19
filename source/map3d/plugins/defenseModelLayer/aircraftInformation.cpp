#include "aircraftInformation.h"
#include <QDebug>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include "aircraftModelNode.h"
#include "plugininterface.h"
#include <time.h>


AircraftInfoModel::AircraftInfoModel(QObject* parent): QAbstractListModel(parent)
{

}

int AircraftInfoModel::rowCount(const QModelIndex &/*parent*/) const {
    return 1;
}

QVariant AircraftInfoModel::data(const QModelIndex &/*index*/, int role) const{
    switch (role) {
        case TN: return QVariant::fromValue<int>(mAircraftInfo.TN);
        case MainInfo: return QVariant::fromValue<QStringList>(getMainInfo());
        case LocationInfo: return QVariant::fromValue<QStringList>(getLocationInfo());
        case LocationInfoHeaders: return QVariant::fromValue<QStringList>(getLocationInfoHeader());
        case MainInfoHeaders: return QVariant::fromValue<QStringList>(getmainInfoHeaders());
        case DetectionSystems: return QVariant::fromValue<QStringList>(mAircraftInfo.DetectionSystems);
        case Sends: return QVariant::fromValue<QStringList>(mAircraftInfo.Sends);
        default: return mAircraftInfo.TN;
    }
}

void AircraftInfoModel::setAircraftInfo(AircraftInfo &a)
{
//    beginResetModel();
    mAircraftInfo = a;
//    endResetModel();
    QAbstractListModel::dataChanged(createIndex(0, 0), createIndex(1, 0));
}

QStringList AircraftInfoModel::getMainInfo() const
{
    time_t datetime = mAircraftInfo.Time;
    char buffer[256];
    std::tm* currTm = localtime(&datetime);
    strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M", currTm);
    return QStringList {QString::number(mAircraftInfo.TN), mAircraftInfo.identifyToString(), mAircraftInfo.CallSign,
                mAircraftInfo.IFFCode, mAircraftInfo.aircraftTypeToString(), mAircraftInfo.MasterRadar,
                mAircraftInfo.IdentificationMethod, buffer};
}

QStringList AircraftInfoModel::getmainInfoHeaders() const
{
    return QStringList {"TN", "Identification", "CallSign", "IFFCode", "Type", "Master"
        , "IdentificationMethod", "Time"};
}

QStringList AircraftInfoModel::getLocationInfo() const
{
    return QStringList {QString("%L1").arg(mAircraftInfo.Latitude),
                        QString("%L1").arg(mAircraftInfo.Longitude),
                        QString("%L1").arg(mAircraftInfo.Altitude),
                        mAircraftInfo.Pos,
                        QString::number(mAircraftInfo.Heading),
                        QString("%L1").arg(mAircraftInfo.Speed) + " m/s"};
}

QStringList AircraftInfoModel::getLocationInfoHeader() const
{
    return QStringList {"Latitude", "Longitude", "Altitude", "Pos", "Heading", "Speed"};
}

QColor AircraftInfoModel::getAircraftColor()
{
    return mAircraftInfo.aircraftColor();
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
    return hash;
}

AircraftInformation::AircraftInformation(QQmlEngine *mQmlEngine, UIHandle *muiHandle, const AircraftInfo minformation, QObject *parent)
    :QObject(parent),
    mInformation(minformation), mUiHandle(muiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){

        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mInfomodel = new AircraftInfoModel;

            mInfomodel->setAircraftInfo(mInformation);
            mItem->setProperty("model", QVariant::fromValue<AircraftInfoModel*>(mInfomodel));
//            QQmlEngine::setObjectOwnership(mItem, QQmlEngine::JavaScriptOwnership);
        }

    });

    comp->loadUrl(QUrl("qrc:/modelplugin/AircraftInfoView.qml"));
}

void AircraftInformation::updateAircraft(AircraftInfo &mInformation)
{
    mInfomodel->setAircraftInfo(mInformation);
}
void AircraftInformation::show() {
    mUiHandle->iwShow(mItem, QString::number(mInformation.TN));
}
