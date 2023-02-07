#include "assignmentModel.h"

#include <QStringLiteral>
#include <QDebug>

AssignmentModel::AssignmentModel(QObject *parent)
    : QAbstractTableModel(parent)
{
//    0for (int i = 0; i < 15; ++i) {
        AircraftAssign a;
        a.Number = 6;
        QPair<int, QSharedPointer<AircraftAssign>> isp;
        isp.first = 1;
        isp.second.reset(new AircraftAssign);
        *(isp.second) = a;
        mAircraftList.push_back(isp);

//    }
    AircraftAssign a2;
    a2.Number = 5;
    QPair<int, QSharedPointer<AircraftAssign>> isp2;
    isp2.first = 2;
    isp2.second.reset(new AircraftAssign);
    *(isp2.second) = a2;
    mAircraftList.push_back(isp2);
}

int AssignmentModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

int AssignmentModel::rowCount(const QModelIndex &parent) const
{
    return mAircraftList.size();
}

QVariant AssignmentModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mAircraftList.size()))
        return QVariant();
    switch (role) {
    case SysDisp: {
        switch (index.column()) {
        case 0: return QVariant::fromValue<QString>(QString::number(mAircraftList[static_cast<size_t>(index.row())].first));
        case 1: return QVariant::fromValue<int>(mAircraftList[static_cast<size_t>(index.row())].second->Number);
        case 2: return QVariant::fromValue<QString>("test");
        case 3: return QVariant::fromValue<QString>("trest2");
        default: return QVariant();
        }
    }
    case AirDisp: {
        switch (index.column()) {
        case 0: return QVariant::fromValue<QString>(QString::number(mSystemList[static_cast<size_t>(index.row())].first));
        case 1: return QVariant::fromValue<int>(mSystemList[static_cast<size_t>(index.row())].second->Number);
        case 2: return QVariant::fromValue<QString>("test");
        case 3: return QVariant::fromValue<QString>("trest2");
        default: return QVariant();
        }
    }
    case HeaderText: {
        switch (index.row()) {
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
