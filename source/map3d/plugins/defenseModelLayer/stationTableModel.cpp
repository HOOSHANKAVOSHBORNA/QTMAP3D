#include "stationTableModel.h"

#include <QColor>
#include <QRegularExpression>

StationTableModel::StationTableModel(QObject *parent): QAbstractTableModel(parent)
{

}

int StationTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 9;
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
        case  0: return QVariant::fromValue<double>(mStationInfoListProxy[static_cast<size_t>(index.row() )]->Number);
        case  1: return QVariant::fromValue<QString>(mStationInfoListProxy[static_cast<size_t>(index.row())]->Name);
        case  2: return QVariant::fromValue<QString>(mStationInfoListProxy[static_cast<size_t>(index.row())]->Type);
        case  3: return QVariant::fromValue<QString>(mStationInfoListProxy[static_cast<size_t>(index.row())]->PrimSec);
        case  4: return QVariant::fromValue<double>(mStationInfoListProxy[static_cast<size_t>(index.row() )]->Latitude);
        case  5: return QVariant::fromValue<double>(mStationInfoListProxy[static_cast<size_t>(index.row() )]->Longitude);
        case  6: return QVariant::fromValue<double>(mStationInfoListProxy[static_cast<size_t>(index.row() )]->Radius);
        case  7: return QVariant::fromValue<int>(mStationInfoListProxy[static_cast<size_t>(index.row()    )]->CycleTime);
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
        case  0: return QVariant::fromValue<QString>("Number");
        case  1: return QVariant::fromValue<QString>("Name");
        case  2: return QVariant::fromValue<QString>("Type");
        case  3: return QVariant::fromValue<QString>("Primary/Secondry");
        case  4: return QVariant::fromValue<QString>("Latitude");
        case  5: return QVariant::fromValue<QString>("Longitude");
        case  6: return QVariant::fromValue<QString>("Radius");
        case  7: return QVariant::fromValue<QString>("CycleTime");
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
    case  0: return QStringLiteral("Number");
    case  1: return QStringLiteral("Name");
    case  2: return QStringLiteral("Type");
    case  3: return QStringLiteral("Primary/Secondary");
    case  4: return QStringLiteral("Latitude");
    case  5: return QStringLiteral("Longitude");
    case  6: return QStringLiteral("Radius");
    case  7: return QStringLiteral("CycleTime");
    }

    return QStringLiteral("");
}

int StationTableModel::getNumber(int row) const
{
    if (row < 0) return -1;

    if (row >= static_cast<int>(mStationInfoListProxy.size())) {
        return -1;
    }

    return mStationInfoListProxy[std::size_t(row)]->Number;
}

void StationTableModel::setFilterWildcard(const QString &wildcard)
{
    beginResetModel();

    mFilter = wildcard;
    mFilter.remove(QRegularExpression("\\s"));

    mStationInfoListProxy.clear();
    for (auto& item : mStationInfoList) {
        if (QString::number(item->Number).contains(mFilter))
            mStationInfoListProxy.push_back(item);
    }

    endResetModel();
}


void StationTableModel::updateItemData(const StationInfo &stationInfo)
{
    beginResetModel();

    const auto it = std::find_if(mStationInfoList.begin(), mStationInfoList.end(),
                                 [stationInfo](const QSharedPointer<StationInfo>& itemInfo){
        return itemInfo->Number == stationInfo.Number;
    });


    if (it != mStationInfoList.end()) {
        *(*it) = stationInfo;
    } else {
        QSharedPointer<StationInfo> isp;
        isp.reset(new StationInfo);
        *(isp) = stationInfo;
        mStationInfoList.push_back(isp);
    }

    mStationInfoListProxy.clear();
    for (auto& item : mStationInfoList) {
        if (QString::number(item->Number).contains(mFilter))
            mStationInfoListProxy.push_back(item);
    }

    endResetModel();

}

void StationTableModel::clear()
{
    beginResetModel();
    mStationInfoList.clear();
    mStationInfoListProxy.clear();
    endResetModel();
}
