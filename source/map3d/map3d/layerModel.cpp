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
    updateLayers(mapItem->getMapNode()->getMap());
    connect(mapItem, &MapItem::layerChanged,[this, mapItem](){
        mTreeModel->removeRows(0,mTreeModel->rowCount());
        updateLayers(mapItem->getMapNode()->getMap());
    });
    connect(mapItem, &MapItem::mapCleared,[this, mapItem](){
        mTreeModel->removeRows(0,mTreeModel->rowCount());
        updateLayers(mapItem->getMapNode()->getMap());
    });
    setSourceModel(mTreeModel);
}



void LayersModel::updateLayers(osgEarth::Map *map)
{
    osgEarth::LayerVector layers;
    map->getLayers(layers);
    for(const auto& layer : layers) {
        std::string parent;
        layer->getUserValue("parent", parent);
        QStandardItem *treeItem = new QStandardItem(QString(layer->getName().c_str()));
        treeItem->setData(true,visibleLayerRole);///////////////
        mTreeModel->addItem(treeItem,QString(parent.c_str()));
        if(layer->getNode() && layer->getNode()->asGroup()){
            auto group = layer->getNode()->asGroup();
            for (int i = 0; i < group->getNumChildren(); ++i) {
                auto child = group->getChild(i);
                QStandardItem *treeItemChild = new QStandardItem(QString(child->getName().c_str()));
                treeItemChild->setData(true,visibleLayerRole);///////////////
                mTreeModel->addItem(treeItemChild , QString(layer->getName().c_str()));
            }
        }
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

void LayersModel::setLayerVisible(osgEarth::VisibleLayer *layer)
{
//    qDebug()<<layer->getName();
    bool visible = layer->getVisible();
    layer->setVisible(!visible);
    auto containerData = layer->getUserDataContainer();
    for(int i = 0; i < containerData->getNumUserObjects(); ++i){
        auto userObject = containerData->getUserObject(i);
        osgEarth::VisibleLayer *childLayer = dynamic_cast<osgEarth::VisibleLayer*>(userObject);
        if(childLayer){
            qDebug()<<childLayer->getName();
            setLayerVisible(childLayer);
        }
    }
}

bool LayersModel::getLayerVisible(QModelIndex itemIndex) const
{
    auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(data(itemIndex).toString().toStdString());
    auto visLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    bool visible = visLayer->getVisible();
//    mTreeModel->setData(itemIndex,visible,visibleLayerRole);////////////
    return visible;
}

//void LayersModel::addChildItem(osg::Group *parentGroup, QStandardItem *parentItem)
//{
//    for(int i = 0; i < parentGroup->getNumChildren(); ++i){
//        osg::Node *node = parentGroup->getChild(i);
//        QStandardItem *treeItem = new QStandardItem(QString(node->getName().c_str()));
//        mTreeModel->addItem(treeItem , parentItem->text());

//        auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(node->getName());
//        if(layer && layer->getNode() && layer->getNode()->asGroup())
//            addChildItem(layer->getNode()->asGroup(), treeItem);
//    }
//}

//void LayersModel::addChildItem(osgEarth::Layer *layer, QStandardItem *parentItem)
//{
//    QStandardItem *treeItem = new QStandardItem(QString(layer->getName().c_str()));
//    mTreeModel->addItem(treeItem , parentItem->text());
//    auto containerData = layer->getUserDataContainer();
//    for(int i = 0; i < containerData->getNumUserObjects(); ++i){
//        auto userObject = containerData->getUserObject(i);
//        osgEarth::Layer *childLayer = dynamic_cast<osgEarth::Layer*>(userObject);
//        if(childLayer)
//            addChildItem(childLayer, treeItem);
//    }
//    if(layer->getNode() && layer->getNode()->asGroup()){
//        auto group = layer->getNode()->asGroup();
//        for (int i = 0; i < group->getNumChildren(); ++i) {
//            auto child = group->getChild(i);
//            QStandardItem *treeItemChild = new QStandardItem(QString(child->getName().c_str()));
//            mTreeModel->addItem(treeItemChild , QString(group->getName().c_str()));
//        }
//    }
//}
