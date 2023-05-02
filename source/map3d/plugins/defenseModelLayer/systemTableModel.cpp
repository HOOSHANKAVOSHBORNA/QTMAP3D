#include "systemTableModel.h"

#include <QDebug>
#include <QColor>
#include <QRegularExpression>

SystemTableModel::SystemTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

int SystemTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 22;
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
        case  0: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row())]->Number);
        case  1: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row())]->Name);
        case  2: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row())]->Type);
        case  3: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )]->Terminal);
        case  4: return mMode == "Assignment" ? QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->phaseToString()):
                                                QVariant::fromValue<double>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->TN);
        case  5: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )]->Latitude);
        case  6: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )]->Longitude);
        case  7: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )]->Altitude);
        case  8: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row()    )]->ViewRange);
        case  9: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row())]->MezRange);
        case 10: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row())]->ReceiveTime);
        case 11: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )]->Simulation);
        case 12: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )]->BCCStatusToString());
        case 13: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )]->radarSearchStatusToString());
        case 14: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )]->operationalToString());
        case 15: return QVariant::fromValue<int>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )]->MissileCount);
        case 16: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row()    )]->RadarMode);
        case 17: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row())]->Acceptance);
        case 18: return mMode == "Assignment" ? QVariant::fromValue<double>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->TN):
                                                QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->phaseToString());
        case 19: return QVariant::fromValue<double>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->Antenna);
        case 20: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->ChanelNo);
        case 21: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->Inrange);

        }
        break;
    }
    case BackColorRole:
    {
        return QVariant::fromValue<QColor>(QColor("transparent"));

    }

    case TextColorRole:
    {
        if (mMode == "Assignment") {
            return index.column() == 4 ? QVariant::fromValue<QColor>(QColor("black")) : QVariant::fromValue<QColor>(QColor("white"));
        }
        return QVariant::fromValue<QColor>(QColor("white"));
    }


    case SystemColor:
    {
        if (static_cast<size_t>(index.row()) >= mSystemCombatInfoListProxy.size())
            return QVariant::fromValue<QColor>("white");
        if (mMode == "Assignment") {
            switch (index.column()) {
                case 4: return QVariant::fromValue<QColor>(QColor(mSystemCombatInfoListProxy[static_cast<size_t>(index.row())]->phaseToColor()));
                default : return QVariant::fromValue<QColor>(QColor("transparent"));

            }
        }
        return QVariant::fromValue<QColor>(QColor(mSystemCombatInfoListProxy[static_cast<size_t>(index.row())]->phaseToColor()));
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
    hash[AssignColor] = "AssignColor";
    hash[SystemColor] = "SystemColor";
    return hash;
}

QString SystemTableModel::headerText(int column) const
{
    switch (column) {
    case  0: return QStringLiteral("Number");
    case  1: return QStringLiteral("Name");
    case  2: return QStringLiteral("Type");
    case  3: return QStringLiteral("Terminal");
    case  4: return mMode == "Assignment" ? QStringLiteral("Phase") : QStringLiteral("TN");
    case  5: return QStringLiteral("Latitude");
    case  6: return QStringLiteral("Longitude");
    case  7: return QStringLiteral("Altitude");
    case  8: return QStringLiteral("ViewRange");
    case  9: return QStringLiteral("MezRange");
    case 10: return QStringLiteral("ReceiveTime");
    case 11: return QStringLiteral("Simulation");
    case 12: return QStringLiteral("BCCStatus");
    case 13: return QStringLiteral("RadarSearchStatus");
    case 14: return QStringLiteral("Operational");
    case 15: return QStringLiteral("MissileCount");
    case 16: return QStringLiteral("RadarMode");
    case 17: return QStringLiteral("Acceptance");
    case 18: return mMode == "Assignment" ? QStringLiteral("TN") : QStringLiteral("Phase");
    case 19: return QStringLiteral("Antenna");
    case 20: return QStringLiteral("ChanelNo");
    case 21: return QStringLiteral("Inrange");
    default: QStringLiteral("");
    }
}

int SystemTableModel::getNumber(int row) const
{
    if (row < 0) return -1;

    if (row >= static_cast<int>(mSystemInfoListProxy.size())) {
        return -1;
    }

    return mSystemInfoListProxy[std::size_t(row)]->Number;
}

void SystemTableModel::setFilterWildcard(const QString &wildcard)
{
        beginResetModel();

        mFilter = wildcard;
        mFilter.remove(QRegularExpression("\\s"));

        mSystemInfoListProxy.clear();
        for (auto& item : mSystemInfoList) {
            if (QString::number(item->Number).contains(mFilter))
                mSystemInfoListProxy.push_back(item);
        }

        endResetModel();
}

void SystemTableModel::onAircraftClicked(int TN)
{
    mTN = TN;
    beginResetModel();
    mSystemInfoListProxy.clear();
    mSystemCombatInfoListProxy.clear();
    mSystemStatusInfoListProxy.clear();
    if (mSystemsAssigned.contains(TN)) {
        for (SystemAssignInfo system : mSystemsAssigned[TN]) {
            auto it = std::find_if(mSystemInfoList.begin(), mSystemInfoList.end(), [system](QSharedPointer<SystemInfo> &item){
                return item->Number == system.Number;
            });
            mSystemInfoListProxy.push_back(*it);

            auto it2 = std::find_if(mSystemCombatInfoList.begin(), mSystemCombatInfoList.end(), [system](QSharedPointer<SystemCombatInfo> &item){
                return item->Number == system.Number;
            });
            mSystemCombatInfoListProxy.push_back(*it2);

            auto it3 = std::find_if(mSystemStatusInfoList.begin(), mSystemStatusInfoList.end(), [system](QSharedPointer<SystemStatusInfo> &item){
                return item->Number == system.Number;
            });
            mSystemStatusInfoListProxy.push_back(*it3);
        }
    }
    endResetModel();
}

void SystemTableModel::onSystemClicked(int Number)
{
    emit systemClicked(Number);
//    refresh();
}

void SystemTableModel::updateItemData(const SystemInfo &systemInfo)
{
//    beginResetModel();

//    const auto it = std::find_if(mSystemInfoList.begin(), mSystemInfoList.end(),
//                                 [systemInfo](const QSharedPointer<SystemInfo>& itemInfo){
//        return itemInfo->Number == systemInfo.Number;
//    });


//    if (it != mSystemInfoList.end()) {
//        *(*it) = systemInfo;
//    } else {
//        QSharedPointer<SystemInfo> isp;
//        isp.reset(new SystemInfo);
//        *(isp) = systemInfo;
//        mSystemInfoList.push_back(isp);

//        //------------------ add combatinfo if doesn't exist
//        const auto it2 = std::find_if(mSystemCombatInfoList.begin(), mSystemCombatInfoList.end(), [systemInfo](QSharedPointer<SystemCombatInfo> &item){
//            return item->Number == systemInfo.Number;
//        });
//        const auto it3 = std::find_if(mSystemStatusInfoList.begin(), mSystemStatusInfoList.end(), [systemInfo](QSharedPointer<SystemStatusInfo> &item){
//            return item->Number == systemInfo.Number;
//        });
//        if (it2 == mSystemCombatInfoList.end()) {
//            SystemCombatInfo tmp1;
//            tmp1.Number = systemInfo.Number;
//            tmp1.Phase = SystemCombatInfo::Search;
//            updateItemData(tmp1);
//        }
//        if (it3 == mSystemStatusInfoList.end()) {
//            SystemStatusInfo tmp2;
//            tmp2.Number = systemInfo.Number;
//            updateItemData(tmp2);
//        }
//    }
//    if (mMode == "TableModel" || mTN == -1) {
//        mSystemInfoListProxy.clear();
//        for (auto& item : mSystemInfoList) {
//            if (QString::number(item->Number).contains(mFilter))
//                mSystemInfoListProxy.push_back(item);
//        }
//    }
//    else {
//        if (mTN > 0)
//            onAircraftClicked(mTN);
//    }
//    emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount()-1));

//    endResetModel();
}

void SystemTableModel::updateItemData(const SystemStatusInfo &systemStatusInfo)
{
//    beginResetModel();

//    bool update = false;
//    for (auto system : mSystemInfoList) {
//        if (system->Number == systemStatusInfo.Number)
//            update = true;
//    }
//    if (update) {
//        const auto it = std::find_if(mSystemStatusInfoList.begin(), mSystemStatusInfoList.end(),
//                                     [systemStatusInfo](const QSharedPointer<SystemStatusInfo>& itemInfo){
//            return itemInfo->Number == systemStatusInfo.Number;
//        });


//        if (it != mSystemStatusInfoList.end()) {
//            *(*it) = systemStatusInfo;
//        } else {
//            QSharedPointer<SystemStatusInfo> isp;
//            isp.reset(new SystemStatusInfo);
//            *(isp) = systemStatusInfo;
//            mSystemStatusInfoList.push_back(isp);
//        }

//        if (mMode == "TableModel" || mTN == -1) {
//            mSystemStatusInfoListProxy.clear();
//            for (auto& item : mSystemStatusInfoList) {
//                if (QString::number(item->Number).contains(mFilter))
//                    mSystemStatusInfoListProxy.push_back(item);
//            }
//            emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount()-1));
//        }
//        else {
//            if (mTN > 0)
//                onAircraftClicked(mTN);
//        }
//    }

//    endResetModel();
}

void SystemTableModel::updateItemData(const SystemCombatInfo &systemCombatInfo)
{
    bool update = false;
    for (auto system : mSystemInfoList) {
        if (system->Number == systemCombatInfo.Number)
            update = true;
    }
    if (update) {
        beginResetModel();

        const auto it = std::find_if(mSystemCombatInfoList.begin(), mSystemCombatInfoList.end(),
                                     [systemCombatInfo](const QSharedPointer<SystemCombatInfo>& itemInfo){
            return itemInfo->Number == systemCombatInfo.Number;
        });


        if (it != mSystemCombatInfoList.end()) {
            *(*it) = systemCombatInfo;
        } else {
            QSharedPointer<SystemCombatInfo> isp;
            isp.reset(new SystemCombatInfo);
            *(isp) = systemCombatInfo;
            mSystemCombatInfoList.push_back(isp);
        }
        if (mMode == "TableModel" || mTN == -1) {
            mSystemCombatInfoListProxy.clear();
            for (auto& item : mSystemCombatInfoList) {
                if (QString::number(item->Number).contains(mFilter))
                    mSystemCombatInfoListProxy.push_back(item);
            }
        }
        else {
            if (mTN > 0)
                onAircraftClicked(mTN);
        }

        endResetModel();
    }
}

void SystemTableModel::assign(int Number, int TN)
{
    SystemAssignInfo tmp;
    tmp.Number = Number;
    tmp.Phase = "";
    if (mSystemsAssigned.contains(TN)) {
        mSystemsAssigned[TN].push_back(tmp);
    }
    else {
        mSystemsAssigned[TN] = QList<SystemAssignInfo> {tmp};
    }
    if (mMode == "Assignment") {
        if (mTN > 0)
            onAircraftClicked(TN);
    }

}

void SystemTableModel::cancelSystemsAssigned(int TN, int ExceptNum)
{

    if (mSystemsAssigned.contains(TN)){
        for (auto &system : mSystemsAssigned[TN]){
            if (system.Number != ExceptNum) {
                cancelAssign(system.Number, ExceptNum);
            }
        }
    }
    if (mMode == "Assignment") {
        if (mTN > 0)
            onAircraftClicked(TN);
    }
}

void SystemTableModel::cancelAllAssigns()
{
    mSystemsAssigned.clear();
    refresh();
}

void SystemTableModel::cancelAssign(int TN, int Number)
{
    if (mSystemsAssigned.contains(TN)) {
        auto it = std::remove_if(mSystemsAssigned[TN].begin(), mSystemsAssigned[TN].end(), [Number](SystemAssignInfo &system){
            return system.Number == Number;
    });
        if (it != mSystemsAssigned[TN].end())
            mSystemsAssigned[TN].erase(it);
    }
    if (mMode == "Assignment"){
        if (mTN > 0) {
            onAircraftClicked(mTN);
        }
    }
}

void SystemTableModel::acceptAssign(int TN, int Number, bool result)
{
    if (result) {
        auto it = std::find_if(mSystemsAssigned[TN].begin(), mSystemsAssigned[TN].end(), [Number](SystemAssignInfo &item) {
            return Number == item.Number;
        });
        if (it != mSystemsAssigned[TN].end())
            it->assign = true;
    }
    else {
        cancelAssign(TN, Number);
    }
}

void SystemTableModel::refresh()
{
    beginResetModel();
    mTN = -1;
    mSystemInfoListProxy.clear();
    mSystemCombatInfoListProxy.clear();
    mSystemStatusInfoListProxy.clear();
    mSystemInfoListProxy.assign(mSystemInfoList.begin(), mSystemInfoList.end());
    mSystemCombatInfoListProxy.assign(mSystemCombatInfoList.begin(), mSystemCombatInfoList.end());
    mSystemStatusInfoListProxy.assign(mSystemStatusInfoList.begin(), mSystemStatusInfoList.end());

    endResetModel();
}

void SystemTableModel::clearList()
{
    beginResetModel();
    mSystemInfoList.clear();
    mSystemInfoListProxy.clear();
    mSystemCombatInfoList.clear();
    mSystemCombatInfoListProxy.clear();
    mSystemStatusInfoList.clear();
    mSystemStatusInfoListProxy.clear();
    cancelAllAssigns();
    endResetModel();
}

void SystemTableModel::setMode(QString mode)
{
    mMode = mode;
}

QMap<int, QList<SystemAssignInfo> > SystemTableModel::getAssignmentMap()
{
    return mSystemsAssigned;
}

