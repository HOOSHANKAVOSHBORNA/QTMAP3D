#include "stationtablemodel.h"

StationTableModel::StationTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

int StationTableModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

int StationTableModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant StationTableModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

