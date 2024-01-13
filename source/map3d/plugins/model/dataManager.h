#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "mainwindow.h"
#include "simpleModelNode.h"

class DataManager : public QObject
{
    Q_OBJECT
public:
    DataManager(MapItem *mapItem, MainWindow *mainWindow);
    ~DataManager();
//    void removeFlyableNodeData(NodeData *nodeData);
//    void removeMovableNodeData(NodeData *nodeData);

    int nodeCount();
    SimpleModelNode* getNodeAtIndex(int index);

//    FlyableModelNode* addUpdateFlyableNode(NodeData *nodeData);

//    MoveableModelNode* addUpdateMovableNode(NodeData *nodeData);
    QMap<int, osg::ref_ptr<SimpleModelNode>> *getNodeMap();

public slots:
    SimpleModelNode* onNodeDataReceived(const NodeData &nodeData);

private:
    SimpleModelNode* addUpdateNode(const NodeData &nodeData);
    void removeNode(const NodeData &nodeData);

private:
    MapItem* mMapItem;

//    QMap<int, osg::ref_ptr<FlyableModelNode>> mFlyableNodeMap;
    QMap<int, osg::ref_ptr<SimpleModelNode>> mNodeMap;
//    QMap<int, osg::ref_ptr<MoveableModelNode>> mMovableNodeMap;
    NodeData* mNodeData;
    MainWindow *mMainWindow;
};

#endif // DATAMANAGER_H
