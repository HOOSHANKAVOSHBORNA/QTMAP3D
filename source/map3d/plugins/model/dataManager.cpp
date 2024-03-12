#include "dataManager.h"
#include "flyableModelNode.h"
#include "moveableModelNode.h"
#include "mainwindow.h"
#include "utility.h"

DataManager::DataManager(QQmlEngine *engine, MapItem *mapItem, MainWindow *mainWindow, QObject *parent)
    :QObject(parent),
    mMapItem(mapItem),
    mMainWindow(mainWindow)
{
    mQmlEngine = engine;
    mFilterManager = new FilterManager(this);
}

DataManager::~DataManager()
{
    qDebug()<<"~DataManager";
}

QMap<int, osg::ref_ptr<SimpleModelNode> > *DataManager::getNodeMap()
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
        // TODO: setup filter needed data mUniqueColorss
//        NodeData movedNode(nodeData);

        // adding new uniuqe category name
        for (int i = 0; i < nodeData.fieldData.size(); ++i) {
            QString category = nodeData.fieldData.at(i).category;
            bool found = false;
            if (!mUniqueTabNames.contains(category)) {
//                emit tabNameAppendingStart(QModelIndex(),
//                                           mUniqueTabNames.size(),
//                                           mUniqueTabNames.size());
                mUniqueTabNames.append(category);
//                emit tabNameAppendingEnd();
            }
        }

        // adding new unique column name
        for (int i = 0; i < nodeData.fieldData.size(); ++i) {
            QString name = nodeData.fieldData.at(i).name;
            if (!mUniqueAddedColumnNames.contains(name)) {
//                emit columnAppendigStart(QModelIndex(),
//                                         mUniqueAddedColumnNames.size(),
//                                         mUniqueAddedColumnNames.size());
                mUniqueAddedColumnNames.append(name);
                mColumnToCategory.insert(name, nodeData.fieldData.at(i).category);
//                emit columnAppendigEnd();
            }
        }

        //add new Category Tag Names
        QString categoryTag = nodeData.category;
        if (!mCategoryTagNames.contains(categoryTag)) {
            mCategoryTagNames.append(categoryTag);
        }

        if (nodeData.type == NodeType::Movable)
            node = new MoveableModelNode(mQmlEngine,
                                         mMapItem,
                                         nodeData.url3D.toStdString(),
                                         nodeData.url2D.toStdString());
        else if (nodeData.type == NodeType::Flyable)
            node = new FlyableModelNode(mQmlEngine,
                                        mMapItem,
                                        nodeData.url3D.toStdString(),
                                        nodeData.url2D.toStdString());
        else
            node = new SimpleModelNode(mQmlEngine,
                                       mMapItem,
                                       nodeData.url3D.toStdString(),
                                       nodeData.url2D.toStdString());

        node->setPosition(geoPoint);

//        emit nodeAppendingStart(QModelIndex(), nodeCount(), nodeCount());
        mNodeMap[nodeData.id] = node;
//        emit nodeAppendingEnd();

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

//        emit nodeUpdated(getNodeIndexById(nodeData.id));
    }
    node->setName(nodeData.name.toStdString());
    //    node->setPosition(geoPoint);
    node->setAttacker(nodeData.isAttacker);
    node->setNodeData(nodeData);
    node->setColor(Utility::qColor2osgEarthColor(QColor(nodeData.color)));
    //add to layer after set data
    for(int layerId: node->nodeData().layersId){
        auto layer = mMapItem->getMapObject()->getLayerByUserId(layerId);
        if(layer)
            layer->addChild(node);
    }

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

//        int nodeDataId = getNodeIndexById(nodeData.id);
//        nodeRemovingStart(QModelIndex(), nodeDataId, nodeDataId);
//        mNodeMap[nodeData.id].release();
        emit nodeRemoved(mNodeMap[nodeData.id]);
        mNodeMap.remove(nodeData.id);
//        nodeRemovingEnd();

    }
}

MapItem *DataManager::mapItem() const
{
    return mMapItem;
}

FilterManager *DataManager::filterManager() const
{
    return mFilterManager;
}

QVector<QString> DataManager::essentialColumnNames() const
{
    return mEssentialColumnNames;
}

//void DataManager::setEssentialColumnNames(const QVector<QString> &newEssentialColumnNames)
//{
//    mEssentialColumnNames = newEssentialColumnNames;
//}

QVector<QString> DataManager::categoryTagNames() const
{
    return mCategoryTagNames;
}

//void DataManager::setCategoryTagNames(const QVector<QString> &newCategoryTagNames)
//{
//    mCategoryTagNames = newCategoryTagNames;
//}

QVector<QString> DataManager::fixedColumnNames() const
{
    return mFixedColumnNames;
}

//void DataManager::setFixedColumnNames(const QVector<QString> &newFixedColumnNames)
//{
//    mFixedColumnNames = newFixedColumnNames;
//}

QMap<QString, QString> DataManager::columnToCategory() const
{
    return mColumnToCategory;
}

//void DataManager::setColumnToCategory(const QMap<QString, QString> &newColumnToCategory)
//{
//    mColumnToCategory = newColumnToCategory;
//}

int DataManager::getNodeIndexById(int id)
{
    for (int i = 0; i < mNodeMap.size(); ++i) {
        if (mNodeMap.keys().at(i) == id) {
            return i;
        }
    }

    return -1;
}

QVector<QString> DataManager::uniqueCategoryNames()
{
    return mUniqueTabNames;
}

QVector<QString> *DataManager::getUniqueCategoryNames()
{
    return &mUniqueTabNames;
}

//void DataManager::setUniqueCategoryNames(const QVector<QString> &newUniqueCategoryNames)
//{
//    mUniqueTabNames = newUniqueCategoryNames;
//}

QVector<QString> DataManager::uniqueAddedColumnNames() const
{
    return mUniqueAddedColumnNames;
}

//void DataManager::setUniqueAddedColumnNames(const QVector<QString> &newUniqueColumnNames)
//{
//    mUniqueAddedColumnNames = newUniqueColumnNames;
//}

int DataManager::nodeCount()
{
    return mNodeMap.count();
}

SimpleModelNode *DataManager::getNodeAtIndex(int index)
{
    return mNodeMap.values().at(index);
}
