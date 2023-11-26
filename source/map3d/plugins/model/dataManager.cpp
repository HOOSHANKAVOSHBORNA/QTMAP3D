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

    if(!mNodeMap.contains(nodeData->id)){
        flyableNode = new FlyableModelNode(mMapItem, nodeData->url3D, nodeData->url2D);
        flyableNode->setPosition(geoPoint);
        mNodeMap[nodeData->id] = flyableNode;
        flyableNode->setBookmarkManager(mMainWindow->getBookmarkManager());
    }
    else{
        flyableNode = mNodeMap[nodeData->id]->asFlyableModelNode();
        for(auto layer: flyableNode->nodeData()->layers){
            layer->removeChild(flyableNode);
        }
        flyableNode->flyTo(geoPoint, nodeData->speed);
    }

    flyableNode->setName(nodeData->name);
    flyableNode->setNodeData(nodeData);
    //add to layer after set data
    for(auto layer: nodeData->layers){
        layer->addChild(flyableNode);
    }

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
        node->setBookmarkManager(mMainWindow->getBookmarkManager());
    }
    else{
        node = mNodeMap[nodeData->id];
        for(auto layer: node->nodeData()->layers){
            layer->removeChild(node);
        }
    }
    node->setName(nodeData->name);
    node->setPosition(geoPoint);
    node->setNodeData(nodeData);
    //add to layer after set data
    for(auto layer: nodeData->layers){
        layer->addChild(node);
    }
    return node;
}

MoveableModelNode *DataManager::addUpdateMovableNode(NodeData *nodeData)
{
    osgEarth::GeoPoint geoPoint(mMapItem->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
    osg::ref_ptr<MoveableModelNode> movableNode;

    if(!mNodeMap.contains(nodeData->id)){
        movableNode = new MoveableModelNode(mMapItem, nodeData->url3D, nodeData->url2D);
        movableNode->setPosition(geoPoint);
        mNodeMap[nodeData->id] = movableNode;
        movableNode->setBookmarkManager(mMainWindow->getBookmarkManager());
    }
    else{
        movableNode = mNodeMap[nodeData->id]->asMoveableModelNode();
        for(auto layer: movableNode->nodeData()->layers){
            layer->removeChild(movableNode);
        }
        movableNode->moveTo(geoPoint, nodeData->speed);
    }
    movableNode->setName(nodeData->name);
    movableNode->setNodeData(nodeData);
    //add to layer after set data
    for(auto layer: nodeData->layers){
        layer->addChild(movableNode);
    }
    return movableNode;
}

void DataManager::flyableNodeDataReceived(NodeData *nodeData)
{
    if (nodeData->command == "REMOVE"){
        removeNodeData(nodeData);
    } else if (nodeData->command == "UPDATE") {
        addUpdateFlyableNode(nodeData);
    } else {
        addUpdateFlyableNode(nodeData);
    }
}

void DataManager::movableNodeDataReceived(NodeData *nodeData)
{
    if (nodeData->command == "REMOVE"){
        removeNodeData(nodeData);
    } else if (nodeData->command == "UPDATE") {
        addUpdateMovableNode(nodeData);
    }
    else {
        addUpdateMovableNode(nodeData);
    }
}

void DataManager::nodeDataReceived(NodeData *nodeData)
{
    if (nodeData->command == "REMOVE"){
        removeNodeData(nodeData);
    } else if (nodeData->command == "UPDATE") {
        addUpdateNode(nodeData);
    }
    else {
        addUpdateNode(nodeData);
    }
}

//void DataManager::removeFlyableNodeData(NodeData *nodeData)
//{
//    for (auto &layer: nodeData->layers)
//        layer->removeChild(mNodeMap[nodeData->id]);
//    mNodeMap.remove(nodeData->id);
//}

//void DataManager::removeMovableNodeData(NodeData *nodeData)
//{
//    for (auto &layer: nodeData->layers)
//            layer->removeChild(mNodeMap[nodeData->id]);
//    mNodeMap.remove(nodeData->id);
//}

void DataManager::removeNodeData(NodeData *nodeData)
{
    if(mNodeMap.contains(nodeData->id)){
        for(auto layer: mNodeMap[nodeData->id]->nodeData()->layers){
            layer->removeChild(mNodeMap[nodeData->id]);
        }
        mNodeMap[nodeData->id].release();
        mNodeMap.remove(nodeData->id);
    }
}

int DataManager::nodeCount()
{
    return mNodeMap.count();
}

SimpleModelNode *DataManager::getNodeAtIndex(int index)
{
    return mNodeMap.values().at(index);
}
