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

    int getNodeIndexById(int id);

    QVector<QString> fixedColumnNames() const;
    void setFixedColumnNames(const QVector<QString> &newFixedColumnNames);

    QVector<QString> categoryTagNames() const;
    void setCategoryTagNames(const QVector<QString> &newCategoryTagNames);

signals:
    void nodeAppendingStart(QModelIndex, int, int);
    void nodeAppendingEnd();

    void nodeUpdated(int index);

    void columnAppendigStart(QModelIndex, int, int);
    void columnAppendigEnd();

    void categoryTagAppendingStart(QModelIndex, int, int);
    void categoryTagAppendingEnd();

    void tabNameAppendingStart(QModelIndex, int, int);
    void tabNameAppendingEnd();

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
    QVector<QString> mFixedColumnNames = {"Color", "Icon", "Name", "Type"};
    QVector<QString> mUniqueAddedColumnNames;
    QVector<QString> mUniqueTabNames;
    QVector<QString> mCategoryTagNames = {"All"};
    QMap<QString, QString> mColumnToCategory = {{"Color", "Fixed"},
                                                {"Icon", "Fixed"},
                                                {"Name", "Fixed"},
                                                {"Type", "Fixed"}};
    NodeData* mNodeData;
    MainWindow *mMainWindow;
};

#endif // DATAMANAGER_H
