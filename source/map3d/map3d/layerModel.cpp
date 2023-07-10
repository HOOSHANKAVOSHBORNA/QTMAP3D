#include "layerModel.h"
#include <osgEarth/Map>
#include <osgEarth/ModelLayer>
#include <QHeaderView>
#include <QQmlComponent>
#include <QQuickItem>


LayersModel::LayersModel(MapItem *mapItem, QObject *parent) :
    TreeProxyModel(parent)
{
    mTreeModel = new TreeModel;
    mMapItem = mapItem;

    initializeModel(mapItem->getMapNode()->getMap());
    setSourceModel(mTreeModel);
    connect(mapItem, &MapItem::layerChanged,[this, mapItem](){
        mTreeModel->removeRows(0,mTreeModel->rowCount());
        initializeModel(mapItem->getMapNode()->getMap());
    });
    connect(mapItem, &MapItem::mapCleared,[this, mapItem](){
        mTreeModel->removeRows(0,mTreeModel->rowCount());
        initializeModel(mapItem->getMapNode()->getMap());
    });
    connect(mapItem->getMapObject(), &MapObject::layerAdded,this ,&LayersModel::onLayerAdded);
    connect(mapItem->getMapObject(), &MapObject::layerRemoved,this ,&LayersModel::onLayerRemoved);
    connect(mapItem->getMapObject(), &MapObject::nodeToLayerAdded,this ,&LayersModel::onNodeToLayerAdded);
    connect(mapItem->getMapObject(), &MapObject::nodeFromLayerRemoved,this ,&LayersModel::onNodeFromLayerRemoved);
    connect(mapItem->getMapObject(), &MapObject::parentLayerChanged,this ,&LayersModel::onParentLayerChanged);
}


void LayersModel::initializeModel(osgEarth::Map *map)
{
    osgEarth::LayerVector layers;
    map->getLayers(layers);
    for(const auto& layer : layers) {
        //        auto parentLayer = mMapItem->getMapObject()->getParentLayer(layer);
        onLayerAdded(layer, nullptr, map->getIndexOfLayer(layer));
    }
}


QHash<int, QByteArray> LayersModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[visibleLayerRole] = "isVisible";
    return hash;
}


void LayersModel::onItemClicked(const QModelIndex &current)
{
    QModelIndex indexSource = mapToSource(current);
    bool visibleRoleData = mTreeModel->data(indexSource,visibleLayerRole).toBool();
    mTreeModel->updateData(indexSource,!visibleRoleData,visibleLayerRole);
    auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(data(current).toString().toStdString());
    auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    if(visibleLayer)
    {
        setLayerVisible(visibleLayer);
    }
    else{
        auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(data(current.parent()).toString().toStdString());
        if(layer){
            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
            if(group){
                auto node = group->getChild(current.row());
                node->setNodeMask(!node->getNodeMask());
            }
        }
    }
}

void LayersModel::onDeleteLayerClicked(const QModelIndex &current)
{
    QModelIndex indexSource = mapToSource(current);
    QStandardItem *item =  mTreeModel->itemFromIndex(indexSource);
    QString layerItem = item->text();
    QString parentLayerItem = item->parent()->text();
    mTreeModel->removeItem(layerItem , parentLayerItem);
    auto parentLayer = mMapItem->getMapNode()->getMap()->getLayerByName(parentLayerItem.toStdString());
    CompositeAnnotationLayer *parentAnnotationlayer = dynamic_cast<CompositeAnnotationLayer*>(parentLayer);
    if (parentAnnotationlayer){
        parentAnnotationlayer->removeLayerByName(layerItem);
    }else{
        auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(data(current).toString().toStdString());
        mMapItem->getMapObject()->removeLayer(layer,parentLayer);
    }


}

void LayersModel::onLayerAdded(osgEarth::Layer *layer , osgEarth::Layer *parentLayer , unsigned index)
{
    QStandardItem *treeItem = new QStandardItem(QString(layer->getName().c_str()));
    treeItem->setData(getLayerVisible(layer),visibleLayerRole);
    if(parentLayer){
        QString parentName = parentLayer->getName().c_str();
        mTreeModel->addItem(treeItem,parentName);
    }else{
        mTreeModel->addItem(treeItem);
    }
    //--------------------------------------
    CompositeAnnotationLayer* compositeLayer = dynamic_cast<CompositeAnnotationLayer*>(layer);
    if (compositeLayer){
        for(int i = 0; i < compositeLayer->getNumChildren(); i++)
            onLayerAdded(compositeLayer->getChild(i), layer, index);
    }
}

void LayersModel::onLayerRemoved(osgEarth::Layer *layer , osgEarth::Layer *parentLayer, unsigned index)
{
    QString treeItem = QString(layer->getName().c_str());
    if(parentLayer){
        QString parentName = parentLayer->getName().c_str();
        mTreeModel->removeItem(treeItem , parentName);
    }else{
        mTreeModel->removeItem(treeItem);
    }
}

void LayersModel::onParentLayerChanged(osgEarth::Layer *layer, osgEarth::Layer *oldParentLayer, osgEarth::Layer *newParentLayer)
{
    QString treeItem = QString(layer->getName().c_str());
    if(oldParentLayer){
        QString oldParent = QString(oldParentLayer->getName().c_str());
        mTreeModel->removeItem(treeItem , oldParent);
    }else{
        mTreeModel->removeItem(treeItem);
    }
    QStandardItem *item = new QStandardItem(treeItem);
    if(newParentLayer){
        QString newParent = QString(newParentLayer->getName().c_str());
        mTreeModel->addItem(item,newParent);
    }else{
        mTreeModel->addItem(item);
    }
}


void LayersModel::onNodeToLayerAdded(osg::Node *node, osgEarth::Layer *layer)
{
    QStandardItem *treeItem = new QStandardItem(QString(node->getName().c_str()));
    treeItem->setData(getLayerVisible(layer),visibleLayerRole);
    QString parentLayer = layer->getName().c_str();
    mTreeModel->addItem(treeItem,parentLayer);
}

void LayersModel::onNodeFromLayerRemoved(osg::Node *node, osgEarth::Layer *layer)
{
    QString treeItem = QString(node->getName().c_str());
    QString parentLayer = QString(layer->getName().c_str());
    mTreeModel->removeItem(treeItem , parentLayer);
}

void LayersModel::setLayerVisible(osgEarth::VisibleLayer *layer)
{
    bool visible = layer->getVisible();
    layer->setVisible(!visible);
}

bool LayersModel::getLayerVisible(osgEarth::Layer *layer) const
{
    auto visLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    bool visible = visLayer->getVisible();
    return visible;
}
