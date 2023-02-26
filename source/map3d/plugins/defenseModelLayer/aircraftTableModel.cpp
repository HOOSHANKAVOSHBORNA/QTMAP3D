#include "aircraftTableModel.h"
#include <QHash>
#include <QColor>
#include <QRegularExpression>
#include <QDebug>
#include <QTimer>

AircraftTableModel::AircraftTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    //    for (int i = 0; i < 100; i++) {
    //        AircraftInfo ai;
    //        ai.TN = QStringLiteral("OOPS") + QString::number(i);
    //        updateItemData(ai);
    //    }

    QTimer *timer = new QTimer;
    QObject::connect(timer , &QTimer::timeout,
                     this, &AircraftTableModel::onUpdateTimerTriggered,
                     Qt::DirectConnection);
    timer->start(500);
}

int AircraftTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 17;
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

        switch(index.column()) {
        case  0: return QVariant::fromValue<QString>(QString::number(mAircraftInfoListProxy[static_cast<size_t>(_row)].first));
        case  1: return QVariant::fromValue<int>(mAircraftInfoListProxy[static_cast<size_t>(_row)].second->TN);
        case  2: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)].second->IFFCode);
        case  3: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)].second->CallSign);
        case  4: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)].second->Type);
        case  5: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)].second->MasterRadar);
        case  6: return QVariant::fromValue<QString>( mAircraftInfoListProxy[static_cast<size_t>(_row)].second->identifyToString());
        case  7: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)].second->IdentificationMethod);
        case  8: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)].second->Time);
        case  9: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)].second->Pos);
        case 10: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(_row )].second->Latitude);
        case 11: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(_row )].second->Longitude);
        case 12: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(_row )].second->Altitude);
        case 13: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(_row )].second->Heading);
        case 14: return QVariant::fromValue<double>(mAircraftInfoListProxy[static_cast<size_t>(_row )].second->Speed);
        case 15: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)].second->detectionSystemsToString());
        case 16: return QVariant::fromValue<QString>(mAircraftInfoListProxy[static_cast<size_t>(_row)].second->sendsToString());
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
    case AircraftColor:
    {
        const int _row = index.row();
        if (mAircraftInfoListProxy[static_cast<size_t>(_row)].second->identifyToString() == "F")
            return QVariant::fromValue<QColor>(QColor("green"));
        else if (mAircraftInfoListProxy[static_cast<size_t>(_row)].second->identifyToString() == "K")
            return QVariant::fromValue<QColor>(QColor("yellow"));
        else if (mAircraftInfoListProxy[static_cast<size_t>(_row)].second->identifyToString() == "Z")
            return QVariant::fromValue<QColor>(QColor("orange"));
        else if (mAircraftInfoListProxy[static_cast<size_t>(_row)].second->identifyToString() == "X")
            return QVariant::fromValue<QColor>(QColor("red"));
        else if (mAircraftInfoListProxy[static_cast<size_t>(_row)].second->identifyToString() == "U")
            return QVariant::fromValue<QColor>(QColor("white"));
        else
            return QVariant::fromValue<QColor>(QColor("red"));

        break;
    }

    case AssignColor:
    {
        const int _row = index.row();
        if (mAircraftsAssigned[mNumber][_row].Phase == "Search")
            return QVariant::fromValue<QColor>(QColor("yellow"));
        else if (mAircraftsAssigned[mNumber][_row].Phase == "Lock")
            return QVariant::fromValue<QColor>(QColor("orange"));
        else if (mAircraftsAssigned[mNumber][_row].Phase == "Fire")
            return QVariant::fromValue<QColor>(QColor("red"));
        else if (mAircraftsAssigned[mNumber][_row].Phase == "Kill")
            return QVariant::fromValue<QColor>(QColor("black"));
        else if (mAircraftsAssigned[mNumber][_row].Phase == "NoKill")
            return QVariant::fromValue<QColor>(QColor("brown"));
        else
            return QVariant::fromValue<QColor>(QColor("white"));

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
    hash[AircraftColor] = "AircraftColor";
    hash[AssignColor] = "AssignColor";
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

int AircraftTableModel::getTN(int row) const
{
    if (row < 0) return -1;

    if (row >= static_cast<int>(mAircraftInfoListProxy.size())) {
        return -1;
    }

    return mAircraftInfoListProxy[std::size_t(row)].second->TN;
}

void AircraftTableModel::setFilterWildcard(const QString &wildcard)
{
    if (!mShowAssigned) {
        mMinRowUpdate = -1;
        mMaxRowUpdate = -1;
        mNeedUpdateOnTimerTrigger = false;

        beginResetModel();

        mFilter = wildcard;
        mFilter.remove(QRegularExpression("\\s"));

        mAircraftInfoListProxy.clear();
        for (auto& item : mAircraftInfoList) {
            if (QString::number(item.second->TN).contains(mFilter))
                mAircraftInfoListProxy.push_back(item);
        }

        endResetModel();
    }
}

void AircraftTableModel::onAircraftClicked(int TN)
{
    emit aircraftClicked(TN);
}

void AircraftTableModel::onSystemClicked(int Number) {
    mNumber = Number;
    mShowAssigned = true;
    beginResetModel();
    mAircraftInfoListProxy.clear();
    if (mAircraftsAssigned.contains(Number)) {
        for (AircraftAssignInfo aircraft : mAircraftsAssigned[Number]) {
            auto it = std::find_if(mAircraftInfoList.begin(), mAircraftInfoList.end(), [aircraft](QPair<int, QSharedPointer<AircraftInfo>> &item){
                return item.second->TN == aircraft.TN;
            });
//            (*it).first = mAircraftInfoListProxy.size();
            mAircraftInfoListProxy.push_back(*it);

        }
    }
    endResetModel();
}


void AircraftTableModel::onUpdateTimerTriggered()
{

    if (mIndex != 3)
        mFilterProxy = mFilter;
    if (mNeedUpdateOnTimerTrigger) {
        if (!mShowAssigned) {
            mAircraftInfoListProxy.clear();
            for (auto& item : mAircraftInfoList) {
                if (QString::number(item.second->TN).contains(mFilterProxy))
                    mAircraftInfoListProxy.push_back(item);
            }
        }
        else {
//            for (int i = 0; i < mAircraftInfoList.size(); i++) {
                beginResetModel();
                mAircraftInfoListProxy.clear();
                if (mAircraftsAssigned.contains(mNumber)) {
                    for (AircraftAssignInfo aircraft : mAircraftsAssigned[mNumber]) {
                        auto it = std::find_if(mAircraftInfoList.begin(), mAircraftInfoList.end(), [aircraft](QPair<int, QSharedPointer<AircraftInfo>> &item){
                            return item.second->TN == aircraft.TN;
                        });
//                        (*it).first = mAircraftInfoListProxy.size();
                        mAircraftInfoListProxy.push_back(*it);

                    }
                }
                endResetModel();
//            }
        }

        if (mAircraftInfoListProxy.size() > 0)
            emit dataChanged(createIndex(mMinRowUpdate, 0), createIndex(mMaxRowUpdate, columnCount()-1));

        mMinRowUpdate = -1;
        mMaxRowUpdate = -1;
        mNeedUpdateOnTimerTrigger = false;
    }
}

bool AircraftTableModel::getShowAssigned()
{
    return mShowAssigned;
}

void AircraftTableModel::updateItemData(const QString &/*jsonStr*/)
{

}

void AircraftTableModel::updateItemData(const AircraftInfo &aircraftInfo)
{
//    beginResetModel();

    const auto it = std::find_if(mAircraftInfoList.begin(), mAircraftInfoList.end(),
                                 [aircraftInfo](const QPair<int, QSharedPointer<AircraftInfo>>& itemInfo){
        return itemInfo.second->TN == aircraftInfo.TN;
    });


    if (it != mAircraftInfoList.end()) {
        *(*it).second = aircraftInfo;

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


    } else {


        QPair<int, QSharedPointer<AircraftInfo>> isp;
        isp.first = mAircraftInfoList.size();
        isp.second.reset(new AircraftInfo);
        *(isp.second) = aircraftInfo;
        mAircraftInfoList.push_back(isp);

        mMinRowUpdate = -1;
        mMaxRowUpdate = -1;
        mNeedUpdateOnTimerTrigger = false;


        beginInsertRows(QModelIndex(), mAircraftInfoList.size() - 1, mAircraftInfoList.size() - 1);

        mAircraftInfoListProxy.clear();
        for (auto& item : mAircraftInfoList) {
            if (QString::number(item.second->TN).contains(mFilter))
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
                                        [&TN](const QPair<int, QSharedPointer<AircraftInfo>>& itemInfo) {

        return itemInfo.second->TN == TN;
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
        if (QString::number(item.second->TN).contains(mFilter))
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
    if (mNumber == Number) {
        beginResetModel();
        onSystemClicked(Number);
        endResetModel();
    }
}

void AircraftTableModel::cancelAssign(int TN, int Number)
{
    if (TN == -1){
        if (mAircraftsAssigned.contains(Number)){
            mAircraftsAssigned.remove(Number);
        }
    }
    else if (Number == -1) {
        for (auto &i : mAircraftsAssigned) {
            auto toDelete = std::remove_if(i.begin(), i.end(), [TN](AircraftAssignInfo &aircraft){
                return TN == aircraft.TN;
            });

            i.erase(toDelete, i.end());
        }
    }
    else {

        auto toDelete = std::remove_if(mAircraftsAssigned[Number].begin(), mAircraftsAssigned[Number].end(), [TN](AircraftAssignInfo &aircraft){
            return TN == aircraft.TN;
        });
        mAircraftsAssigned[Number].erase(toDelete, mAircraftsAssigned[Number].end());
    }
    if (mShowAssigned) {
//        refresh(mIndex);
        mNeedUpdateOnTimerTrigger = true;
        onUpdateTimerTriggered();
    }

}

void AircraftTableModel::accept(int TN, int Number, bool result)
{
    if (result) {
        auto it = std::find_if(mAircraftsAssigned[Number].begin(), mAircraftsAssigned[Number].end(), [TN] (AircraftAssignInfo &item) {
            return item.TN == TN;
        });
        it->assign = true;
    }
    else {
        cancelAssign(TN, Number);
    }
}


void AircraftTableModel::refresh(int indx)
{
    mShowAssigned = false;
    mNumber = -1;
    beginResetModel();
    mIndex = indx;
    if (indx == 3) {
        mFilterProxy = "";
        mNeedUpdateOnTimerTrigger = true;
        onUpdateTimerTriggered();
    }
    else {
        setFilterWildcard(mFilter);
    }
//    mAircraftInfoListProxy.assign(mAircraftInfoList.begin(), mAircraftInfoList.end());
    endResetModel();
}

void AircraftTableModel::clear()
{
    mMinRowUpdate = -1;
    mMaxRowUpdate = -1;
    mNeedUpdateOnTimerTrigger = false;

    beginResetModel();
    mAircraftInfoList.clear();
    mAircraftInfoListProxy.clear();
    endResetModel();
}

