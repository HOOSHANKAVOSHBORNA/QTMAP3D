#ifndef NodeList_H
#define NodeList_H

#include <QAbstractTableModel>
#include <QItemSelectionModel>
#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>
#include <QRandomGenerator>
#include <QSortFilterProxyModel>

#include "dataManager.h"
#include "mapControllerItem.h"

class NodeProxyModel;
class NodeList;
class NodeListModel;

// ---------------------------------------- CategoryTagModel
class CategoryTagModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CategoryTagModel(DataManager *dataManager, QObject *parent = nullptr);
    ~CategoryTagModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void beginEndResetModel();

private:
    DataManager *mDataManager{nullptr};
};

// ---------------------------------------- CategoryTabbarModel
class CategoryTabbarModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CategoryTabbarModel(DataManager *dataManager, QObject *parent = nullptr);
    ~CategoryTabbarModel();

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void beginEndResetModel();

private:
    DataManager *mDataManager{nullptr};
};

//-----------------------------------------NodeListModel------------------------------------
class NodeListModel : public QAbstractTableModel
{
    Q_OBJECT

protected:
    friend class NodeList;

public:
    explicit NodeListModel(DataManager *dataManager, QObject *parent = nullptr);
    ~NodeListModel();
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void selectionRow(int rowCount, int idxRow);
    Q_INVOKABLE QItemSelectionModel *selectModel();

    //    Q_INVOKABLE void attacker(QString name);
    //    Q_INVOKABLE void setChangeModel(QString checkModel);

    DataManager *dataManager() const;

public slots:
    void onNodeUpdated(int index);
    void beginEndResetModel();

private:
    DataManager *mDataManager{nullptr};
    QItemSelectionModel *mSelectionModel{nullptr};
};

//--------------------------------------NodeProxyModel-------------------------------------
class NodeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(CategoryTagModel *categoryTagModel READ categoryTagModel WRITE setCategoryTagModel
                   NOTIFY categoryTagModelChanged FINAL)
    Q_PROPERTY(CategoryTabbarModel *tabbarModel READ tabbarModel WRITE setTabbarModel NOTIFY
                   tabbarModelChanged FINAL)

protected:
    friend class NodeList;

public:
    explicit NodeProxyModel(DataManager *dataManager, QObject *parent = nullptr);
    ~NodeProxyModel();

    void invalidateRowFilterInvoker();
    void invalidateColumnFilterInvoker();

    Q_INVOKABLE void sortTable(int column);

    Q_INVOKABLE void setFilterColumn(QString name);
    Q_INVOKABLE void setFilterCategoryTag(QString name);

    Q_INVOKABLE void selectionRow(int row, int column);
    Q_INVOKABLE QItemSelectionModel *selectModel();
    //    Q_INVOKABLE void attacker(QString name);
    //    Q_INVOKABLE void setChangeModel(QString checkModel);

    Q_INVOKABLE CategoryTabbarModel *tabbarModel() const;
    void setTabbarModel(CategoryTabbarModel *newTabbarModel);

    CategoryTagModel *categoryTagModel() const;
    void setCategoryTagModel(CategoryTagModel *newCategoryTagModel);

    Q_INVOKABLE void goToPosition(int index);
    Q_INVOKABLE void trackPosition(int index);

    Q_INVOKABLE QString filterSearch() const;
    Q_INVOKABLE void setFilterSearch(const QString &newFilterSearch);

signals:
    void categoryTagModelChanged();
    void tabbarModelChanged();

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QString mFilterSearch;
    QString mFilterCategoryTag = "All";
    QString mFilterColumn = "Essential";

    bool mAscending = true; // ascending or descending sort

    NodeListModel *nodeListModel = nullptr;
    CategoryTagModel *mCategoryTagModel = nullptr;
    CategoryTabbarModel *mTabbarModel = nullptr;
    DataManager *mDataManager = nullptr;
};

//--------------------------------NodeList-----------------------
class NodeList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *qmlItem READ qmlItem WRITE setQmlItem NOTIFY qmlItemChanged FINAL)

public:
    explicit NodeList(QQmlEngine *engine, MapControllerItem *mapItem, DataManager *dataManager, QObject *parent = nullptr);

    QQuickItem *qmlItem() const;
    void setQmlItem(QQuickItem *newQmlItem);

    NodeProxyModel *proxyModel() const;
    void setProxyModel(NodeProxyModel *newProxyModel);

signals:
    void qmlItemChanged();

private:
    void createQml();

private:
    QQmlEngine *mQmlEngine = nullptr;
    MapControllerItem *mMapItem = nullptr;
    QQuickItem *mQmlItem = nullptr;
    DataManager *mDataManager = nullptr;
    NodeProxyModel *mProxyModel = nullptr;
};

#endif // NodeList_H
