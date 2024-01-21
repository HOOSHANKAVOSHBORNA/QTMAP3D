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

    QVector<QString> uniqueAddedColumnNames() const;
    void setUniqueAddedColumnNames(const QVector<QString> &newUniqueColumnNames);

    QVector<QString> uniqueCategoryNames();
    void setUniqueCategoryNames(const QVector<QString> &newUniqueCategoryNames);
    QVector<QString> *getUniqueCategoryNames();

    QMap<QString, QString> columnToCategory() const;
    void setColumnToCategory(const QMap<QString, QString> &newColumnToCategory);

signals:
    void nodeDataManagerChanged();

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
    QVector<QString> mUniqueAddedColumnNames;
    QVector<QString> mUniqueCategoryNames;
    QMap<QString, QString> mColumnToCategory = {{"Color", "Fixed"},
                                                {"Icon", "Fixed"},
                                                {"Name", "Fixed"},
                                                {"Type", "Fixed"}};
    NodeData* mNodeData;
    MainWindow *mMainWindow;
};

#endif // DATAMANAGER_H
