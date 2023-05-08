#include "aircraftTableModel.h"
#include "aircraftDataManager.h"
#include "systemModelNode.h"
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
}

int AircraftTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 16;
}

int AircraftTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(mAircraftInfosProxy.size());
}

QVariant AircraftTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mAircraftInfosProxy.size()))
        return QVariant();

    const int _row = index.row();

    switch (role) {
    case Qt::DisplayRole:
    {
        time_t datetime = (*mAircraftInfos)[mAircraftInfosProxy[_row]].info.Time;
        char buffer[256];
        std::tm* currTm = localtime(&datetime);
        strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M", currTm);
        switch(index.column()) {
        case  0: return QVariant::fromValue<QString>( (*mAircraftInfos)[mAircraftInfosProxy[_row]].info.identifyToString());
        case  1: return QVariant::fromValue<int>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.TN);
        case  2: return QVariant::fromValue<QString>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.IFFCode);
        case  3: return QVariant::fromValue<QString>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.CallSign);
        case  4: return QVariant::fromValue<QString>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.aircraftTypeToString());
        case  5: return QVariant::fromValue<QString>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.MasterRadar);
        case  6: return QVariant::fromValue<QString>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.IdentificationMethod);
        case  7: return QVariant::fromValue<QString>(buffer);
        case  8: return QVariant::fromValue<QString>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.Pos);
        case  9: return QVariant::fromValue<QString>(QString::number((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.Latitude, 'f', 6));
        case 10: return QVariant::fromValue<QString>(QString::number((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.Longitude, 'f', 6));
        case 11: return QVariant::fromValue<QString>(QString::number((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.Altitude, 'f', 6));
        case 12: return QVariant::fromValue<double>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.Heading);
        case 13: return QVariant::fromValue<double>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.Speed);
        case 14: return QVariant::fromValue<QString>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.detectionSystemsToString());
        case 15: return QVariant::fromValue<QString>((*mAircraftInfos)[mAircraftInfosProxy[_row]].info.sendsToString());
        }

        break;
    }


    case BackColorRole:
    {
        switch (index.column()) {
        case 0: return QVariant::fromValue<QColor>((*mAircraftInfos)[mAircraftInfosProxy[index.row()]].info.aircraftColor());
        default : return QVariant::fromValue<QString>("transparent");
        }
    }

    case TextColorRole:
    {
        switch (index.column()) {
            case 0: return QVariant::fromValue<QColor>(QColor("black"));
            default: return QVariant::fromValue<QColor>(QColor("white"));
        }
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
        return QVariant::fromValue<QColor>((*mAircraftInfos)[index.row()].info.aircraftColor());
    }

    case AircraftHoverColor:
    {
        return QVariant::fromValue<QColor>((*mAircraftInfos)[index.row()].info.aircraftHoverColor());
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

    if (row >= static_cast<int>(mAircraftInfosProxy.size())) {
        return -1;
    }

    return (*mAircraftInfos)[mAircraftInfosProxy[row]].info.TN;
}

void AircraftTableModel::setAircraftInfos(const QMap<int, Aircraft::Data> &aircrafts)
{
    beginResetModel();
    mAircraftInfos = &aircrafts;
    mAircraftInfosProxy = mAircraftInfos->keys();
    endResetModel();
}

void AircraftTableModel::updateTable(int tn)
{
    if (!mAircraftInfos->contains(tn) && mAircraftInfosProxy.contains(tn)) {
        beginRemoveRows(QModelIndex(), mAircraftInfosProxy.indexOf(tn), mAircraftInfosProxy.indexOf(tn));
        mAircraftInfosProxy = mAircraftInfos->keys();
        endRemoveRows();
    }

    else if (mAircraftInfos->contains(tn) && !mAircraftInfosProxy.contains(tn)) {
        mAircraftInfosProxy = mAircraftInfos->keys();
        setFilterWildcard(mFilter);
    }

    else {
        int row = mAircraftInfosProxy.indexOf(tn);
        emit dataChanged(createIndex(row, 0), createIndex(row, 16));
    }


}

void AircraftTableModel::setFilterWildcard(const QString &wildcard)
{
    beginResetModel();

    mFilter = wildcard;
    mFilter.remove(QRegularExpression("\\s"));

    mAircraftInfosProxy.clear();
    for (auto& item : mAircraftInfos->keys()) {
        if (QString::number(item).contains(mFilter))
            mAircraftInfosProxy.push_back(item);
    }

    endResetModel();
}

void AircraftTableModel::sortWithHeader(int column)
{
//    switch (column) {
//    case 0: std::sort(mAircraftInfos.begin(), mAircraftInfos.end(), [=](const QSharedPointer<AircraftInfo> &item1, const QSharedPointer<AircraftInfo> &item2) {
//            return item1->identifyToString() < item2->identifyToString();
//        });
//        break;
//    }
//    mNeedUpdateOnTimerTrigger = true;
//    onUpdateTimerTriggered();


}

void AircraftTableModel::onAircraftClicked(int TN)
{
    emit aircraftClicked(TN);
//    refresh();
}

void AircraftTableModel::onSystemClicked(int Number) {


}

void AircraftTableModel::refresh()
{
    beginResetModel();
//    mNumber = -1;
    mAircraftInfosProxy.clear();
    mAircraftInfosProxy = mAircraftInfos->keys();
    endResetModel();
}

void AircraftTableModel::setMode(QString mode)
{
    mMode = mode;
}
