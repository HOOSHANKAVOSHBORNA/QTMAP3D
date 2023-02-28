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
        case  1: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->Number);
        case  2: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->Name);
        case  3: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->Type);
        case  4: return QVariant::fromValue<QString>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Terminal);
        case  5: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Latitude);
        case  6: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Longitude);
        case  7: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row() )].second->Altitude);
        case  8: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row()    )].second->ViewRange);
        case  9: return QVariant::fromValue<double>(mSystemInfoListProxy[static_cast<size_t>(index.row())].second->MezRange);
        case 10: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row())].second->ReceiveTime);
        case 11: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )].second->Simulation);
        case 12: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )].second->radarStatusToString(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )].second->RadarSearchStatus));
        case 13: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )].second->Operational);
        case 14: return QVariant::fromValue<int>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row() )].second->MissileCount);
        case 15: return QVariant::fromValue<QString>(mSystemStatusInfoListProxy[static_cast<size_t>(index.row()    )].second->RadarMode);
        case 16: return QVariant::fromValue<double>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row())].second->TN);
        case 17: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row())].second->Acceptance);
        case 18: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )].second->phaseToString());
        case 19: return QVariant::fromValue<double>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )].second->Antenna);
        case 20: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )].second->ChanelNo);
        case 21: return QVariant::fromValue<QString>(mSystemCombatInfoListProxy[static_cast<size_t>(index.row() )].second->Inrange);

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
        case  1: return QVariant::fromValue<QString>("Number");
        case  2: return QVariant::fromValue<QString>("Name");
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


    case SystemColor:
    {
        const int _row = index.row();
        if (mSystemCombatInfoListProxy[static_cast<size_t>(_row)].second->phaseToString() == "Search")
            return QVariant::fromValue<QColor>(QColor("yellow"));
        else if (mSystemCombatInfoListProxy[static_cast<size_t>(_row)].second->phaseToString() == "Lock")
            return QVariant::fromValue<QColor>(QColor("orange"));
        else if (mSystemCombatInfoListProxy[static_cast<size_t>(_row)].second->phaseToString() == "Fire")
            return QVariant::fromValue<QColor>(QColor("red"));
        else if (mSystemCombatInfoListProxy[static_cast<size_t>(_row)].second->phaseToString() == "Kill")
            return QVariant::fromValue<QColor>(QColor("black"));
        else if (mSystemCombatInfoListProxy[static_cast<size_t>(_row)].second->phaseToString() == "NoKill")
            return QVariant::fromValue<QColor>(QColor("brown"));
        else
            return QVariant::fromValue<QColor>(QColor("white"));
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
    case  0: return QStringLiteral("Index");
    case  1: return QStringLiteral("Number");
    case  2: return QStringLiteral("Name");
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

int SystemTableModel::getNumber(int row) const
{
    if (row < 0) return -1;

    if (row >= static_cast<int>(mSystemInfoListProxy.size())) {
        return -1;
    }

    return mSystemInfoListProxy[std::size_t(row)].second->Number;
}

void SystemTableModel::setFilterWildcard(const QString &wildcard)
{
    if (!mshowAssigned) {
        beginResetModel();

        mFilter = wildcard;
        mFilter.remove(QRegularExpression("\\s"));

        mSystemInfoListProxy.clear();
        for (auto& item : mSystemInfoList) {
            if (QString::number(item.second->Number).contains(mFilter))
                mSystemInfoListProxy.push_back(item);
        }

        endResetModel();
    }
}

void SystemTableModel::onAircraftClicked(int TN)
{
    mTN = TN;
    mshowAssigned = true;
    beginResetModel();
    mSystemInfoListProxy.clear();
    mSystemCombatInfoListProxy.clear();
    if (mSystemsAssigned.contains(TN)) {
        for (SystemAssignInfo system : mSystemsAssigned[TN]) {
            auto it = std::find_if(mSystemInfoList.begin(), mSystemInfoList.end(), [system](QPair<int, QSharedPointer<SystemInfo>> &item){
                return item.second->Number == system.Number;
            });
//            (*it).first = mSystemInfoListProxy.size();
            mSystemInfoListProxy.push_back(*it);

            auto it2 = std::find_if(mSystemCombatInfoList.begin(), mSystemCombatInfoList.end(), [system](QPair<int, QSharedPointer<SystemCambatInfo>> &item){
                return item.second->Number == system.Number;
            });
//            (*it2).first = mSystemCombatInfoListProxy.size();
            mSystemCombatInfoListProxy.push_back(*it2);
        }
    }
    endResetModel();
}

void SystemTableModel::onSystemClicked(int Number)
{
    emit systemClicked(Number);
}

bool SystemTableModel::getShowAssigned()
{
    return mshowAssigned;
}

void SystemTableModel::updateItemData(const SystemInfo &systemInfo)
{
    beginResetModel();

    const auto it = std::find_if(mSystemInfoList.begin(), mSystemInfoList.end(),
                                 [systemInfo](const QPair<int, QSharedPointer<SystemInfo>>& itemInfo){
        return itemInfo.second->Number == systemInfo.Number;
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

    if (!mshowAssigned) {
        mSystemInfoListProxy.clear();
        for (auto& item : mSystemInfoList) {
            if (QString::number(item.second->Number).contains(mFilter))
                mSystemInfoListProxy.push_back(item);
        }
    }

    endResetModel();
}

void SystemTableModel::updateItemData(const SystemStatusInfo &systemStatusInfo)
{
    beginResetModel();

    const auto it = std::find_if(mSystemStatusInfoList.begin(), mSystemStatusInfoList.end(),
                                 [systemStatusInfo](const QPair<int, QSharedPointer<SystemStatusInfo>>& itemInfo){
        return itemInfo.second->Number == systemStatusInfo.Number;
    });


    if (it != mSystemStatusInfoList.end()) {
        *(*it).second = systemStatusInfo;
    } else {
        QPair<int, QSharedPointer<SystemStatusInfo>> isp;
        isp.first = mSystemStatusInfoList.size();
        isp.second.reset(new SystemStatusInfo);
        *(isp.second) = systemStatusInfo;
        mSystemStatusInfoList.push_back(isp);
    }

    mSystemStatusInfoListProxy.clear();
    for (auto& item : mSystemStatusInfoList) {
        if (QString::number(item.second->Number).contains(mFilter))
            mSystemStatusInfoListProxy.push_back(item);
    }


    endResetModel();
}

void SystemTableModel::updateItemData(const SystemCambatInfo &systemCambatInfo)
{
    beginResetModel();

    const auto it = std::find_if(mSystemCombatInfoList.begin(), mSystemCombatInfoList.end(),
                                 [systemCambatInfo](const QPair<int, QSharedPointer<SystemCambatInfo>>& itemInfo){
        return itemInfo.second->Number == systemCambatInfo.Number;
    });


    if (it != mSystemCombatInfoList.end()) {
        *(*it).second = systemCambatInfo;
    } else {
        QPair<int, QSharedPointer<SystemCambatInfo>> isp;
        isp.first = mSystemCombatInfoList.size();
        isp.second.reset(new SystemCambatInfo);
        *(isp.second) = systemCambatInfo;
        mSystemCombatInfoList.push_back(isp);
    }

    if (!mshowAssigned) {
        mSystemCombatInfoListProxy.clear();
        for (auto& item : mSystemCombatInfoList) {
            if (QString::number(item.second->Number).contains(mFilter))
                mSystemCombatInfoListProxy.push_back(item);
        }

    }
    endResetModel();
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
    if (mTN == TN) {
        beginResetModel();
        onAircraftClicked(TN);
        endResetModel();
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
//    if (mSystemsAssigned.contains(TN)) {
//        QList<SystemAssignInfo>::iterator i;
//        i = mSystemsAssigned[TN].begin();
//        while (i != mSystemsAssigned[TN].end()){
//            if (i->Number != ExceptNum)
//                mSystemsAssigned[TN].erase(i);
//            i++;
//        }
//    }
    if (mTN == TN) {
        beginResetModel();
        onAircraftClicked(TN);
        endResetModel();
    }
}

void SystemTableModel::cancelAllAssigns()
{
    mSystemsAssigned.clear();
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
    if (mshowAssigned) {
        onAircraftClicked(mTN);
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

void SystemTableModel::refresh(int indx)
{
    mshowAssigned = false;
    beginResetModel();
    mTN = -1;
    QString tmp = mFilter;
    if (indx == 3)
        tmp = "";
    mSystemInfoListProxy.clear();
    mSystemCombatInfoListProxy.clear();


    for (auto& item : mSystemCombatInfoList) {
        if (QString::number(item.second->Number).contains(tmp))
            mSystemCombatInfoListProxy.push_back(item);
    }
    for (auto& item : mSystemInfoList){
        if (QString::number(item.second->Number).contains(tmp))
            mSystemInfoListProxy.push_back(item);
    }

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

QMap<int, QList<SystemAssignInfo> > SystemTableModel::getAssignmentMap()
{
    return mSystemsAssigned;
}

