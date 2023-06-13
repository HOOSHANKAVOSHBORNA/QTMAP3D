
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
//    void clear();


public slots:
    void toggleLayerEnabled(int layerIndex );
    void clickedItem(QModelIndex itemIndex);

public:
//    QStandardItem *rootItem = invisibleRootItem();
//    QStandardItem *group1 = new QStandardItem;
//    QStandardItem *group2 = new QStandardItem;
//    QStandardItem *group3 = new QStandardItem;

//    QStandardItem *value1 = new QStandardItem;
//    QStandardItem *value2 = new QStandardItem;
//    QStandardItem *value3 = new QStandardItem;
//    QStandardItem *value4 = new QStandardItem;
//    QStandardItem *value5 = new QStandardItem;
//    QStandardItem *value6 = new QStandardItem;
//    QStandardItem *value7 = new QStandardItem;
//    QStandardItem *value8 = new QStandardItem;
//    QStandardItem *value9 = new QStandardItem;
//    int rowCount(const QModelIndex &parent) const override;
//    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
//    QHash<int,QByteArray> roleNames() const override;

private:
    QList<osgEarth::Layer*> mLayersList;
    MapItem *mMapItem;
};


#endif // LAYERSMODEL_H
