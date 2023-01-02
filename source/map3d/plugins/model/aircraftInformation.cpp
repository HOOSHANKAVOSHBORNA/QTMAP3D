#include "aircraftInformation.h"
#include <QDebug>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include "aircraftmodelnode.h"
#include "plugininterface.h"


InfoModel::InfoModel(QObject* parent): QAbstractListModel(parent)
{

}

int InfoModel::rowCount(const QModelIndex &/*parent*/) const {
    return 1;
}

QVariant InfoModel::data(const QModelIndex &/*index*/, int role) const{
    switch (role) {
        case TN: return QVariant::fromValue<QString>(aircraftInfo.TN);
        case IFFCode: return QVariant::fromValue<QString>(aircraftInfo.IFFCode);
        case CallSign: return QVariant::fromValue<QString>(aircraftInfo.CallSign);
        case Type: return QVariant::fromValue<QString>(aircraftInfo.Type);
        case Master: return QVariant::fromValue<QString>(aircraftInfo.MasterRadar);
        case Identification: return QVariant::fromValue<QString>(aircraftInfo.Identification);
        case IdentificationMethod: return QVariant::fromValue<QString>(aircraftInfo.IdentificationMethod);
        case Time: return QVariant::fromValue<QString>(aircraftInfo.Time);
        case Pos: return QVariant::fromValue<QString>(aircraftInfo.Pos);
        case Latitude: return QVariant::fromValue<double>(aircraftInfo.Latitude);
        case Longitude: return QVariant::fromValue<double>(aircraftInfo.Longitude);
        case Altitude: return QVariant::fromValue<double>(aircraftInfo.Altitude);
        case Heading: return QVariant::fromValue<double>(aircraftInfo.Heading);
        case Speed: return QVariant::fromValue<double>(aircraftInfo.Speed);
        case DetectionSystems: return QVariant::fromValue<QStringList>(aircraftInfo.DetectionSystems);
        case Sends: return QVariant::fromValue<QStringList>(aircraftInfo.Sends);
    }
    return aircraftInfo.TN;
}

void InfoModel::setAircraftInfo(AircraftInfo &a)
{
    aircraftInfo = a;
}

QHash<int, QByteArray> InfoModel::roleNames() const
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

AircraftInformation::AircraftInformation(QQmlEngine *mQmlEngine, UIHandle *muiHandle, const AircraftInfo minformation, QObject *parent): mInformation(minformation), mUiHandle(muiHandle)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
        qDebug() << comp->errorString();

        if (comp->status() == QQmlComponent::Ready) {
            item = static_cast<QQuickItem*>(comp->create(nullptr));
            infomodel = new InfoModel;

            infomodel->setAircraftInfo(mInformation);
            item->setProperty("model", QVariant::fromValue<InfoModel*>(infomodel));
            QQmlEngine::setObjectOwnership(item, QQmlEngine::JavaScriptOwnership);

        }

    });

    comp->loadUrl(QUrl("qrc:/modelplugin/InfoView.qml"));
}
void AircraftInformation::show() {
    mUiHandle->iwShow(item);
}
