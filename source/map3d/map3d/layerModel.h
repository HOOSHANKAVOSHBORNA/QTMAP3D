#ifndef LAYERSMODEL_H
#define LAYERSMODEL_H

#include "mapItem.h"
#include "treeModel.h"
#include <osgEarth/Layer>



class LayersModel : public TreeProxyModel
{
    Q_OBJECT
    enum CustomRoles{
        visibleLayerRole =Qt::UserRole + 100
    };

public:
    LayersModel(MapItem *mapController = nullptr, QObject *parent = nullptr);
    QHash<int,QByteArray> roleNames() const override;
    void initializeModel(osgEarth::Map *map);
    bool getLayerVisible(QModelIndex itemIndex) const;


public slots:
    void onItemClicked(const QModelIndex &current)override;

    void onLayerAdded(osgEarth::Layer* layer);
    void onLayerRemoved(osgEarth::Layer* layer);
    void onNodeToLayerAdded(osg::Node *node, osgEarth::Layer *layer);
    void onNodeFromLayerRemoved(osg::Node *node, osgEarth::Layer *layer);
    void onParentLayerChanged(osgEarth::Layer *layer, osgEarth::Layer *oldParentLayer, osgEarth::Layer *newParentLayer);

private:
    void setLayerVisible(osgEarth::VisibleLayer *layer);
    QStandardItem mLayerList;
    MapItem *mMapItem;
    TreeModel *mTreeModel;
};


#endif // LAYERSMODEL_H
