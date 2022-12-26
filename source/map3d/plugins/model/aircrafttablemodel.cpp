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
    return static_cast<int>(mAircraftInfoListProxy.size()) + 1;
}

QVariant AircraftTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mAircraftInfoListProxy.size()))
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
            case  4: return QVariant::fromValue<QString>("Master Radar");
            case  5: return QVariant::fromValue<QString>("Identification");
            case  6: return QVariant::fromValue<QString>("Identification Method");
            case  7: return QVariant::fromValue<QString>("Time");
            case  8: return QVariant::fromValue<QString>("Pos");
            case  9: return QVariant::fromValue<QString>("Latitude");
            case 10: return QVariant::fromValue<QString>("Longitude");
            case 11: return QVariant::fromValue<QString>("Altitude");
            case 12: return QVariant::fromValue<QString>("Heading");
            case 13: return QVariant::fromValue<QString>("Speed");
            case 14: return QVariant::fromValue<QString>("Detection Systems");
            case 15: return QVariant::fromValue<QString>("Sends");
            }

        } else {
            switch(index.column()) {
            case  0: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->TN);
            case  1: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->IFFCode);
            case  2: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->CallSign);
            case  3: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->Type);
            case  4: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->MasterRadar);
            case  5: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->Identification);
            case  6: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->IdentificationMethod);
            case  7: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->Time);
            case  8: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->Pos);
            case  9: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->Latitude);
            case 10: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->Longitude);
            case 11: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->Altitude);
            case 12: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->Heading);
            case 13: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->Speed);
            case 14: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->DetectionSystems);
            case 15: return QVariant::fromValue<QString>(mAircraftInfoListProxy[(size_t)(index.row()-1)]->Sends);
            }
        }

        break;
    }

    case BackColorRole:
    {
        if (index.row() == 0) {
            return QVariant::fromValue<QColor>(QColor("skyblue"));
        } else {
            return QVariant::fromValue<QColor>(QColor("darkkhaki"));
        }

        break;
    }

    case TextColorRole:
    {
        return QVariant::fromValue<QColor>(QColor("black"));
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
    return hash;
}

QString AircraftTableModel::getTN(int row) const
{
    if (row < 1) return QString();

    if (row > static_cast<int>(mAircraftInfoListProxy.size())) {
        return QString();
    }

    return mAircraftInfoListProxy[std::size_t(row-1)]->TN;
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

