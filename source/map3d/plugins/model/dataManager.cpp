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

//FlyableModelNode *DataManager::addUpdateFlyableNode(NodeData *nodeData)
//{
//    osgEarth::GeoPoint geoPoint(mMapItem->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
//    osg::ref_ptr<FlyableModelNode> flyableNode;

//    if(!mNodeMap.contains(nodeData->id)){
////        flyableNode = new FlyableModelNode(mMapItem, nodeData->url3D, nodeData->url2D);
//        flyableNode->setPosition(geoPoint);
//        mNodeMap[nodeData->id] = flyableNode;
//        flyableNode->setBookmarkManager(mMainWindow->getBookmarkManager());
//    }
//    else{
//        flyableNode = mNodeMap[nodeData->id]->asFlyableModelNode();
////        for(auto layer: flyableNode->nodeData()->layers){
////            layer->removeChild(flyableNode);
////        }
//        flyableNode->flyTo(geoPoint, nodeData->speed);
//    }

////    flyableNode->setName(nodeData->name);
//    flyableNode->setAttacker(nodeData->isAttacker);
//    flyableNode->setNodeData(nodeData);
//    //add to layer after set data
////    for(auto layer: nodeData->layers){
////        layer->addChild(flyableNode);
////    }

//    return flyableNode;
//}

SimpleModelNode *DataManager::addUpdateNode(const NodeData &nodeData)
{
    osgEarth::GeoPoint geoPoint(mMapItem->getMapObject()->getSRS()->getGeographicSRS(), nodeData.longitude, nodeData.latitude, nodeData.altitude);
    geoPoint.transformInPlace(mMapItem->getMapObject()->getSRS());
    osg::ref_ptr<SimpleModelNode> node;

    if(!mNodeMap.contains(nodeData.id)){

        if(nodeData.type == NodeType::Movable)
            node = new MoveableModelNode(mMapItem, nodeData.url3D.toStdString(), nodeData.url2D.toStdString());
        else if(nodeData.type == NodeType::Flyable)
            node = new FlyableModelNode(mMapItem, nodeData.url3D.toStdString(), nodeData.url2D.toStdString());
        else
             node = new SimpleModelNode(mMapItem, nodeData.url3D.toStdString(), nodeData.url2D.toStdString());

        node->setPosition(geoPoint);
        mNodeMap[nodeData.id] = node;
        node->setBookmarkManager(mMainWindow->getBookmarkManager());
    }
    else{
        node = mNodeMap[nodeData.id];
        for(int layerId: node->nodeData().layersId){
            auto layer = mMapItem->getMapObject()->getLayerByUserId(layerId);
            if(layer)
                layer->removeChild(node);
        }
        if(nodeData.type == NodeType::Movable)
            node->asMoveableModelNode()->moveTo(geoPoint, nodeData.speed);
        else if(nodeData.type == NodeType::Flyable)
            node->asFlyableModelNode()->flyTo(geoPoint, nodeData.speed);
        else
            node->setPosition(geoPoint);
    }
    node->setName(nodeData.name.toStdString());
//    node->setPosition(geoPoint);
    node->setAttacker(nodeData.isAttacker);
    node->setNodeData(nodeData);
    //add to layer after set data
    for(int layerId: node->nodeData().layersId){
        auto layer = mMapItem->getMapObject()->getLayerByUserId(layerId);
        if(layer)
            layer->addChild(node);
    }
    return node;
}

//MoveableModelNode *DataManager::addUpdateMovableNode(NodeData *nodeData)
//{
//    osgEarth::GeoPoint geoPoint(mMapItem->getMapObject()->getSRS(), nodeData->longitude, nodeData->latitude, nodeData->altitude);
//    osg::ref_ptr<MoveableModelNode> movableNode;

//    if(!mNodeMap.contains(nodeData->id)){
////        movableNode = new MoveableModelNode(mMapItem, nodeData->url3D, nodeData->url2D);
//        movableNode->setPosition(geoPoint);
//        mNodeMap[nodeData->id] = movableNode;
//        movableNode->setBookmarkManager(mMainWindow->getBookmarkManager());
//    }
//    else{
//        movableNode = mNodeMap[nodeData->id]->asMoveableModelNode();
////        for(auto layer: movableNode->nodeData()->layers){
////            layer->removeChild(movableNode);
////        }
//        movableNode->moveTo(geoPoint, nodeData->speed);
//    }
////    movableNode->setName(nodeData->name);
//    movableNode->setNodeData(nodeData);
//    movableNode->setAttacker(nodeData->isAttacker);
//    //add to layer after set data
////    for(auto layer: nodeData->layers){
////        layer->addChild(movableNode);
////    }
//    return movableNode;
//}

QMap<int, osg::ref_ptr<SimpleModelNode>> *DataManager::getNodeMap()
{
    return &mNodeMap;
}

void DataManager::nodeDataReceived(const NodeData &nodeData)
{
    if (nodeData.command == Command::Remove){
        removeNode(nodeData);
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

void DataManager::removeNode(const NodeData &nodeData)
{
    if(mNodeMap.contains(nodeData.id)){
//        for(auto layer: mNodeMap[nodeData->id]->nodeData()->layers){
//            layer->removeChild(mNodeMap[nodeData->id]);
//        }
        mNodeMap[nodeData.id].release();
        mNodeMap.remove(nodeData.id);
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
