#include "systemtablemodel.h"

#include <QColor>
#include <QRegularExpression>

SystemTableModel::SystemTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

int SystemTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 23;
}

int SystemTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(mSystemInfoListProxy.size());
}

QVariant SystemTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mSystemInfoListProxy.size()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        switch(index.column()) {
        case  0: return QVariant::fromValue<QString>(QString::number(mSystemInfoListProxy[static_cast<size_t>(index.row())].first));
        case  1: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->Name);
        case  2: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->Number);
        case  3: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->Type);
        case  4: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Terminal);
        case  5: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Latitude);
        case  6: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Longitude);
        case  7: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Altitude);
        case  8: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row()    )].second->ViewRange);
        case  9: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->MezRange);
        case 10: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->ReceiveTime);
        case 11: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Simulation);
        case 12: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->RadarSearchStatus);
        case 13: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Operational);
        case 14: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->MissileCount);
        case 15: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row()    )].second->RadarMode);
        case 16: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->TN);
        case 17: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->Acceptance);
        case 18: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Phase);
        case 19: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Antenna);
        case 20: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->ChanelNo);
        case 21: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Inrange);

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
        switch (index.row()) {
        case  0: return QVariant::fromValue<QString>("Index");
        case  1: return QVariant::fromValue<QString>("Name");
        case  2: return QVariant::fromValue<QString>("Number");
        case  3: return QVariant::fromValue<QString>("Type");
        case  4: return QVariant::fromValue<QString>("Terminal");
        case  5: return QVariant::fromValue<QString>("Latitude");
        case  6: return QVariant::fromValue<QString>("Longitude");
        case  7: return QVariant::fromValue<QString>("Altitude");
        case  8: return QVariant::fromValue<QString>("ViewRange");
        case  9: return QVariant::fromValue<QString>("MezRange");
        case 10: return QVariant::fromValue<QString>("ReceiveTime");
        case 11: return QVariant::fromValue<QString>("Simulation");
        case 12: return QVariant::fromValue<QString>("RadarSearchStatus");
        case 13: return QVariant::fromValue<QString>("Operational");
        case 14: return QVariant::fromValue<QString>("MissileCount");
        case 15: return QVariant::fromValue<QString>("RadarMode");
        case 16: return QVariant::fromValue<QString>("TN");
        case 17: return QVariant::fromValue<QString>("Acceptance");
        case 18: return QVariant::fromValue<QString>("Phase");
        case 19: return QVariant::fromValue<QString>("Antenna");
        case 20: return QVariant::fromValue<QString>("ChanelNo");
        case 21: return QVariant::fromValue<QString>("Inrange");

        }
        break;
    }

    }
    return QVariant();
}

QHash<int, QByteArray> SystemTableModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractTableModel::roleNames();
    hash[BackColorRole] = "d_bkcolor";
    hash[TextColorRole] = "d_txtcolor";
    hash[HeaderTextRole] = "d_headerTxt";
    return hash;
}

QString SystemTableModel::headerText(int column) const
{
    switch (column) {
    case  0: return QStringLiteral("Index");
    case  1: return QStringLiteral("Name");
    case  2: return QStringLiteral("Number");
    case  3: return QStringLiteral("Type");
    case  4: return QStringLiteral("Terminal");
    case  5: return QStringLiteral("Latitude");
    case  6: return QStringLiteral("Longitude");
    case  7: return QStringLiteral("Altitude");
    case  8: return QStringLiteral("ViewRange");
    case  9: return QStringLiteral("MezRange");
    case 10: return QStringLiteral("ReceiveTime");
    case 11: return QStringLiteral("Simulation");
    case 12: return QStringLiteral("RadarSearchStatus");
    case 13: return QStringLiteral("Operational");
    case 14: return QStringLiteral("MissileCount");
    case 15: return QStringLiteral("RadarMode");
    case 16: return QStringLiteral("TN");
    case 17: return QStringLiteral("Acceptance");
    case 18: return QStringLiteral("Phase");
    case 19: return QStringLiteral("Antenna");
    case 20: return QStringLiteral("ChanelNo");
    case 21: return QStringLiteral("Inrange");
    default: QStringLiteral("");
    }
}

QString SystemTableModel::getName(int row) const
{
    if (row < 0) return QString();

    if (row >= static_cast<int>(mSystemInfoListProxy.size())) {
        return QString();
    }

    return mSystemInfoListProxy[std::size_t(row)].second->Name;
}

void SystemTableModel::setFilterWildcard(const QString &wildcard)
{
    beginResetModel();

    mFilter = wildcard;
    mFilter.remove(QRegularExpression("\\s"));

    mSystemInfoListProxy.clear();
    for (auto& item : mSystemInfoList) {
        if (item.second->Name.contains(mFilter))
            mSystemInfoListProxy.push_back(item);
    }

    endResetModel();
}

void SystemTableModel::updateItemData(const SystemInfo &systemInfo)
{
    beginResetModel();

    const auto it = std::find_if(mSystemInfoList.begin(), mSystemInfoList.end(),
                                 [systemInfo](const QPair<int, QSharedPointer<SystemInfo>>& itemInfo){
        return itemInfo.second->TN == systemInfo.Name;
    });


    if (it != mSystemInfoList.end()) {
        *(*it).second = systemInfo;
    } else {
        QPair<int, QSharedPointer<SystemInfo>> isp;
        isp.first = mSystemInfoList.size();
        isp.second.reset(new SystemInfo);
        *(isp.second) = systemInfo;
        mSystemInfoList.push_back(isp);
    }

    mSystemInfoListProxy.clear();
    for (auto& item : mSystemInfoList) {
        if (item.second->Name.contains(mFilter))
            mSystemInfoListProxy.push_back(item);
    }

    endResetModel();
}

