#include "stationtablemodel.h"

#include <QColor>

StationTableModel::StationTableModel(QObject *parent): QAbstractTableModel(parent)
{

}

int StationTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 7;
}

int StationTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(mStationInfoListProxy.size());
}

QVariant StationTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mStationInfoListProxy.size()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        switch(index.column()) {
        case  0: return QVariant::fromValue<QString>(QString::number(mStationInfoListProxy[static_cast<size_t>(index.row())].first));
        case  1: return QVariant::fromValue<QString>(mStationInfoListProxy[static_cast<size_t>(index.row())].second->Name);
        case  2: return QVariant::fromValue<QString>(mStationInfoListProxy[static_cast<size_t>(index.row())].second->Type);
        case  3: return QVariant::fromValue<QString>(mStationInfoListProxy[static_cast<size_t>(index.row())].second->PrimSec);
        case  4: return QVariant::fromValue<double>(mStationInfoListProxy[static_cast<size_t>(index.row() )].second->Number);
        case  5: return QVariant::fromValue<double>(mStationInfoListProxy[static_cast<size_t>(index.row() )].second->Latitude);
        case  6: return QVariant::fromValue<double>(mStationInfoListProxy[static_cast<size_t>(index.row() )].second->Longitude);
        }

        break;
    }

    case BackColorRole:
    {
        return QVariant::fromValue<QColor>(QColor("transparent"));

        break;
    }

    case TextColorRole:
    {
        return QVariant::fromValue<QColor>(QColor("white"));
        break;
    }

    case HeaderTextRole:
    {
        switch(index.row()) {
        case  0: return QVariant::fromValue<QString>("Index");
        case  1: return QVariant::fromValue<QString>("Name");
        case  2: return QVariant::fromValue<QString>("TYpe");
        case  3: return QVariant::fromValue<QString>("Primary/Secondry");
        case  4: return QVariant::fromValue<QString>("Number");
        case  5: return QVariant::fromValue<QString>("Latitude");
        case  6: return QVariant::fromValue<QString>("Longitude");
        }
        break;
    }
    }

    return QVariant();
}

QHash<int, QByteArray> StationTableModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractTableModel::roleNames();
    hash[BackColorRole] = "d_bkcolor";
    hash[TextColorRole] = "d_txtcolor";
    hash[HeaderTextRole] = "d_headerTxt";
    return hash;
}


QString StationTableModel::headerText(int column) const
{
    switch(column) {
    case  0: return QStringLiteral("Index");
    case  1: return QStringLiteral("Name");
    case  2: return QStringLiteral("Type");
    case  3: return QStringLiteral("Primary/Secondary");
    case  4: return QStringLiteral("Number");
    case  5: return QStringLiteral("Latitude");
    case  6: return QStringLiteral("Longitude");
    }

    return QStringLiteral("");
}

QString StationTableModel::getName(int row) const
{
    if (row < 0) return QString();

    if (row >= static_cast<int>(mStationInfoListProxy.size())) {
        return QString();
    }

    return mStationInfoListProxy[std::size_t(row)].second->Name;
}


void StationTableModel::updateItemData(const QString &jsonStr)
{

}

void StationTableModel::updateItemData(const StationInfo &stationInfo)
{
    beginResetModel();

    const auto it = std::find_if(mStationInfoList.begin(), mStationInfoList.end(),
                                 [stationInfo](const QPair<int, QSharedPointer<StationInfo>>& itemInfo){
        return itemInfo.second->Name == stationInfo.Name;
    });


    if (it != mStationInfoList.end()) {
        *(*it).second = stationInfo;
    } else {
        QPair<int, QSharedPointer<StationInfo>> isp;
        isp.first = mStationInfoList.size();
        isp.second.reset(new StationInfo);
        *(isp.second) = stationInfo;
        mStationInfoList.push_back(isp);
    }

    mStationInfoListProxy.clear();
    for (auto& item : mStationInfoList) {
        if (item.second->Name.contains(mFilter))
            mStationInfoListProxy.push_back(item);
    }

    endResetModel();

}
