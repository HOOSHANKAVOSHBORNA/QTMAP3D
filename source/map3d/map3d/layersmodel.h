
#ifndef LAYERSMODEL_H
#define LAYERSMODEL_H

#include "mapItem.h"

#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>
#include <QList>
#include <osgEarth/Layer>
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

//class TreeItem
//{
//public:
//    explicit TreeItem(const QList<QVariant> &data, ToolboxItem *toolbox = nullptr, TreeItem *parentItem = nullptr);
//    ~TreeItem();

//    void appendChild(TreeItem *child);

//    TreeItem *child(int row);
//    int childCount() const;
//    int columnCount() const;
//    QVariant data(int column) const;
//    int row() const;
//    TreeItem *parentItem();
//    ToolboxItem* getToolboxItem() const;
//    TreeItem *child(TreeItem* row);
//    QString imageSource();

//private:
//    QList<TreeItem *> m_childItems;
//    QList<QVariant> m_itemData;
////    ToolboxItem *mToolboxItem;
//    TreeItem *m_parentItem;
//};

class LayersModel : public QStandardItemModel
{
    Q_OBJECT
public:
    LayersModel(MapItem *mapController = nullptr, QObject *parent = nullptr);

    enum {
        LayerIndexRole = Qt::UserRole + 300,
        LayerEnabledRole = Qt::UserRole + 301
    };

public:
    void updateLayers(osgEarth::Map *map);
//    int rowCount(const QModelIndex& parent) const;
//    void clear();


public slots:
    void toggleLayerEnabled(int layerIndex );
    void clickedItem(QModelIndex itemIndex);

//public slots:
//    void onItemClicked(const QModelIndex &current);

//private:
//    QMap<QString, QStandardItem*> mItems;
//    QModelIndex previous;
//    QMap<QString, ToolboxItem*> mToolboxItems;
//    TreeItem *rootItem;

private:
    void setLayerVisible(osgEarth::VisibleLayer *layer);
    void addChildItem(osg::Group *parentGroup, QStandardItem *parentItem);
    void addChildItem(osgEarth::Layer *layer, QStandardItem *parentItem);
private:

//    QList<osgEarth::Layer*> mLayersList;
    QStandardItem mLayerList;
    MapItem *mMapItem;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// define proxy model
class LayersProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    LayersProxyModel(QObject *parent = nullptr);

    QString filterString() const;

public slots:
    void onItemClicked(const QModelIndex &current);
    void setFilterString(const QString &filterString);

signals:
    void filterStringChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString mFilterString = "";
};

#endif // LAYERSMODEL_H
