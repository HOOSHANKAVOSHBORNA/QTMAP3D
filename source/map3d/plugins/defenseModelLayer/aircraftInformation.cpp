#include "aircraftInformation.h"
#include <QDebug>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include "aircraftModelNode.h"
#include "plugininterface.h"


AircraftInfoModel::AircraftInfoModel(QObject* parent): QAbstractListModel(parent)
{

}

int AircraftInfoModel::rowCount(const QModelIndex &/*parent*/) const {
    return 1;
}

QVariant AircraftInfoModel::data(const QModelIndex &/*index*/, int role) const{
    switch (role) {
        case TN: return QVariant::fromValue<QString>(mAircraftInfo.TN);
        case IFFCode: return QVariant::fromValue<QString>(mAircraftInfo.IFFCode);
        case CallSign: return QVariant::fromValue<QString>(mAircraftInfo.CallSign);
        case Type: return QVariant::fromValue<QString>(mAircraftInfo.Type);
        case Master: return QVariant::fromValue<QString>(mAircraftInfo.MasterRadar);
        case Identification: return QVariant::fromValue<QString>(mAircraftInfo.Identification);
        case IdentificationMethod: return QVariant::fromValue<QString>(mAircraftInfo.IdentificationMethod);
        case Time: return QVariant::fromValue<QString>(mAircraftInfo.Time);
        case Pos: return QVariant::fromValue<QString>(mAircraftInfo.Pos);
        case Latitude: return QVariant::fromValue<double>(mAircraftInfo.Latitude);
        case Longitude: return QVariant::fromValue<double>(mAircraftInfo.Longitude);
        case Altitude: return QVariant::fromValue<double>(mAircraftInfo.Altitude);
        case Heading: return QVariant::fromValue<double>(mAircraftInfo.Heading);
        case Speed: return QVariant::fromValue<double>(mAircraftInfo.Speed);
        case DetectionSystems: return QVariant::fromValue<QStringList>(mAircraftInfo.DetectionSystems);
        case Sends: return QVariant::fromValue<QStringList>(mAircraftInfo.Sends);
        default: return mAircraftInfo.TN;
    }
}

void AircraftInfoModel::setAircraftInfo(AircraftInfo &a)
{
    mAircraftInfo = a;
}

QHash<int, QByteArray> AircraftInfoModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[TN] = "TN";
    hash[IFFCode] = "IFFCode";
    hash[CallSign] = "CallSign";
    hash[Type] = "Type";
    hash[Master] = "Master";
    hash[Identification] = "Identification";
    hash[IdentificationMethod] = "IdentificationMethod";
    hash[Time] = "Time";
    hash[Pos] = "Pos";
    hash[Latitude] = "Latitude";
    hash[Longitude] = "Longitude";
    hash[Altitude] = "Altitude";
    hash[Heading] = "Heading";
    hash[Speed] = "Speed";
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
        qDebug() << comp->errorString();

        if (comp->status() == QQmlComponent::Ready) {
            mItem = static_cast<QQuickItem*>(comp->create(nullptr));
            mInfomodel = new AircraftInfoModel;

            mInfomodel->setAircraftInfo(mInformation);
            mItem->setProperty("model", QVariant::fromValue<AircraftInfoModel*>(mInfomodel));
            QQmlEngine::setObjectOwnership(mItem, QQmlEngine::JavaScriptOwnership);
        }

    });

    comp->loadUrl(QUrl("qrc:/modelplugin/AircraftInfoView.qml"));
}
void AircraftInformation::show() {
    mUiHandle->iwShow(mItem);
}
