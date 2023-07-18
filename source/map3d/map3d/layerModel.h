#ifndef LAYERSMODEL_H
#define LAYERSMODEL_H

#include "mapItem.h"
#include "treeModel.h"
#include <osgEarth/Layer>

Q_DECLARE_METATYPE(osgEarth::Layer);

class LayersModel : public TreeProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QModelIndex   dropIndex      READ getDropIndex     WRITE setDropIndex    )

    enum CustomRoles{
        visibleRole =Qt::UserRole + 100 ,
        locatableRole =Qt::UserRole + 101 ,
        layerRole = Qt::UserRole + 102

    };

public:
    LayersModel(MapItem *mapController = nullptr, QObject *parent = nullptr);
    QHash<int,QByteArray> roleNames() const override;
    void initializeModel(osgEarth::Map *map);
    bool getLayerVisible(osgEarth::Layer *layer) const;
    QModelIndex getDropIndex();
    void setDropIndex(QModelIndex dropValue);


public slots:
    void onItemClicked(const QModelIndex &current)override;
    void onDeleteLayerClicked(const QModelIndex &current);
    void onGoToClicked(const QModelIndex &current);
    void onReplaceItem(QModelIndex fromIndex);

    void onLayerAdded(osgEarth::Layer* layer , osgEarth::Layer *parentLayer,   unsigned index);
    void onLayerRemoved(osgEarth::Layer* layer ,osgEarth::Layer *parentLayer, unsigned index);
    void onNodeToLayerAdded(osg::Node *node, osgEarth::Layer *layer);
    void onNodeFromLayerRemoved(osg::Node *node, osgEarth::Layer *layer);
    void onParentLayerChanged(osgEarth::Layer *layer, osgEarth::Layer *oldParentLayer, osgEarth::Layer *newParentLayer);

private:
    void setLayerVisible(osgEarth::VisibleLayer *layer);
    QStandardItem mLayerList;
    MapItem *mMapItem;
    TreeModel *mTreeModel;
    QMap<QStandardItem , osgEarth::Layer*> treeLayerMap;
    QModelIndex mDropIndex;


};


#endif // LAYERSMODEL_H
