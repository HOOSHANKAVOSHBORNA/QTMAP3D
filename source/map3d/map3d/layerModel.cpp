#include "layerModel.h"
#include <osgEarth/Map>
#include <osgEarth/ModelLayer>
#include <QHeaderView>
#include <QQmlComponent>
#include <QQuickItem>
#include <osgEarthAnnotation/AnnotationNode>
#include <osgEarthAnnotation/AnnotationNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/GeoPositionNode>


LayersModel::LayersModel(MapItem *mapItem, QObject *parent) :
    TreeProxyModel(parent)
{
    mSourceModel = new QStandardItemModel;
    mMapItem = mapItem;

    initializeModel(mapItem->getMapNode()->getMap());
    setSourceModel(mSourceModel);
//    connect(mapItem, &MapItem::layerChanged,[this, mapItem](){
//        mTreeModel->removeRows(0,mTreeModel->rowCount());
//        initializeModel(mapItem->getMapNode()->getMap());
//    });
    connect(mapItem, &MapItem::mapCleared,[this, mapItem](){
        mSourceModel->clear();
        initializeModel(mapItem->getMapNode()->getMap());
    });
    connect(mapItem->getMapObject(), &MapObject::layerAdded,this ,&LayersModel::onLayerAdded);
    connect(mapItem->getMapObject(), &MapObject::layerRemoved,this ,&LayersModel::onLayerRemoved);
//    connect(mapItem->getMapObject(), &MapObject::nodeToLayerAdded,this ,&LayersModel::onNodeToLayerAdded);
//    connect(mapItem->getMapObject(), &MapObject::nodeFromLayerRemoved,this ,&LayersModel::onNodeFromLayerRemoved);
//    connect(mapItem->getMapObject(), &MapObject::parentLayerChanged,this ,&LayersModel::onParentLayerChanged);
}

void LayersModel::initializeModel(osgEarth::Map *map)
{
    osgEarth::LayerVector layers;
    //    map.
    //    QStandardItem *rootItem = mTreeModel->getRootItem();
    //    osgEarth::Layer rootLayer = mMapItem->getRoot();
    //    QVariant rootLayerVariant;
    //    rootLayerVariant.setValue(rootLayer);
    //    rootItem->setData(rootLayerVariant,layerRole);
    map->getLayers(layers);
    for(const auto& layer : layers) {
        //        auto parentLayer = mMapItem->getMapObject()->getParentLayer(layer);
        onLayerAdded(layer, nullptr, map->getIndexOfLayer(layer));
    }
}

QHash<int, QByteArray> LayersModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[VisibleRole] = "visibleRole";
    hash[LayerRole] = "layerRole";
    hash[DropRole] = "dropRole";
    return hash;
}

QModelIndex LayersModel::getDragIndex()
{
    return mDragIndex;
}

void LayersModel::setDragIndex(QModelIndex value)
{
    //--set old to false------------------------
    if(mDragIndex.isValid())
    {
        auto itemDrag = mSourceModel->itemFromIndex(mapToSource(mDragIndex));
        QStandardItem *parentItem = itemDrag->parent();
        if(!parentItem){
            parentItem = mSourceModel->invisibleRootItem();
        }
        for(int i = 0; i < parentItem->rowCount(); i++)
        {
            parentItem->child(i, 0)->setData(false, DropRole);
        }
    }
    //--set new to true---------------------------
    mDragIndex = value;
    if(mDragIndex.isValid())
    {
        auto itemDrag = mSourceModel->itemFromIndex(mapToSource(mDragIndex));
        QStandardItem *parentItem = itemDrag->parent();
        if(!parentItem){
            parentItem = mSourceModel->invisibleRootItem();
        }
        for(int i = 0; i < parentItem->rowCount(); i++)
        {
            parentItem->child(i, 0)->setData(true, DropRole);
        }
    }
}

void LayersModel::onVisibleItemClicked(const QModelIndex &current)
{
    QModelIndex indexSource = mapToSource(current);
    QStandardItem *item = mSourceModel->itemFromIndex(indexSource);
    bool visible = item->data(VisibleRole).toBool();
    setItemVisible(item, !visible);

    auto layer = item->data(LayerRole).value<osgEarth::Layer*>();
    auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    if(visibleLayer)
        setLayerVisible(visibleLayer);
}

void LayersModel::onRemoveItemClicked(const QModelIndex &current)
{
    QModelIndex indexSource = mapToSource(current);
    QStandardItem *item =  mSourceModel->itemFromIndex(indexSource);
    QStandardItem *parentItem = item->parent();
    if(!parentItem){
        parentItem = mSourceModel->invisibleRootItem();
    }
    osgEarth::Layer *layer = item->data(LayerRole).value<osgEarth::Layer*>();
    osgEarth::Layer *parentLayer = parentItem->data(LayerRole).value<osgEarth::Layer*>();

    ParenticAnnotationLayer *parenticLayer = dynamic_cast<ParenticAnnotationLayer*>(layer);
    auto compositParentLayer = dynamic_cast<CompositeAnnotationLayer*>(parentLayer);
    if (compositParentLayer && parenticLayer){
        compositParentLayer->removeLayer(parenticLayer);
    }else{
        mMapItem->getMapObject()->removeLayer(layer, parentLayer);
    }


}

void LayersModel::onMoveItem(QModelIndex oldIndex, QModelIndex newIndex)
{
    if(!oldIndex.isValid() || !newIndex.isValid())
        return;
    QModelIndex fromIndexSource = mapToSource(oldIndex);
    QModelIndex toIndexSource = mapToSource(newIndex);

    if((fromIndexSource != toIndexSource)){

        QStandardItem *item =  mSourceModel->itemFromIndex(fromIndexSource);
        osgEarth::Layer *layer = item->data(LayerRole).value<osgEarth::Layer*>();

        QStandardItem *parentItem = item->parent();
        if(!parentItem){
            mMapItem->getMapObject()->moveLayer(layer, toIndexSource.row());
        }
        else{
            osgEarth::Layer *parentLayer = parentItem->data(LayerRole).value<osgEarth::Layer*>();
            auto copositeParentLayer = dynamic_cast<CompositeAnnotationLayer*>(parentLayer);
            ParenticAnnotationLayer *parenticLayer = dynamic_cast<ParenticAnnotationLayer*>(layer);
            if (copositeParentLayer && parenticLayer){
                copositeParentLayer->moveLayer(parenticLayer, toIndexSource.row());
            }
        }

        moveItem(fromIndexSource,toIndexSource);
    }

}

void LayersModel::onLayerAdded(osgEarth::Layer *layer , osgEarth::Layer *parentLayer , unsigned index)
{
    QStandardItem *newItem = new QStandardItem(QString(layer->getName().c_str()));
    newItem->setData(getLayerVisible(layer),VisibleRole);
    newItem->setData(false, DropRole);

    QVariant layerVariant;
    layerVariant.setValue(layer);
    newItem->setData(layerVariant,LayerRole);

    if(parentLayer){
        auto itemList = mSourceModel->findItems(QString(parentLayer->getName().c_str()));
        for(auto& item: itemList){
            osgEarth::Layer *itemLayer = item->data(LayerRole).value<osgEarth::Layer*>();
            if(itemLayer == parentLayer){
                item->insertRow(index, newItem);
                break;
            }
        }
    }
    else
        mSourceModel->invisibleRootItem()->insertRow(index, newItem);
}

void LayersModel::onLayerRemoved(osgEarth::Layer *layer , osgEarth::Layer *parentLayer, unsigned index)
{
    if(parentLayer){
        auto itemList = mSourceModel->findItems(QString(parentLayer->getName().c_str()));
        for(auto& item: itemList){
            osgEarth::Layer *itemLayer = item->data(LayerRole).value<osgEarth::Layer*>();
            if(itemLayer == parentLayer){
                item->removeRow(index);
                break;
            }
        }
    }
    else
        mSourceModel->invisibleRootItem()->removeRow(index);
}

void LayersModel::moveItem(QModelIndex from, QModelIndex to)
{
    if(from.parent() == to.parent()){
        QStandardItem *itemToBeMoved = mSourceModel->itemFromIndex(from);
        QStandardItem *itemsParent = itemToBeMoved->parent();
        if(!itemsParent){
            itemsParent  = mSourceModel->invisibleRootItem();
        }
        itemsParent->takeRow(from.row());
        itemsParent->insertRow(to.row(), itemToBeMoved);
    }
}

void LayersModel::setItemVisible(QStandardItem *item, bool visible)
{
    item->setData(visible, VisibleRole);
    for(int i = 0; i < item->rowCount(); ++i){
        setItemVisible(item->child(i), visible);
    }
}

bool LayersModel::getLayerVisible(osgEarth::Layer *layer) const
{
    auto visLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    bool visible = visLayer->getVisible();
    return visible;
}

void LayersModel::setLayerVisible(osgEarth::VisibleLayer *layer)
{
    bool visible = layer->getVisible();
    layer->setVisible(!visible);
}

