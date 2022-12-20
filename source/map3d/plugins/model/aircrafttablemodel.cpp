#include "aircrafttablemodel.h"

AircraftTableModel::AircraftTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{

    mAircraftInfoList.push_back(AircraftInfo{"xxx"});
}

int AircraftTableModel::columnCount(const QModelIndex &parent) const
{
    return 16;
}

int AircraftTableModel::rowCount(const QModelIndex &parent) const
{
    return static_cast<int>(mAircraftInfoList.size()) + 1;
}

QVariant AircraftTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mAircraftInfoList.size()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        if (index.row() == 0) {

            switch(index.column()) {
            case  0: return QVariant::fromValue<QString>("TN");
            case  1: return QVariant::fromValue<QString>("IFFCode");
            case  2: return QVariant::fromValue<QString>("CallSign");
            case  3: return QVariant::fromValue<QString>("Type");
            case  4: return QVariant::fromValue<QString>("MasterRadar");
            case  5: return QVariant::fromValue<QString>("Identification");
            case  6: return QVariant::fromValue<QString>("IdentificationMethod");
            case  7: return QVariant::fromValue<QString>("Time");
            case  8: return QVariant::fromValue<QString>("Pos");
            case  9: return QVariant::fromValue<QString>("Latitude");
            case 10: return QVariant::fromValue<QString>("Longitude");
            case 11: return QVariant::fromValue<QString>("Altitude");
            case 12: return QVariant::fromValue<QString>("Heading");
            case 13: return QVariant::fromValue<QString>("Speed");
            case 14: return QVariant::fromValue<QString>("DetectionSystems");
            case 15: return QVariant::fromValue<QString>("Sends");
            }

        } else {
            switch(index.column()) {
            case  0: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].TN);
            case  1: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].IFFCode);
            case  2: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].CallSign);
            case  3: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].Type);
            case  4: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].MasterRadar);
            case  5: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].Identification);
            case  6: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].IdentificationMethod);
            case  7: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].Time);
            case  8: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].Pos);
            case  9: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].Latitude);
            case 10: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].Longitude);
            case 11: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].Altitude);
            case 12: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].Heading);
            case 13: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].Speed);
            case 14: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].DetectionSystems);
            case 15: return QVariant::fromValue<QString>(mAircraftInfoList[(size_t)(index.row()-1)].Sends);
            }
        }

        break;
    }
    }

    return QVariant();
}

void AircraftTableModel::updateItemData(const QString &jsonStr)
{

}

void AircraftTableModel::updateItemData(const AircraftInfo &aircraftInfo)
{

}

