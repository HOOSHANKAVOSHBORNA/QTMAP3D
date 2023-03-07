#include "aircraftTableModel.h"
#include <QHash>
#include <QColor>
#include <QRegularExpression>
#include <QDebug>
#include <QTimer>
#include <algorithm>
#include <time.h>
#include <iomanip>

AircraftTableModel::AircraftTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    QTimer *timer = new QTimer;
    QObject::connect(timer , &QTimer::timeout,
                     this, &AircraftTableModel::onUpdateTimerTriggered,
                     Qt::DirectConnection);
    timer->start(500);
}

int AircraftTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 16;
}

int AircraftTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(mAircraftInfoListProxy.size());
}

QVariant AircraftTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mAircraftInfoListProxy.size()))
        return QVariant();

    const int _row = index.row();

    switch (role) {
    case Qt::DisplayRole:
    {
        time_t datetime = mAircraftInfoListProxy[static_cast<size_t>(_row)]->Time;
        char buffer[256];
        std::tm* currTm = localtime(&datetime);
        strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M", currTm);
        switch(index.column()) {
        case  0: return QVariant::fromValue<QString>( mAircraftInfoListProxy[static_cast<size_t>(_row)]->identifyToString());
        case  1: return QVariant::fromValue<int>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->TN);
        case  2: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->IFFCode);
        case  3: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->CallSign);
        case  4: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->Type);
        case  5: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->MasterRadar);
        case  6: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->IdentificationMethod);
        case  7: return QVariant::fromValue<QString>(buffer);
        case  8: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->Pos);
        case  9: return QVariant::fromValue<QString>(QString::number(mAircraftInfoListProxy[static_cast<size_t>(_row )]->Latitude, 'f', 6));
        case 10: return QVariant::fromValue<QString>(QString::number(mAircraftInfoListProxy[static_cast<size_t>(_row )]->Longitude, 'f', 6));
        case 11: return QVariant::fromValue<QString>(QString::number(mAircraftInfoListProxy[static_cast<size_t>(_row )]->Altitude, 'f', 6));
        case 12: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(_row )]->Heading);
        case 13: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(_row )]->Speed);
        case 14: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->detectionSystemsToString());
        case 15: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->sendsToString());
        }

        break;
    }


    case BackColorRole:
    {
        const static auto backColorRole = QVariant::fromValue<QColor>(QColor("transparent"));

        return backColorRole;

        break;
    }

    case TextColorRole:
    {
        const static auto textColorRole = QVariant::fromValue<QColor>(QColor("white"));

        return textColorRole;
        break;
    }

    case HeaderTextRole:
    {
        switch(index.row()) {
        case  0: return QVariant::fromValue<QString>("Identification");
        case  1: return QVariant::fromValue<QString>("TN");
        case  2: return QVariant::fromValue<QString>("IFFCode");
        case  3: return QVariant::fromValue<QString>("CallSign");
        case  4: return QVariant::fromValue<QString>("Type");
        case  5: return QVariant::fromValue<QString>("Master Radar");
        case  6: return QVariant::fromValue<QString>("Identify Method");
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

        break;
    }
    case AircraftColor:
    {
        const int _row = index.row();
        return QVariant::fromValue<QColor>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->aircraftColor());
    }

    case AircraftHoverColor:
    {
        const int _row = index.row();
        return QVariant::fromValue<QColor>(mAircraftInfoListProxy[static_cast<size_t>(_row)]->aircraftHoverColor());
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
    hash[AircraftColor] = "AircraftColor";
    hash[AircraftHoverColor] = "AircraftHoverColor";
    return hash;
}

QString AircraftTableModel::headerText(int column) const
{
    switch(column) {
    case  0: return QStringLiteral("Identification");
    case  1: return QStringLiteral("TN");
    case  2: return QStringLiteral("IFFCode");
    case  3: return QStringLiteral("CallSign");
    case  4: return QStringLiteral("Type");
    case  5: return QStringLiteral("Master Radar");
    case  6: return QStringLiteral("Identif. Method");
    case  7: return QStringLiteral("Time");
    case  8: return QStringLiteral("Pos");
    case  9: return QStringLiteral("Latitude");
    case 10: return QStringLiteral("Longitude");
    case 11: return QStringLiteral("Altitude");
    case 12: return QStringLiteral("Heading");
    case 13: return QStringLiteral("Speed");
    case 14: return QStringLiteral("Detection Systems");
    case 15: return QStringLiteral("Sends");
    }

    return QStringLiteral("");
}

int AircraftTableModel::getTN(int row) const
{
    if (row < 0) return -1;

    if (row >= static_cast<int>(mAircraftInfoListProxy.size())) {
        return -1;
    }

    return mAircraftInfoListProxy[std::size_t(row)]->TN;
}

void AircraftTableModel::setFilterWildcard(const QString &wildcard)
{
    //    if (!mShowAssigned) {
    mMinRowUpdate = -1;
    mMaxRowUpdate = -1;
    mNeedUpdateOnTimerTrigger = false;

    beginResetModel();

    mFilter = wildcard;
    mFilter.remove(QRegularExpression("\\s"));

    mAircraftInfoListProxy.clear();
    for (auto& item : mAircraftInfoList) {
        if (QString::number(item->TN).contains(mFilter))
            mAircraftInfoListProxy.push_back(item);
    }

    endResetModel();
    //    }
}

void AircraftTableModel::sortWithHeader(int column)
{
    switch (column) {
    case 0: std::sort(mAircraftInfoList.begin(), mAircraftInfoList.end(), [=](const QSharedPointer<AircraftInfo> &item1, const QSharedPointer<AircraftInfo> &item2) {
            return item1->identifyToString() < item2->identifyToString();
        });
        break;
    }
    mNeedUpdateOnTimerTrigger = true;
    onUpdateTimerTriggered();


}

void AircraftTableModel::onAircraftClicked(int TN)
{
    emit aircraftClicked(TN);
//    refresh();
}

void AircraftTableModel::onSystemClicked(int Number) {
    mNumber = Number;
    beginResetModel();
    mAircraftInfoListProxy.clear();
    if (mAircraftsAssigned.contains(Number)) {
        for (AircraftAssignInfo aircraft : mAircraftsAssigned[Number]) {
            auto it = std::find_if(mAircraftInfoList.begin(), mAircraftInfoList.end(), [aircraft](QSharedPointer<AircraftInfo> &item){
                    return item->TN == aircraft.TN;
        });
            mAircraftInfoListProxy.push_back(*it);
        }
    }
    endResetModel();

}


void AircraftTableModel::onUpdateTimerTriggered()
{
    if (mNeedUpdateOnTimerTrigger) {
        if (mMode == "TableModel") {
            mAircraftInfoListProxy.clear();
            for (auto& item : mAircraftInfoList) {
                if (QString::number(item->TN).contains(mFilter))
                    mAircraftInfoListProxy.push_back(item);
            }
//            if (mAircraftInfoListProxy.size() > 0)
                emit dataChanged(createIndex(mMinRowUpdate, 0), createIndex(mMaxRowUpdate, columnCount()-1));

        }
        else {
            if (mNumber > 0) {
                onSystemClicked(mNumber);
            }

        }

        mMinRowUpdate = -1;
        mMaxRowUpdate = -1;
        mNeedUpdateOnTimerTrigger = false;
    }
}

void AircraftTableModel::updateItemData(const QString &/*jsonStr*/)
{

}

void AircraftTableModel::updateItemData(const AircraftInfo &aircraftInfo)
{
    //    beginResetModel();

    const auto it = std::find_if(mAircraftInfoList.begin(), mAircraftInfoList.end(),
                                 [aircraftInfo](const QSharedPointer<AircraftInfo>& itemInfo){
        return itemInfo->TN == aircraftInfo.TN;
    });

    if (it != mAircraftInfoList.end()) {
        *(*it) = aircraftInfo;

        int index = static_cast<int>(std::distance(mAircraftInfoList.begin(), it));

        if (mMinRowUpdate == -1) {
            mMinRowUpdate = index;
        } else {
            mMinRowUpdate = qMin(index, mMinRowUpdate);

        }

        if (mMaxRowUpdate == -1) {
            mMaxRowUpdate = index;
        } else {
            mMaxRowUpdate = qMax(index, mMaxRowUpdate);
        }
        mNeedUpdateOnTimerTrigger = true;
        onUpdateTimerTriggered();

    }
    else {
        QSharedPointer<AircraftInfo> isp;
        isp.reset(new AircraftInfo);
        *(isp) = aircraftInfo;
        mAircraftInfoList.push_back(isp);

        mMinRowUpdate = -1;
        mMaxRowUpdate = -1;
        mNeedUpdateOnTimerTrigger = false;


        beginInsertRows(QModelIndex(), mAircraftInfoList.size() - 1, mAircraftInfoList.size() - 1);

        mAircraftInfoListProxy.clear();
        for (auto& item : mAircraftInfoList) {
            if (QString::number(item->TN).contains(mFilter))
                mAircraftInfoListProxy.push_back(item);
        }

        endInsertRows();
    }
    //    endResetModel();
}

void AircraftTableModel::deleteItem(int TN)
{
    mMinRowUpdate = -1;
    mMaxRowUpdate = -1;
    mNeedUpdateOnTimerTrigger = false;

    beginResetModel();

    const  auto newEnd = std::remove_if(mAircraftInfoList.begin(),
                                        mAircraftInfoList.end(),
                                        [&TN](const QSharedPointer<AircraftInfo>& itemInfo) {

        return itemInfo->TN == TN;
    });

    mAircraftInfoList.erase(newEnd, mAircraftInfoList.end());

    //    const auto it = std::find_if(mAircraftInfoList.begin(), mAircraftInfoList.end(),
    //                                 [aircraftInfo](const QPair<int, QSharedPointer<AircraftInfo>>& itemInfo){
    //        return itemInfo.second->TN == aircraftInfo.TN;
    //    });


    //    if (it != mAircraftInfoList.end()) {
    //        *(*it).second = aircraftInfo;
    //    } else {
    //        QPair<int, QSharedPointer<AircraftInfo>> isp;
    //        isp.first = mAircraftInfoList.size();
    //        isp.second.reset(new AircraftInfo);
    //        *(isp.second) = aircraftInfo;
    //        mAircraftInfoList.push_back(isp);
    //    }

    mAircraftInfoListProxy.clear();
    for (auto& item : mAircraftInfoList) {
        if (QString::number(item->TN).contains(mFilter))
            mAircraftInfoListProxy.push_back(item);
    }

    endResetModel();
}

void AircraftTableModel::assign(int TN, int Number)
{
    AircraftAssignInfo tmp;
    tmp.TN = TN;
    if (mAircraftsAssigned.contains(Number)) {
        mAircraftsAssigned[Number].push_back(tmp);
    }
    else {
        mAircraftsAssigned[Number] = QList<AircraftAssignInfo> {tmp};
    }
    if (mMode == "Assignment") {
        if (mNumber > 0)
            onSystemClicked(mNumber);
    }
}

void AircraftTableModel::cancelAssign(int TN, int Number)
{
    if (mAircraftsAssigned.contains(Number)) {
        const auto it = std::remove_if(mAircraftsAssigned[Number].begin(), mAircraftsAssigned[Number].end(), [TN](AircraftAssignInfo &aircraft){
                return aircraft.TN == TN;
    });
        mAircraftsAssigned[Number].erase(it, mAircraftsAssigned[Number].end());
    }
    if (mMode == "Assignment") {
        if (mNumber > 0)
            onSystemClicked(mNumber);
    }
}

void AircraftTableModel::cancelAllAssigns()
{
    mAircraftsAssigned.clear();
    refresh();
}

void AircraftTableModel::cancelAircraftsAssigned(int ExceptTN, int Number)
{
    if (mAircraftsAssigned.contains(Number)){
        for (auto &aircraft : mAircraftsAssigned[Number]){
            if (aircraft.TN != ExceptTN) {
                cancelAssign(aircraft.TN, Number);
            }
        }
    }
    if (mMode == "Assignment") {
        if (mNumber > 0)
            onSystemClicked(mNumber);
    }

}

void AircraftTableModel::acceptAssign(int TN, int Number, bool result)
{
    if (result) {
        auto it = std::find_if(mAircraftsAssigned[Number].begin(), mAircraftsAssigned[Number].end(), [TN] (AircraftAssignInfo &item) {
                return item.TN == TN;
    });
        if (it != mAircraftsAssigned[Number].end())
            it->assign = true;
    }
    else {
        cancelAssign(TN, Number);
    }

}


void AircraftTableModel::refresh()
{
    beginResetModel();
    mNumber = -1;
    mAircraftInfoListProxy.clear();
    mAircraftInfoListProxy.assign(mAircraftInfoList.begin(), mAircraftInfoList.end());
    endResetModel();
}

void AircraftTableModel::clearList()
{
    mMinRowUpdate = -1;
    mMaxRowUpdate = -1;
    mNeedUpdateOnTimerTrigger = false;

    beginResetModel();
    mAircraftInfoList.clear();
    mAircraftInfoListProxy.clear();
    cancelAllAssigns();
    endResetModel();
}

void AircraftTableModel::setMode(QString mode)
{
    mMode = mode;
}

QMap<int, QList<AircraftAssignInfo>> AircraftTableModel::getAssignmentMap()
{
    return mAircraftsAssigned;
}

