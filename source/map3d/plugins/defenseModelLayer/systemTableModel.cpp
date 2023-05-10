#include "systemTableModel.h"
#include "systemDataManager.h"
#include "aircraftDataManager.h"

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
        case  0: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Number);
        case  1: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Name);
        case  2: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Type);
        case  3: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Terminal);
        case  4: return mMode == "Assignment" ? QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.phaseToString()):
                                                QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.TN);
        case  5: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Latitude);
        case  6: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Longitude);
        case  7: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Altitude);
        case  8: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.ViewRange);
        case  9: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.MezRange);
        case 10: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.ReceiveTime);
        case 11: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.Simulation);
        case 12: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.BCCStatusToString());
        case 13: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.radarSearchStatusToString());
        case 14: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.operationalToString());
        case 15: return QVariant::fromValue<int>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.MissileCount);
        case 16: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.RadarMode);
        case 17: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.Acceptance);
        case 18: return mMode == "Assignment" ? QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.TN):
                                                QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.phaseToString());
        case 19: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.Antenna);
        case 20: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.ChanelNo);
        case 21: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.Inrange);

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
                case 4: return QVariant::fromValue<QColor>(QColor((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.phaseToColor()));
                default : return QVariant::fromValue<QColor>(QColor("transparent"));

            }
        }
        return QVariant::fromValue<QColor>(QColor((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.phaseToColor()));
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

    return (*mSystemInfos)[mSystemInfosProxy[row]]->information->systemInfo.Number;
}

void SystemTableModel::setFilterWildcard(const QString &wildcard)
{
        beginResetModel();

        mFilter = wildcard;
        mFilter.remove(QRegularExpression("\\s"));

        mSystemInfosProxy.clear();
        for (auto& item : *mSystemInfos) {
            if (QString::number(item->information->systemInfo.Number).contains(mFilter))
                mSystemInfosProxy.push_back(item->information->systemInfo.Number);
        }

        endResetModel();
}

void SystemTableModel::onAircraftClicked(int TN)
{
    mTN = TN;
    beginResetModel();
    mSystemInfosProxy.clear();
    updateAssignments();
    endResetModel();
}

void SystemTableModel::onSystemClicked(int Number)
{
    emit systemClicked(Number);
//    refresh();
}

void SystemTableModel::refresh()
{
    beginResetModel();
    mTN = -1;
    mSystemInfosProxy.clear();
    mSystemInfosProxy = mSystemInfos->keys();
    endResetModel();
}

void SystemTableModel::setMode(QString mode)
{
    mMode = mode;
}

void SystemTableModel::updateAssignments()
{
    if (mTN != -1) {
        mSystemInfosProxy.clear();
        for (auto& system :  (*mAircraftInfos)[mTN]->assignments){
            mSystemInfosProxy.push_back(system->info->systemInfo.Number);
        }
    }
}

void SystemTableModel::updateTable(int number)
{
    if (mTN == -1) {
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
    else {
        updateAssignments();
    }
}

void SystemTableModel::setSystemInfos(const QMap<int, System::Data*> &systems)
{
    beginResetModel();
    mSystemInfos = &systems;
    mSystemInfosProxy = mSystemInfos->keys();
    endResetModel();
}

void SystemTableModel::setAircraftInfos(const QMap<int, Aircraft::Data*> &info)
{
    mAircraftInfos = &info;
}

