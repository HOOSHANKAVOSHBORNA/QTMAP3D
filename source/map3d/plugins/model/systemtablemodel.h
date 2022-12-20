#ifndef SYSTEMTABLEMODEL_H
#define SYSTEMTABLEMODEL_H

#include <QAbstractTableModel>

class SystemTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SystemTableModel(QObject *parent = nullptr);

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // SYSTEMTABLEMODEL_H
