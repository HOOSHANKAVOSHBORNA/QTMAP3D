#include "stationinformation.h"

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
        case Latitude: return QVariant::fromValue<QString>(mStationInfo.Type);
        case Longitude: return QVariant::fromValue<double>(mStationInfo.Longitude);
        case Numberr: return QVariant::fromValue<double>(mStationInfo.Number);
        default: return mStationInfo.Name;
    }
}


QHash<int, QByteArray> StationInfoModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[Name] = "Name";
    hash[Type] = "Type";
    hash[PrimSec] = "PrimSec";
    hash[Numberr] = "Number";
    hash[Latitude] = "Latitude";
    hash[Longitude] = "Longitude";
    return hash;
}
