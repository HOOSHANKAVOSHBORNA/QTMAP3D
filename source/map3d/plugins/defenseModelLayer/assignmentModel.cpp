#include "assignmentModel.h"

#include <QStringLiteral>
#include <QDebug>

AssignmentModel::AssignmentModel(QObject *parent)
    : QAbstractTableModel(parent)
{
//    0for (int i = 0; i < 15; ++i) {
        AircraftAssign a;
        a.Number = 6;
        updateItemData(a);
//    }
    AircraftAssign a2;
    a2.Number = 5;
    updateItemData(a2);
}

int AssignmentModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

int AssignmentModel::rowCount(const QModelIndex &parent) const
{
    return 5;
    if (parent.column() == 1)
        return 2;
    else {
        return 0;
    }
}

QVariant AssignmentModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case AirDisp: {
        if (index.row() >= int(mAircraftList.size()))
            return QVariant::fromValue<QString>("");
        else {
            switch (index.column()) {
            case 0: return QVariant::fromValue<QString>(QString::number(mAircraftList[static_cast<size_t>(index.row())].first));
            case 1: return QVariant::fromValue<int>(mAircraftList[static_cast<size_t>(index.row())].second->Number);
            case 2: return QVariant::fromValue<QString>("test");
            case 3: return QVariant::fromValue<QString>("trest2");
            default: return QVariant::fromValue<QString>("");
            }
        }
    }
    case SysDisp: {
        if (index.row() >= int(mSystemList.size()))
            return QVariant::fromValue<QString>("");
        else {
            switch (index.column()) {
            case 0: return QVariant::fromValue<QString>(QString::number(mSystemList[static_cast<size_t>(index.row())].first));
            case 1: return QVariant::fromValue<int>(mSystemList[static_cast<size_t>(index.row())].second->Number);
            case 2: return QVariant::fromValue<QString>("test");
            case 3: return QVariant::fromValue<QString>("trest2");
            default: return QVariant::fromValue<QString>("");
            }
        }
    }
    case HeaderText: {
        switch (index.column()) {
        case 0: return QVariant::fromValue<QString>("Index");
        case 1: return QVariant::fromValue<QString>("Number");
        case 2: return QVariant::fromValue<QString>("Name");
        case 3: return QVariant::fromValue<QString>("other");
        }
    }

    default: return 0;
    }
}

QHash<int, QByteArray> AssignmentModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractTableModel::roleNames();
    hash[SysDisp] = "SysDisp";
    hash[AirDisp] = "AirDisp";
    hash[HeaderText] = "HeaderText";
    return hash;
}

void AssignmentModel::onAircraftClicked(int row)
{
    beginResetModel();
    mSystemListProxy.clear();
    for (auto system : mAircraftList[std::size_t(row)].second->Systems) {
       for (auto &item : mSystemList) {
           if (system.Number == item.second->Number)
               mSystemListProxy.push_back(item);
       }
    }
    endResetModel();
}

void AssignmentModel::onSystemClicked(int row)
{
    beginResetModel();
    mAircraftListProxy.clear();
    for (auto aircraft : mSystemList[std::size_t(row)].second->Aircrafts) {
       for (auto &item : mAircraftList) {
           if (aircraft.Number == item.second->Number)
               mAircraftListProxy.push_back(item);
       }
    }
    endResetModel();
}

void AssignmentModel::updateItemData(const AircraftAssign &aircraft)
{
    beginResetModel();
    auto it = std::find_if(mAircraftList.begin(), mAircraftList.end(), [aircraft](QPair<int, QSharedPointer<AircraftAssign>>& item) {
            return item.second->Number == aircraft.Number;
        });
    if (it != mAircraftList.end())
        *(*it).second = aircraft;
    else {
        QPair<int, QSharedPointer<AircraftAssign>> isp;
        isp.first = mAircraftList.size();
        isp.second.reset(new AircraftAssign);
        *(isp.second) = aircraft;
        mAircraftList.push_back(isp);
    }
    endResetModel();
}
void AssignmentModel::updateData(const SystemAssign &system)
{
    beginResetModel();
    auto it = std::find_if(mSystemList.begin(), mSystemList.end(), [system](QPair<int, QSharedPointer<SystemAssign>>& item) {
            return item.second->Number == system.Number;
        });
    if (it != mSystemList.end())
        *(*it).second = system;
    else {
        QPair<int, QSharedPointer<SystemAssign>> isp;
        isp.first = mAircraftList.size();
        isp.second.reset(new SystemAssign);
        *(isp.second) = system;
        mSystemList.push_back(isp);
    }
    endResetModel();
}

void AssignmentModel::clear()
{
    beginResetModel();
    mSystemList.clear();
    mSystemListProxy.clear();
    mAircraftList.clear();
    mAircraftListProxy.clear();
    endResetModel();
}

int AssignmentModel::getAircraftNumber(int row) const
{
    if (row < 0) return -1;

    if (row >= static_cast<int>(mAircraftList.size())) {
        return -1;
    }

    return mAircraftList[std::size_t(row)].second->Number;
}

int AssignmentModel::getSystemNumber(int row) const
{
    if (row < 0) return -1;

    if (row >= static_cast<int>(mSystemList.size())) {
        return -1;
    }

    return mSystemList[std::size_t(row)].second->Number;
}

QString AssignmentModel::headerText(int column) const
{
    switch (column) {
    case 0: return QStringLiteral("Index");
    case 1: return QStringLiteral("Number");
    case 2: return QStringLiteral("Name");
    case 3: return QStringLiteral("other");
    default: return QStringLiteral("");
    }
}
