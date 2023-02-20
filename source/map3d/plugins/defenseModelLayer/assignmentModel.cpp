#include "assignmentModel.h"

#include <QStringLiteral>
#include <QDebug>

AssignmentModel::AssignmentModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int AssignmentModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 4;
}

int AssignmentModel::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(std::max(mAircraftList.size(), mSystemList.size()));
}

QVariant AssignmentModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case AirDisp: {
        if (index.row() >= int(mAircraftListProxy.size()))
            return QVariant::fromValue<QString>("");
        else {
            switch (index.column()) {
            case 0: return QVariant::fromValue<QString>(QString::number(mAircraftListProxy[static_cast<size_t>(index.row())].first));
            case 1: return QVariant::fromValue<int>(mAircraftListProxy[static_cast<size_t>(index.row())].second->TN);
            case 2: return QVariant::fromValue<QString>(mAircraftListProxy[static_cast<size_t>(index.row())].second->IFFCode);
            case 3: return QVariant::fromValue<QString>(mAircraftListProxy[static_cast<size_t>(index.row())].second->CallSign);
            default: return QVariant::fromValue<QString>("");
            }
        }
    }
    case SysDisp: {
        if (index.row() >= int(mSystemListProxy.size()))
            return QVariant::fromValue<QString>("");
        else {
            switch (index.column()) {
            case 0: return QVariant::fromValue<QString>(QString::number(mSystemList[static_cast<size_t>(index.row())].first));
            case 1: return QVariant::fromValue<int>(mSystemListProxy[static_cast<size_t>(index.row())].second->Number);
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
    showSystyemAssigned = true;
    beginResetModel();
    mSystemListProxy.clear();
    for (auto system : mAircraftListProxy[std::size_t(row)].second->assignedSystems) {
        QPair<int, QSharedPointer<SystemInfo>> isp;
        isp.first = static_cast<int>(mSystemListProxy.size());
        isp.second.reset(new SystemInfo);
        *(isp.second) = system;
        mSystemListProxy.push_back(isp);
    }
    endResetModel();
}

void AssignmentModel::refresh()
{
    beginResetModel();
    showSystyemAssigned = false;
    mSystemListProxy.assign(mSystemList.begin(), mSystemList.end());
    mAircraftListProxy.assign(mAircraftList.begin(), mAircraftList.end());
    endResetModel();
}

void AssignmentModel::onSystemClicked(int row)
{
    showAircraftAssign = true;
    beginResetModel();
    mAircraftListProxy.clear();
    for (auto aircraft : mSystemListProxy[std::size_t(row)].second->assignedAircrafts) {

        QPair<int, QSharedPointer<AircraftInfo>> isp;
        isp.first = static_cast<int>(mAircraftListProxy.size());
        isp.second.reset(new AircraftInfo);
        *(isp.second) = aircraft;
        mAircraftListProxy.push_back(isp);
    }
    endResetModel();
}

void AssignmentModel::assignAirToSystem(AircraftInfo &aircraft, SystemInfo &system)
{
    auto it = std::find_if(mAircraftList.begin(), mAircraftList.end(), [aircraft](QPair<int, QSharedPointer<AircraftInfo>> &a) {
        return aircraft.TN == a.second->TN;
    });
    auto it2 = std::find_if(it->second->assignedSystems.begin(), it->second->assignedSystems.end(), [system] (SystemInfo &s){
        return system.Number == s.Number;
    });
    if (it2 == it->second->assignedSystems.end())
        it->second->assignedSystems.push_back(system);
    auto it3 = std::find_if(mSystemList.begin(), mSystemList.end(), [system](QPair<int, QSharedPointer<SystemInfo>> &s) {
        return system.Number == s.second->Number;
    });
    auto it4 = std::find_if(it3->second->assignedAircrafts.begin(), it3->second->assignedAircrafts.end(), [aircraft] (AircraftInfo &a){
        return aircraft.TN == a.TN;
    });
    if (it4 == it3->second->assignedAircrafts.end())
        it3->second->assignedAircrafts.push_back(aircraft);
}

void AssignmentModel::cancelAssign(int TN, int Number)
{
    auto it = std::find_if(mAircraftList.begin(), mAircraftList.end(), [TN](QPair<int, QSharedPointer<AircraftInfo>> &a) {
        return TN == a.second->TN;
    });
    const  auto newEnd = std::remove_if(it->second->assignedSystems.begin(),
                                        it->second->assignedSystems.end(),
                                        [Number](SystemInfo& itemInfo) {

        return itemInfo.Number == Number;
    });


    it->second->assignedSystems.erase(newEnd, it->second->assignedSystems.end());

    auto it2 = std::find_if(mSystemList.begin(), mSystemList.end(), [Number](QPair<int, QSharedPointer<SystemInfo>> &a) {
       return Number == a.second->Number;
    });
    const  auto newEnd2 = std::remove_if(it2->second->assignedAircrafts.begin(),
                                       it2->second->assignedAircrafts.end(),
                                       [&TN](AircraftInfo& itemInfo) {

       return itemInfo.TN == TN;
    });

    it2->second->assignedAircrafts.erase(newEnd2, it2->second->assignedAircrafts.end());
    QAbstractTableModel::dataChanged(createIndex(0, 0), createIndex(it->first, 0));
    QAbstractTableModel::dataChanged(createIndex(0, 0), createIndex(it2->first, 0));

}

void AssignmentModel::addAircraft(AircraftInfo aircraft)
{
    auto it = std::find_if(mAircraftList.begin(), mAircraftList.end(), [aircraft](QPair<int, QSharedPointer<AircraftInfo>> &a) {
        return aircraft.TN == a.second->TN;
    });
    if (it == mAircraftList.end()) {
        beginResetModel();
        QPair<int, QSharedPointer<AircraftInfo>> isp;
        isp.first = static_cast<int>(mAircraftList.size());
        isp.second.reset(new AircraftInfo);
        *(isp.second) = aircraft;
        mAircraftList.push_back(isp);
        endResetModel();
    }
    if (!showAircraftAssign) {
        mAircraftListProxy.assign(mAircraftList.begin(), mAircraftList.end());
    }
}

void AssignmentModel::addSystem(SystemInfo system)
{
    auto it = std::find_if(mSystemList.begin(), mSystemList.end(), [system](QPair<int, QSharedPointer<SystemInfo>> &s) {
        return system.Number == s.second->Number;
    });
    if (it == mSystemList.end()) {
        beginResetModel();
        QPair<int, QSharedPointer<SystemInfo>> isp;
        isp.first = static_cast<int>(mSystemList.size());
        isp.second.reset(new SystemInfo);
        *(isp.second) = system;
        mSystemList.push_back(isp);
        endResetModel();
    }
    if (!showSystyemAssigned)
        mSystemListProxy.assign(mSystemList.begin(), mSystemList.end());
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

    return mAircraftList[std::size_t(row)].second->TN;
}

int AssignmentModel::getSystemNumber(int row) const
{
    if (row < 0) return -1;

    if (row >= static_cast<int>(mSystemList.size())) {
        return -1;
    }

    return mSystemList[std::size_t(row)].second->Number;
}

QString AssignmentModel::aircraftHeaderText(int column) const
{
    switch (column) {
    case 0: return QStringLiteral("Index");
    case 1: return QStringLiteral("Number");
    case 2: return QStringLiteral("IFFCode");
    case 3: return QStringLiteral("CallSign");
    default: return QStringLiteral("");
    }
}

QString AssignmentModel::systemHeaderText(int column) const
{
    switch (column) {
    case 0: return QStringLiteral("Index");
    case 1: return QStringLiteral("Number");
    case 2: return QStringLiteral("BCC");
    case 3: return QStringLiteral("radar");
    default: return QStringLiteral("");
    }
}
