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
        auto parentLayer = mMapItem->getMapObject()->getParentLayer(layer);
        onLayerAdded(layer, parentLayer, map->getIndexOfLayer(layer));
//        QStandardItem *treeItem = new QStandardItem(QString(layer->getName().c_str()));
//        auto parentLayer = mMapItem->getMapObject()->getParentLayer(layer);
//        if(parentLayer)
//            mTreeModel->addItem(treeItem,QString(parentLayer->getName().c_str()));
//        else
//            mTreeModel->addItem(treeItem,"");
//        treeItem->setData(getLayerVisible(layer),visibleLayerRole);
//        if(layer->getNode() && layer->getNode()->asGroup()){
//            auto group = layer->getNode()->asGroup();
//            for (int i = 0; i < group->getNumChildren(); ++i) {
//                auto child = group->getChild(i);
//                QStandardItem *treeItemChild = new QStandardItem(QString(child->getName().c_str()));
//                treeItemChild->setData(true,visibleLayerRole);///////////////
//                mTreeModel->addItem(treeItemChild , QString(layer->getName().c_str()));
//            }
//        }
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
    bool visibleRoleSet = mTreeModel->data(indexSource,visibleLayerRole).toBool();
    mTreeModel->setData(indexSource,!visibleRoleSet,visibleLayerRole);
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

void LayersModel::onLayerAdded(osgEarth::Layer *layer , osgEarth::Layer *parentLayer , unsigned index)
{
    qDebug()<<"addLayer:"<<QString(layer->getName().c_str());

    QStandardItem *treeItem = new QStandardItem(QString(layer->getName().c_str()));
    treeItem->setData(getLayerVisible(layer),visibleLayerRole);
    if(parentLayer){
        QString parentName = parentLayer->getName().c_str();
        mTreeModel->addItem(treeItem,parentName);
        qDebug()<<"addLayer parent:"<<parentName;
    }else{
        mTreeModel->addItem(treeItem);
    }
}

void LayersModel::onLayerRemoved(osgEarth::Layer *layer , osgEarth::Layer *parentLayer, unsigned index)
{
    qDebug()<<"removeLayer:"<<QString(layer->getName().c_str());
    QString treeItem = QString(layer->getName().c_str());
    if(parentLayer){
        QString parentName = parentLayer->getName().c_str();
        mTreeModel->removeItem(treeItem , parentName);
        qDebug()<<"removeLayer parent:"<<parentName;
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
    auto containerData = layer->getUserDataContainer();
    if(containerData){
        for(int i = 0; i < containerData->getNumUserObjects(); ++i){
            auto userObject = containerData->getUserObject(i);
            osgEarth::VisibleLayer *childLayer = dynamic_cast<osgEarth::VisibleLayer*>(userObject);
            if(childLayer){
                qDebug()<<childLayer->getName();
                setLayerVisible(childLayer);
            }
        }
    }
}

bool LayersModel::getLayerVisible(osgEarth::Layer *layer) const
{
    auto visLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    bool visible = visLayer->getVisible();
    return visible;
}
