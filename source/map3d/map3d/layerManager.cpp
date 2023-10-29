#include "layerManager.h"
#include <osgEarth/Map>
#include <osgEarth/ModelLayer>
#include <QHeaderView>
#include <QQmlComponent>
#include <QQuickItem>
#include <osgEarthAnnotation/AnnotationNode>
#include <osgEarthAnnotation/AnnotationNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/GeoPositionNode>

LayerModel *LayerModel::mInstance = nullptr;

LayerModel::LayerModel()
{

}

LayerModel *LayerModel::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(mInstance == nullptr){ mInstance = new LayerModel(); }
    return mInstance;
}

void LayerModel::initialize(MapItem *mapItem)
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
    connect(mapItem->getMapObject(), &MapObject::layerAdded,this ,&LayerModel::onLayerAdded);
    connect(mapItem->getMapObject(), &MapObject::layerRemoved,this ,&LayerModel::onLayerRemoved);
    //    connect(mapItem->getMapObject(), &MapObject::nodeToLayerAdded,this ,&LayersModel::onNodeToLayerAdded);
    //    connect(mapItem->getMapObject(), &MapObject::nodeFromLayerRemoved,this ,&LayersModel::onNodeFromLayerRemoved);
    //    connect(mapItem->getMapObject(), &MapObject::parentLayerChanged,this ,&LayersModel::onParentLayerChanged);
}

void LayerModel::initializeModel(osgEarth::Map *map)
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

QHash<int, QByteArray> LayerModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[VisibleRole] = "visibleRole";
    hash[LayerRole] = "layerRole";
    hash[DropRole] = "dropRole";
    return hash;
}

QModelIndex LayerModel::getDragIndex()
{
    return mDragIndex;
}

void LayerModel::setDragIndex(QModelIndex value)
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

void LayerModel::onVisibleItemClicked(const QModelIndex &current)
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

void LayerModel::onRemoveItemClicked(const QModelIndex &current)
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

void LayerModel::onMoveItem(QModelIndex oldIndex, QModelIndex newIndex)
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

void LayerModel::onLayerAdded(osgEarth::Layer *layer , osgEarth::Layer *parentLayer , unsigned index)
{
    QStandardItem *newItem = new QStandardItem(QString(layer->getName().c_str()));
    newItem->setData(getLayerVisible(layer),VisibleRole);
    newItem->setData(false, DropRole);

    QVariant layerVariant;
    layerVariant.setValue(layer);
    newItem->setData(layerVariant,LayerRole);

    if(parentLayer){
        mLayerToItemMap[parentLayer]->insertRow(index, newItem);
    }
    else
        mSourceModel->invisibleRootItem()->insertRow(index, newItem);

    mLayerToItemMap[layer] = newItem;
}

void LayerModel::onLayerRemoved(osgEarth::Layer *layer , osgEarth::Layer *parentLayer, unsigned index)
{
    if(parentLayer){
        mLayerToItemMap[parentLayer]->removeRow(index);
    }
    else
        mSourceModel->invisibleRootItem()->removeRow(index);
}

void LayerModel::moveItem(QModelIndex from, QModelIndex to)
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

void LayerModel::setItemVisible(QStandardItem *item, bool visible)
{
    item->setData(visible, VisibleRole);
    for(int i = 0; i < item->rowCount(); ++i){
        setItemVisible(item->child(i), visible);
    }
}

bool LayerModel::getLayerVisible(osgEarth::Layer *layer) const
{
    auto visLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    bool visible = visLayer->getVisible();
    return visible;
}

void LayerModel::setLayerVisible(osgEarth::VisibleLayer *layer)
{
    bool visible = layer->getVisible();
    layer->setVisible(!visible);
}

bool LayerModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    if (index.data().toString().contains(mFilterString, Qt::CaseInsensitive))
        return true;
    for (int i = 0; i < sourceModel()->rowCount(index); ++i) {
        if (filterAcceptsRow(i, index))
            return true;
    }
    if (source_parent.isValid() && source_parent.data().toString().contains(mFilterString, Qt::CaseInsensitive))
        return true;
    return false;
}



QString LayerModel::filterString() const
{
    return mFilterString;
}

void LayerModel::setFilterString(const QString &newFilterString)
{
    mFilterString = newFilterString;
    invalidateFilter();
}

// ----------------------------------------------------------------- Model Manager
LayerManager::LayerManager()
{
    mLayerModel = LayerModel::createSingletonInstance(nullptr, nullptr);
}

LayerModel *LayerManager::layerModel() const
{
    return mLayerModel;
}

void LayerManager::onLayerAdded(osgEarth::Layer *layer, osgEarth::Layer *parentLayer, unsigned int index)
{
    mLayerModel->onLayerAdded(layer, parentLayer, index);
}

void LayerManager::onLayerRemoved(osgEarth::Layer *layer, osgEarth::Layer *parentLayer, unsigned int index)
{
    mLayerModel->onLayerRemoved(layer, parentLayer, index);
}

bool LayerManager::getLayerVisible(osgEarth::Layer *layer) const
{
    return mLayerModel->getLayerVisible(layer);
}

void LayerManager::setLayerVisible(osgEarth::VisibleLayer *layer)
{
    mLayerModel->setLayerVisible(layer);
}

LayerManager *LayerManager::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(mInstance == nullptr){ mInstance = new LayerManager(); }
    return mInstance;
}

void LayerManager::initialize(MapItem *mapItem)
{

}
