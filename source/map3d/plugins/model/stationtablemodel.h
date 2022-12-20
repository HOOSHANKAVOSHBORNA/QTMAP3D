#ifndef STATIONTABLEMODEL_H
#define STATIONTABLEMODEL_H

#include <QAbstractTableModel>

class StationTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    StationTableModel(QObject *parent = nullptr);

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

};

#endif // STATIONTABLEMODEL_H
