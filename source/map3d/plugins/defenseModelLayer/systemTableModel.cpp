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
    return 21;
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
        case  3: return mMode == "Assignment" ? QVariant::fromValue<double>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row())]->TN):
                                                QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )]->Terminal);
        case  4: return mMode == "Assignment" ? QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->phaseToString()):
                                                QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )]->Latitude);
        case  5: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )]->Longitude);
        case  6: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )]->Altitude);
        case  7: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row()    )]->ViewRange);
        case  8: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row())]->MezRange);
        case  9: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row())]->ReceiveTime);
        case 10: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )]->Simulation);
        case 11: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )]->radarStatusToString(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )]->RadarSearchStatus));
        case 12: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )]->Operational);
        case 13: return QVariant::fromValue<int>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )]->MissileCount);
        case 14: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row()    )]->RadarMode);
        case 15: return QVariant::fromValue<double>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row())]->TN);
        case 16: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row())]->Acceptance);
        case 17: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->phaseToString());
        case 18: return QVariant::fromValue<double>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->Antenna);
        case 19: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->ChanelNo);
        case 20: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )]->Inrange);

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
        case  0: return QVariant::fromValue<QString>("Number");
        case  1: return QVariant::fromValue<QString>("Name");
        case  2: return QVariant::fromValue<QString>("Type");
        case  3: return QVariant::fromValue<QString>("Terminal");
        case  4: return QVariant::fromValue<QString>("Latitude");
        case  5: return QVariant::fromValue<QString>("Longitude");
        case  6: return QVariant::fromValue<QString>("Altitude");
        case  7: return QVariant::fromValue<QString>("ViewRange");
        case  8: return QVariant::fromValue<QString>("MezRange");
        case  9: return QVariant::fromValue<QString>("ReceiveTime");
        case 10: return QVariant::fromValue<QString>("Simulation");
        case 11: return QVariant::fromValue<QString>("RadarSearchStatus");
        case 12: return QVariant::fromValue<QString>("Operational");
        case 13: return QVariant::fromValue<QString>("MissileCount");
        case 14: return QVariant::fromValue<QString>("RadarMode");
        case 15: return QVariant::fromValue<QString>("TN");
        case 16: return QVariant::fromValue<QString>("Acceptance");
        case 17: return QVariant::fromValue<QString>("Phase");
        case 18: return QVariant::fromValue<QString>("Antenna");
        case 19: return QVariant::fromValue<QString>("ChanelNo");
        case 20: return QVariant::fromValue<QString>("Inrange");

        }
        break;
    }


    case SystemColor:
    {
        if (index.row() >= mSystemCombatInfoListProxy.size())
            return QVariant::fromValue<QColor>("white");
        return QVariant::fromValue<QColor>(QColor(mSystemCombatInfoListProxy[static_cast<size_t>(index.row())]->phaseToColor()));
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
    case  3: return mMode == "Assignment" ? QStringLiteral("TN") : QStringLiteral("Terminal");
    case  4: return mMode == "Assignment" ? QStringLiteral("Phase") : QStringLiteral("Latitude");
    case  5: return QStringLiteral("Longitude");
    case  6: return QStringLiteral("Altitude");
    case  7: return QStringLiteral("ViewRange");
    case  8: return QStringLiteral("MezRange");
    case  9: return QStringLiteral("ReceiveTime");
    case 10: return QStringLiteral("Simulation");
    case 11: return QStringLiteral("RadarSearchStatus");
    case 12: return QStringLiteral("Operational");
    case 13: return QStringLiteral("MissileCount");
    case 14: return QStringLiteral("RadarMode");
    case 15: return QStringLiteral("TN");
    case 16: return QStringLiteral("Acceptance");
    case 17: return QStringLiteral("Phase");
    case 18: return QStringLiteral("Antenna");
    case 19: return QStringLiteral("ChanelNo");
    case 20: return QStringLiteral("Inrange");
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
    if (mSystemsAssigned.contains(TN)) {
        for (SystemAssignInfo system : mSystemsAssigned[TN]) {
            auto it = std::find_if(mSystemInfoList.begin(), mSystemInfoList.end(), [system](QSharedPointer<SystemInfo> &item){
                return item->Number == system.Number;
            });
            mSystemInfoListProxy.push_back(*it);

            auto it2 = std::find_if(mSystemCombatInfoList.begin(), mSystemCombatInfoList.end(), [system](QSharedPointer<SystemCambatInfo> &item){
                return item->Number == system.Number;
            });
            mSystemCombatInfoListProxy.push_back(*it2);
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

    const auto it = std::find_if(mSystemInfoList.begin(), mSystemInfoList.end(),
                                 [systemInfo](const QSharedPointer<SystemInfo>& itemInfo){
        return itemInfo->Number == systemInfo.Number;
    });


    if (it != mSystemInfoList.end()) {
        *(*it) = systemInfo;
    } else {
        QSharedPointer<SystemInfo> isp;
        isp.reset(new SystemInfo);
        *(isp) = systemInfo;
        mSystemInfoList.push_back(isp);

        //------------------ add combatinfo if doesn't exist
        const auto it2 = std::find_if(mSystemCombatInfoList.begin(), mSystemCombatInfoList.end(), [systemInfo](QSharedPointer<SystemCambatInfo> &item){
            return item->Number == systemInfo.Number;
        });
        const auto it3 = std::find_if(mSystemStatusInfoList.begin(), mSystemStatusInfoList.end(), [systemInfo](QSharedPointer<SystemStatusInfo> &item){
            return item->Number == systemInfo.Number;
        });
        if (it2 == mSystemCombatInfoList.end()) {
            SystemCambatInfo tmp1;
            tmp1.Number = systemInfo.Number;
            tmp1.Phase = SystemCambatInfo::Search;
            updateItemData(tmp1);
        }
        if (it3 == mSystemStatusInfoList.end()) {
            SystemStatusInfo tmp2;
            tmp2.Number = systemInfo.Number;
            updateItemData(tmp2);
        }
    }
    mSystemInfoListProxy.clear();
    for (auto& item : mSystemInfoList) {
        if (QString::number(item->Number).contains(mFilter))
            mSystemInfoListProxy.push_back(item);
    }
    emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount()-1));

//    endResetModel();
}

void SystemTableModel::updateItemData(const SystemStatusInfo &systemStatusInfo)
{
//    beginResetModel();

    bool update = false;
    for (auto system : mSystemInfoList) {
        if (system->Number == systemStatusInfo.Number)
            update = true;
    }
    if (update) {
        const auto it = std::find_if(mSystemStatusInfoList.begin(), mSystemStatusInfoList.end(),
                                     [systemStatusInfo](const QSharedPointer<SystemStatusInfo>& itemInfo){
            return itemInfo->Number == systemStatusInfo.Number;
        });


        if (it != mSystemStatusInfoList.end()) {
            *(*it) = systemStatusInfo;
        } else {
            QSharedPointer<SystemStatusInfo> isp;
            isp.reset(new SystemStatusInfo);
            *(isp) = systemStatusInfo;
            mSystemStatusInfoList.push_back(isp);
        }

        if (mMode == "TableModel") {
            mSystemStatusInfoListProxy.clear();
            for (auto& item : mSystemStatusInfoList) {
                if (QString::number(item->Number).contains(mFilter))
                    mSystemStatusInfoListProxy.push_back(item);
            }
            emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, columnCount()-1));
        }
        else {
            if (mTN > 0)
                onAircraftClicked(mTN);
        }
    }

//    endResetModel();
}

void SystemTableModel::updateItemData(const SystemCambatInfo &systemCambatInfo)
{
    bool update = false;
    for (auto system : mSystemInfoList) {
        if (system->Number == systemCambatInfo.Number)
            update = true;
    }
    if (update) {
        beginResetModel();

        const auto it = std::find_if(mSystemCombatInfoList.begin(), mSystemCombatInfoList.end(),
                                     [systemCambatInfo](const QSharedPointer<SystemCambatInfo>& itemInfo){
            return itemInfo->Number == systemCambatInfo.Number;
        });


        if (it != mSystemCombatInfoList.end()) {
            *(*it) = systemCambatInfo;
        } else {
            QSharedPointer<SystemCambatInfo> isp;
            isp.reset(new SystemCambatInfo);
            *(isp) = systemCambatInfo;
            mSystemCombatInfoList.push_back(isp);
        }
        mSystemCombatInfoListProxy.clear();
        for (auto& item : mSystemCombatInfoList) {
            if (QString::number(item->Number).contains(mFilter))
                mSystemCombatInfoListProxy.push_back(item);
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

