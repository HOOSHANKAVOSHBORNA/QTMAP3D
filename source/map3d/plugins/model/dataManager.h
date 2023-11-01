#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "simpleModelNode.h"

class DataManager : public QObject
{
    Q_OBJECT
public:
    DataManager(MapItem *mapItem, MainWindow *mainWindow);
    ~DataManager();
    void removeFlyableNodeData(NodeData *nodeData);
    void removeMovableNodeData(NodeData *nodeData);
    void removeNodeData(NodeData *nodeData);
public slots:
    FlyableModelNode* addUpdateFlyableNode(NodeData *nodeData);
    SimpleModelNode* addUpdateNode(NodeData *nodeData);
    MoveableModelNode* addUpdateMovableNode(NodeData *nodeData);

private:
    MapItem* mMapItem;

    QMap<int, osg::ref_ptr<FlyableModelNode>> mFlyableNodeMap;
    QMap<int, osg::ref_ptr<SimpleModelNode>> mNodeMap;
    QMap<int, osg::ref_ptr<MoveableModelNode>> mMovableNodeMap;
    NodeData* mNodeData;
    MainWindow *mMainWindow;
    static inline int mCount = 0;
};

#endif // DATAMANAGER_H
