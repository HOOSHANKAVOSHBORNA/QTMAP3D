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

public:
    void updateLayers(osgEarth::Map *map);


public slots:
//    void clickedItem(QModelIndex itemIndex);
    void onItemClicked(const QModelIndex &current)override;
    bool getLayerVisible(QModelIndex itemIndex) const;

private:
    void setLayerVisible(osgEarth::VisibleLayer *layer);
//    void addChildItem(osg::Group *parentGroup, QStandardItem *parentItem);
//    void addChildItem(osgEarth::Layer *layer, QStandardItem *parentItem);
private:

//    QList<osgEarth::Layer*> mLayersList;
    QStandardItem mLayerList;
    MapItem *mMapItem;
    TreeModel *mTreeModel;
//    TreeProxyModel *mTreeProxyModel;
};


#endif // LAYERSMODEL_H
