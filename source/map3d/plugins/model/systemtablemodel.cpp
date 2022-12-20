#include "systemtablemodel.h"

SystemTableModel::SystemTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

int SystemTableModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

int SystemTableModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant SystemTableModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

