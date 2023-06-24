
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
//    int rowCount(const QModelIndex& parent) const;
//    void clear();


public slots:
    void toggleLayerEnabled(int layerIndex );
    void clickedItem(QModelIndex itemIndex);

private:
    void setLayerVisible(osgEarth::VisibleLayer *layer);
    void addChildItem(osg::Group *parentGroup, QStandardItem *parentItem);
    void addChildItem(osgEarth::Layer *layer, QStandardItem *parentItem);
private:

//    QList<osgEarth::Layer*> mLayersList;
    QStandardItem mLayerList;
    MapItem *mMapItem;
};


#endif // LAYERSMODEL_H
