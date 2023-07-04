#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QItemSelectionModel>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QVariant>
#include <QList>
#include <QSortFilterProxyModel>
#include <QMap>


class TreeModel : public QStandardItemModel
{
    Q_OBJECT

public:
    TreeModel(QObject *parent = nullptr);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    void addItem(QStandardItem *item , const QString parent = "root");
    void removeItem(QString item , const QString parent = "root");

private:
    QModelIndex previous;
    QStandardItem *rootItem;
    QMap<QString , QStandardItem*> treeMap;
};

class TreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TreeProxyModel(QObject *parent = nullptr);
    QString filterString() const;
public slots:
    virtual void onItemClicked(const QModelIndex &current);
    void setFilterString(const QString &filterString);
signals:
    void filterStringChanged();
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
private:
    QString mFilterString = "";

};

#endif // TREEMODEL_H
