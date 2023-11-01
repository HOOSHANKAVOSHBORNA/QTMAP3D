#include "dataManager.h"
#include "flyableModelNode.h"
#include "moveableModelNode.h"
#include "mainwindow.h"
DataManager::DataManager(MapItem *mapItem, MainWindow *mainWindow)
    : mMapItem(mapItem), mMainWindow(mainWindow)
{

}

DataManager::~DataManager()
{

}

FlyableModelNode *DataManager::addUpdateFlyableNode(NodeData *nodeData)
{
    osgEarth::GeoPoint geoPoint(mMapItem->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
    osg::ref_ptr<FlyableModelNode> flyableNode;

    if(!mFlyableNodeMap.contains(nodeData->id)){
        flyableNode = new FlyableModelNode(mMapItem, nodeData->url3D, nodeData->url2D);
        flyableNode->setPosition(geoPoint);
        mFlyableNodeMap[nodeData->id] = flyableNode;
        flyableNode->setBookmark(mMainWindow->getBookmarkManager());
    }
    else{
        flyableNode = mFlyableNodeMap[nodeData->id];
        for(auto layer: flyableNode->nodeData()->layers){
            layer->removeChild(flyableNode);
        }
        flyableNode->flyTo(geoPoint, nodeData->speed);
    }
    for(auto layer: nodeData->layers){
        layer->addChild(flyableNode);
    }
    flyableNode->setName(nodeData->name);
    flyableNode->setNodeData(nodeData);

    return flyableNode;
}

SimpleModelNode *DataManager::addUpdateNode(NodeData *nodeData)
{
    osgEarth::GeoPoint geoPoint(mMapItem->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
    osg::ref_ptr<SimpleModelNode> node;

    if(!mNodeMap.contains(nodeData->id)){
        node = new SimpleModelNode(mMapItem, nodeData->url3D, nodeData->url2D);
        node->setPosition(geoPoint);
        mNodeMap[nodeData->id] = node;
        node->setBookmark(mMainWindow->getBookmarkManager());
    }
    else{
        node = mNodeMap[nodeData->id];
        for(auto layer: node->nodeData()->layers){
            layer->removeChild(node);
        }
    }
    for(auto layer: nodeData->layers){
        layer->addChild(node);
    }
    node->setName(nodeData->name);
    node->setPosition(geoPoint);
    node->setNodeData(nodeData);
    return node;
}

MoveableModelNode *DataManager::addUpdateMovableNode(NodeData *nodeData)
{
    osgEarth::GeoPoint geoPoint(mMapItem->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
    osg::ref_ptr<MoveableModelNode> movableNode;

    if(!mMovableNodeMap.contains(nodeData->id)){
        movableNode = new MoveableModelNode(mMapItem, nodeData->url3D, nodeData->url2D);
        movableNode->setPosition(geoPoint);
        mMovableNodeMap[nodeData->id] = movableNode;
        movableNode->setBookmark(mMainWindow->getBookmarkManager());
    }
    else{
        movableNode = mMovableNodeMap[nodeData->id];
        for(auto layer: movableNode->nodeData()->layers){
            layer->removeChild(movableNode);
        }
        movableNode->moveTo(geoPoint, nodeData->speed);
    }
    for(auto layer: nodeData->layers){
        layer->addChild(movableNode);
    }
    movableNode->setName(nodeData->name);
    movableNode->setNodeData(nodeData);
    return movableNode;
}

void DataManager::removeFlyableNodeData(NodeData *nodeData)
{
    for (auto &layer: nodeData->layers)
        layer->removeChild(mFlyableNodeMap[nodeData->id]);
    mFlyableNodeMap.remove(nodeData->id);
}

void DataManager::removeMovableNodeData(NodeData *nodeData)
{
    for (auto &layer: nodeData->layers)
        if (mMovableNodeMap.contains(nodeData->id))
            layer->removeChild(mMovableNodeMap[nodeData->id]);
    mMovableNodeMap.remove(nodeData->id);
}

void DataManager::removeNodeData(NodeData *nodeData)
{
    for (auto &layer: nodeData->layers)
        layer->removeChild(mNodeMap[nodeData->id]);
    mNodeMap.remove(nodeData->id);
}
