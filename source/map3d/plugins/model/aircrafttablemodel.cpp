#include "aircrafttablemodel.h"
#include <QHash>
#include <QColor>
#include <QRegularExpression>

AircraftTableModel::AircraftTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    //    for (int i = 0; i < 100; i++) {
    //        AircraftInfo ai;
    //        ai.TN = QStringLiteral("OOPS") + QString::number(i);
    //        updateItemData(ai);
    //    }
}

int AircraftTableModel::columnCount(const QModelIndex &parent) const
{
    return 16;
}

int AircraftTableModel::rowCount(const QModelIndex &parent) const
{
    return static_cast<int>(mAircraftInfoListProxy.size());
}

QVariant AircraftTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mAircraftInfoListProxy.size()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        switch(index.column()) {
        case  0: return QVariant::fromValue<QString>(QString::number(index.row()));
        case  1: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->TN);
        case  2: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->IFFCode);
        case  3: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->CallSign);
        case  4: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->Type);
        case  5: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->MasterRadar);
        case  6: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->Identification);
        case  7: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->IdentificationMethod);
        case  8: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->Time);
        case  9: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->Pos);
        case 10: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(index.row() )]->Latitude);
        case 11: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(index.row() )]->Longitude);
        case 12: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(index.row() )]->Altitude);
        case 13: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(index.row() )]->Heading);
        case 14: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(index.row() )]->Speed);
        case 15: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->detectionSystemsToString());
        case 16: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(index.row())]->sendsToString());
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
        case  1: return QVariant::fromValue<QString>("TN");
        case  2: return QVariant::fromValue<QString>("IFFCode");
        case  3: return QVariant::fromValue<QString>("CallSign");
        case  4: return QVariant::fromValue<QString>("Type");
        case  5: return QVariant::fromValue<QString>("Master Radar");
        case  6: return QVariant::fromValue<QString>("Identification");
        case  7: return QVariant::fromValue<QString>("Identify Method");
        case  8: return QVariant::fromValue<QString>("Time");
        case  9: return QVariant::fromValue<QString>("Pos");
        case 10: return QVariant::fromValue<QString>("Latitude");
        case 11: return QVariant::fromValue<QString>("Longitude");
        case 12: return QVariant::fromValue<QString>("Altitude");
        case 13: return QVariant::fromValue<QString>("Heading");
        case 14: return QVariant::fromValue<QString>("Speed");
        case 15: return QVariant::fromValue<QString>("Detection Systems");
        case 16: return QVariant::fromValue<QString>("Sends");
        }

        break;
    }


    }

    return QVariant();
}

QHash<int, QByteArray> AircraftTableModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractTableModel::roleNames();
    hash[BackColorRole] = "d_bkcolor";
    hash[TextColorRole] = "d_txtcolor";
    hash[HeaderTextRole] = "d_headerTxt";
    return hash;
}

QString AircraftTableModel::headerText(int column) const
{
    switch(column) {
    case  0: return QStringLiteral("Index");
    case  1: return QStringLiteral("TN");
    case  2: return QStringLiteral("IFFCode");
    case  3: return QStringLiteral("CallSign");
    case  4: return QStringLiteral("Type");
    case  5: return QStringLiteral("Master Radar");
    case  6: return QStringLiteral("Identification");
    case  7: return QStringLiteral("Identification Method");
    case  8: return QStringLiteral("Time");
    case  9: return QStringLiteral("Pos");
    case 10: return QStringLiteral("Latitude");
    case 11: return QStringLiteral("Longitude");
    case 12: return QStringLiteral("Altitude");
    case 13: return QStringLiteral("Heading");
    case 14: return QStringLiteral("Speed");
    case 15: return QStringLiteral("Detection Systems");
    case 16: return QStringLiteral("Sends");
    }

    return QStringLiteral("");
}

QString AircraftTableModel::getTN(int row) const
{
    if (row < 0) return QString();

    if (row >= static_cast<int>(mAircraftInfoListProxy.size())) {
        return QString();
    }

    return mAircraftInfoListProxy[std::size_t(row)]->TN;
}

void AircraftTableModel::setFilterWildcard(const QString &wildcard)
{
    beginResetModel();

    mFilter = wildcard;
    mFilter.remove(QRegularExpression("\\s"));

    mAircraftInfoListProxy.clear();
    for (auto& item : mAircraftInfoList) {
        if (item->TN.contains(mFilter))
            mAircraftInfoListProxy.push_back(item);
    }

    endResetModel();
}

void AircraftTableModel::updateItemData(const QString &jsonStr)
{

}

void AircraftTableModel::updateItemData(const AircraftInfo &aircraftInfo)
{
    beginResetModel();

    const auto it = std::find_if(mAircraftInfoList.begin(), mAircraftInfoList.end(),
                                 [aircraftInfo](const QSharedPointer<AircraftInfo>& itemInfo){
        return itemInfo->TN == aircraftInfo.TN;
    });


    if (it != mAircraftInfoList.end()) {
        *(*it) = aircraftInfo;
    } else {
        QSharedPointer<AircraftInfo> isp(new AircraftInfo);
        *isp = aircraftInfo;
        mAircraftInfoList.push_back(isp);
    }

    mAircraftInfoListProxy.clear();
    for (auto& item : mAircraftInfoList) {
        if (item->TN.contains(mFilter))
            mAircraftInfoListProxy.push_back(item);
    }

    endResetModel();

}

