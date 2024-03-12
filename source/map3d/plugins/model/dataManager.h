#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "filterManager.h"
#include "mainwindow.h"
#include "simpleModelNode.h"

class DataManager : public QObject
{
    Q_OBJECT
public:
    DataManager(QQmlEngine *engine, MapItem *mapItem, MainWindow *mainWindow, QObject *parent = nullptr);
    ~DataManager();

    int nodeCount();
    SimpleModelNode* getNodeAtIndex(int index);
    QMap<int, osg::ref_ptr<SimpleModelNode>> *getNodeMap();

    QVector<QString> uniqueAddedColumnNames() const;
//    void setUniqueAddedColumnNames(const QVector<QString> &newUniqueColumnNames);

    QVector<QString> uniqueCategoryNames();
//    void setUniqueCategoryNames(const QVector<QString> &newUniqueCategoryNames);
    QVector<QString> *getUniqueCategoryNames();

    QMap<QString, QString> columnToCategory() const;
//    void setColumnToCategory(const QMap<QString, QString> &newColumnToCategory);

    int getNodeIndexById(int id);

    QVector<QString> fixedColumnNames() const;
//    void setFixedColumnNames(const QVector<QString> &newFixedColumnNames);

    QVector<QString> categoryTagNames() const;
//    void setCategoryTagNames(const QVector<QString> &newCategoryTagNames);

    QVector<QString> essentialColumnNames() const;
//    void setEssentialColumnNames(const QVector<QString> &newEssentialColumnNames);

    MapItem *mapItem() const;
//    void setMapItem(MapItem *newMapItem);

    FilterManager *filterManager() const;
//    void setFilterManager(FilterManager *newFilterManager);

signals:
    void nodeAppendingStart(QModelIndex, int, int);
    void nodeAppendingEnd();

    void nodeRemovingStart(QModelIndex, int, int);
    void nodeRemovingEnd();

    void nodeUpdated(int index);

    void columnAppendigStart(QModelIndex, int, int);
    void columnAppendigEnd();

    void categoryTagAppendingStart(QModelIndex, int, int);
    void categoryTagAppendingEnd();

    void tabNameAppendingStart(QModelIndex, int, int);
    void tabNameAppendingEnd();

    void nodeRemoved(SimpleModelNode*);
public slots:
    SimpleModelNode* onNodeDataReceived(const NodeData &nodeData);

private:
    SimpleModelNode* addUpdateNode(const NodeData &nodeData);
    void removeNode(const NodeData &nodeData);

private:
    QQmlEngine *mQmlEngine = nullptr;
    MapItem* mMapItem;

    QMap<int, osg::ref_ptr<SimpleModelNode>> mNodeMap;
    QVector<QString> mUniqueTabNames = {"Essential"};
    QVector<QString> mCategoryTagNames = {"All"};
    QVector<QString> mFixedColumnNames = {"Color", "Icon", "Id", "Name"};
    QVector<QString> mEssentialColumnNames
        = {"Type", "IsAttacker", "Latitude", "Longitude", "Altitude", "Speed"};
    QVector<QString> mUniqueAddedColumnNames;
    QMap<QString, QString> mColumnToCategory;
    NodeData* mNodeData;
    MainWindow *mMainWindow;
    FilterManager *mFilterManager = nullptr;
};

#endif // DATAMANAGER_H
