#include "systemTableModel.h"
#include "systemDataManager.h"

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
    return static_cast<int>(mSystemInfosProxy.size());
}

QVariant SystemTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mSystemInfosProxy.size()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        switch(index.column()) {
        case  0: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemInfo.Number);
        case  1: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemInfo.Name);
        case  2: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemInfo.Type);
        case  3: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemInfo.Terminal);
        case  4: return mMode == "Assignment" ? QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemCombatInfo.phaseToString()):
                                                QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemCombatInfo.TN);
        case  5: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemInfo.Latitude);
        case  6: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemInfo.Longitude);
        case  7: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemInfo.Altitude);
        case  8: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemInfo.ViewRange);
        case  9: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemInfo.MezRange);
        case 10: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemStatusInfo.ReceiveTime);
        case 11: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemStatusInfo.Simulation);
        case 12: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemStatusInfo.BCCStatusToString());
        case 13: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemStatusInfo.radarSearchStatusToString());
        case 14: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemStatusInfo.operationalToString());
        case 15: return QVariant::fromValue<int>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemStatusInfo.MissileCount);
        case 16: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemStatusInfo.RadarMode);
        case 17: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemCombatInfo.Acceptance);
        case 18: return mMode == "Assignment" ? QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemCombatInfo.TN):
                                                QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemCombatInfo.phaseToString());
        case 19: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemCombatInfo.Antenna);
        case 20: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemCombatInfo.ChanelNo);
        case 21: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemCombatInfo.Inrange);

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
        if (static_cast<size_t>(index.row()) >= mSystemInfosProxy.size())
            return QVariant::fromValue<QColor>("white");
        if (mMode == "Assignment") {
            switch (index.column()) {
                case 4: return QVariant::fromValue<QColor>(QColor((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemCombatInfo.phaseToColor()));
                default : return QVariant::fromValue<QColor>(QColor("transparent"));

            }
        }
        return QVariant::fromValue<QColor>(QColor((*mSystemInfos)[mSystemInfosProxy[index.row()]].information.systemCombatInfo.phaseToColor()));
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

    if (row >= static_cast<int>(mSystemInfosProxy.size())) {
        return -1;
    }

    return (*mSystemInfos)[mSystemInfosProxy[row]].information.systemInfo.Number;
}

void SystemTableModel::setFilterWildcard(const QString &wildcard)
{
        beginResetModel();

        mFilter = wildcard;
        mFilter.remove(QRegularExpression("\\s"));

        mSystemInfosProxy.clear();
        for (auto& item : *mSystemInfos) {
            if (QString::number(item.information.systemInfo.Number).contains(mFilter))
                mSystemInfosProxy.push_back(item.information.systemInfo.Number);
        }

        endResetModel();
}

void SystemTableModel::onAircraftClicked(int TN)
{
//    mTN = TN;
//    beginResetModel();
//    mSystemInfosProxy.clear();
//    mSystemInfosProxy.clear();
//    mSystemInfosProxy.clear();
//    if (mSystemsAssigned.contains(TN)) {
//        for (SystemAssignInfo system : mSystemsAssigned[TN]) {
//            auto it = std::find_if(mSystemInfos.begin(), mSystemInfos.end(), [system](QSharedPointer<SystemInfo> &item){
//                return item->Number == system.Number;
//            });
//            mSystemInfosProxy.push_back(*it);

//            auto it2 = std::find_if(mSystemCombatInfos.begin(), mSystemCombatInfos.end(), [system](QSharedPointer<SystemCombatInfo> &item){
//                return item->Number == system.Number;
//            });
//            mSystemInfosProxy.push_back(*it2);

//            auto it3 = std::find_if(mSystemStatusInfos.begin(), mSystemStatusInfos.end(), [system](QSharedPointer<SystemStatusInfo> &item){
//                return item->Number == system.Number;
//            });
//            mSystemInfosProxy.push_back(*it3);
//        }
//    }
//    endResetModel();
}

void SystemTableModel::onSystemClicked(int Number)
{
    emit systemClicked(Number);
//    refresh();
}


void SystemTableModel::assign(int Number, int TN)
{
//    SystemAssignInfo tmp;
//    tmp.Number = Number;
//    tmp.Phase = "";
//    if (mSystemsAssigned.contains(TN)) {
//        mSystemsAssigned[TN].push_back(tmp);
//    }
//    else {
//        mSystemsAssigned[TN] = QList<SystemAssignInfo> {tmp};
//    }
//    if (mMode == "Assignment") {
//        if (mTN > 0)
//            onAircraftClicked(TN);
//    }

}

void SystemTableModel::cancelSystemsAssigned(int TN, int ExceptNum)
{

//    if (mSystemsAssigned.contains(TN)){
//        for (auto &system : mSystemsAssigned[TN]){
//            if (system.Number != ExceptNum) {
//                cancelAssign(system.Number, ExceptNum);
//            }
//        }
//    }
//    if (mMode == "Assignment") {
//        if (mTN > 0)
//            onAircraftClicked(TN);
//    }
}

void SystemTableModel::cancelAllAssigns()
{
    mSystemsAssigned.clear();
    refresh();
}

void SystemTableModel::cancelAssign(int TN, int Number)
{
//    if (mSystemsAssigned.contains(TN)) {
//        auto it = std::remove_if(mSystemsAssigned[TN].begin(), mSystemsAssigned[TN].end(), [Number](SystemAssignInfo &system){
//            return system.Number == Number;
//    });
//        if (it != mSystemsAssigned[TN].end())
//            mSystemsAssigned[TN].erase(it);
//    }
//    if (mMode == "Assignment"){
//        if (mTN > 0) {
//            onAircraftClicked(mTN);
//        }
//    }
}

void SystemTableModel::acceptAssign(int TN, int Number, bool result)
{
//    if (result) {
//        auto it = std::find_if(mSystemsAssigned[TN].begin(), mSystemsAssigned[TN].end(), [Number](SystemAssignInfo &item) {
//            return Number == item.Number;
//        });
//        if (it != mSystemsAssigned[TN].end())
//            it->assign = true;
//    }
//    else {
//        cancelAssign(TN, Number);
//    }
}

void SystemTableModel::refresh()
{
    beginResetModel();
    mTN = -1;
    mSystemInfosProxy.clear();
    endResetModel();
}

void SystemTableModel::setMode(QString mode)
{
    mMode = mode;
}

//QMap<int, QList<SystemAssignInfo> > SystemTableModel::getAssignmentMap()
//{
//    return mSystemsAssigned;
//}

//void SystemTableModel::setSystemStatusInfos(QMap<int, SystemStatusInfo> &statusInfos)
//{
//    mSystemStatusInfos = &statusInfos;
//}

//void SystemTableModel::setSystemCombatInfos(QMap<int, SystemCombatInfo> &combatInfos)
//{
//    mSystemCombatInfos = &combatInfos;
//}

void SystemTableModel::updateTable(int number)
{
    if (!mSystemInfos->contains(number) && mSystemInfosProxy.contains(number)) {
        beginRemoveRows(QModelIndex(), mSystemInfosProxy.indexOf(number), mSystemInfosProxy.indexOf(number));
        mSystemInfosProxy = mSystemInfos->keys();
        endRemoveRows();
    }

    else if (mSystemInfos->contains(number) && !mSystemInfosProxy.contains(number)) {
        mSystemInfosProxy = mSystemInfos->keys();
        setFilterWildcard(mFilter);
    }

    else {
        int row = mSystemInfosProxy.indexOf(number);
        emit dataChanged(createIndex(row, 0), createIndex(row, 22));
    }
}

void SystemTableModel::setSystemInfos(QMap<int, System::Data> &systems)
{
    beginResetModel();
    mSystemInfos = &systems;
    mSystemInfosProxy = mSystemInfos->keys();
    endResetModel();
}

