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

QMap<int, osg::ref_ptr<SimpleModelNode>> *DataManager::getNodeMap()
{
    return &mNodeMap;
}

SimpleModelNode* DataManager::onNodeDataReceived(const NodeData &nodeData)
{
    SimpleModelNode* node{nullptr};
    if (nodeData.command == Command::Remove){
        removeNode(nodeData);
    }
    else {
        node = addUpdateNode(nodeData);
    }
    return node;
}

SimpleModelNode *DataManager::addUpdateNode(const NodeData &nodeData)
{
    osgEarth::GeoPoint geoPoint(mMapItem->getMapObject()->getSRS()->getGeographicSRS(), nodeData.longitude, nodeData.latitude, nodeData.altitude);
    geoPoint.transformInPlace(mMapItem->getMapObject()->getSRS());
    osg::ref_ptr<SimpleModelNode> node;

    if (!mNodeMap.contains(nodeData.id)) {
        // TODO
        // mUniqueNodeNames
        // mUniqueColors
        // mUniqueCategory

        // adding new unique column name
        for (int i = 0; i < nodeData.fieldData.size(); ++i) {
            QString name = nodeData.fieldData.at(i).name;
            bool found = false;
            for (int j = 0; j < mUniqueAddedColumnNames.size(); ++j) {
                if (name == mUniqueAddedColumnNames.at(j)) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                mUniqueAddedColumnNames.append(name);
            }
        }

        if (nodeData.type == NodeType::Movable)
            node = new MoveableModelNode(mMapItem,
                                         nodeData.url3D.toStdString(),
                                         nodeData.url2D.toStdString());
        else if (nodeData.type == NodeType::Flyable)
            node = new FlyableModelNode(mMapItem,
                                        nodeData.url3D.toStdString(),
                                        nodeData.url2D.toStdString());
        else
            node = new SimpleModelNode(mMapItem,
                                       nodeData.url3D.toStdString(),
                                       nodeData.url2D.toStdString());

        node->setPosition(geoPoint);
        mNodeMap[nodeData.id] = node;
        node->setBookmarkManager(mMainWindow->getBookmarkManager());
    } else {
        node = mNodeMap[nodeData.id];
        for (int layerId : node->nodeData().layersId) {
            auto layer = mMapItem->getMapObject()->getLayerByUserId(layerId);
            if (layer)
                layer->removeChild(node);
        }
        if (nodeData.type == NodeType::Movable)
            node->asMoveableModelNode()->moveTo(geoPoint, nodeData.speed);
        else if (nodeData.type == NodeType::Flyable)
            node->asFlyableModelNode()->flyTo(geoPoint, nodeData.speed);
        else
            node->setPosition(geoPoint);
    }
    node->setName(nodeData.name.toStdString());
    //    node->setPosition(geoPoint);
    node->setAttacker(nodeData.isAttacker);
    node->setNodeData(nodeData);
    node->setColor(nodeData.color.toStdString());
    //add to layer after set data
    for(int layerId: node->nodeData().layersId){
        auto layer = mMapItem->getMapObject()->getLayerByUserId(layerId);
        if(layer)
            layer->addChild(node);
    }

    emit nodeDataManagerChanged();

    return node;
}

void DataManager::removeNode(const NodeData &nodeData)
{
    if(mNodeMap.contains(nodeData.id)){
        for(int layerId: nodeData.layersId){
            auto layer = mMapItem->getMapObject()->getLayerByUserId(layerId);
            if(layer)
                layer->removeChild(mNodeMap[nodeData.id]);
        }
        mNodeMap[nodeData.id].release();
        mNodeMap.remove(nodeData.id);
    }
}

QVector<QString> DataManager::uniqueAddedColumnNames() const
{
    return mUniqueAddedColumnNames;
}

void DataManager::setUniqueAddedColumnNames(const QVector<QString> &newUniqueColumnNames)
{
    mUniqueAddedColumnNames = newUniqueColumnNames;
}

int DataManager::nodeCount()
{
    return mNodeMap.count();
}

SimpleModelNode *DataManager::getNodeAtIndex(int index)
{
    return mNodeMap.values().at(index);
}
