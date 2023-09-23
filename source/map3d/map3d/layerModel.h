#ifndef LAYERSMODEL_H
#define LAYERSMODEL_H

#include "mapItem.h"
#include "treeModel.h"
#include <osgEarth/Layer>

Q_DECLARE_METATYPE(osgEarth::Layer);

class LayersModel : public TreeProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QModelIndex dragIndex READ getDragIndex WRITE setDragIndex)

    enum Role{
        VisibleRole = Qt::UserRole + 100 ,
        LayerRole,
        DropRole
    };

public:
    LayersModel(MapItem *mapController = nullptr, QObject *parent = nullptr);
    void initializeModel(osgEarth::Map *map);
    QHash<int,QByteArray> roleNames() const override;
    QModelIndex getDragIndex();
    void setDragIndex(QModelIndex value);

public slots:
    void onVisibleItemClicked(const QModelIndex &current);
    void onRemoveItemClicked(const QModelIndex &current);
    void onMoveItem(QModelIndex oldIndex, QModelIndex newIndex);

    void onLayerAdded(osgEarth::Layer* layer , osgEarth::Layer *parentLayer,   unsigned index);
    void onLayerRemoved(osgEarth::Layer* layer ,osgEarth::Layer *parentLayer, unsigned index);
private:
    void moveItem(QModelIndex from , QModelIndex to);
    void setItemVisible(QStandardItem *item, bool visible);
    bool getLayerVisible(osgEarth::Layer *layer) const;
    void setLayerVisible(osgEarth::VisibleLayer *layer);
private:
//    QStandardItem mLayerList;
    MapItem *mMapItem;
    QStandardItemModel *mSourceModel;
//    QMap<QStandardItem , osgEarth::Layer*> treeLayerMap;
    QModelIndex mDragIndex;
    std::map<osgEarth::Layer*, QStandardItem*> mLayerToItemMap;


};


#endif // LAYERSMODEL_H
