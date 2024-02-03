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

// ---------------------------------------- Tiny Models
class CategoryTagModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CategoryTagModel(DataManager *dataManager);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void beginEndResetModel();

private:
    DataManager *mDataManager;
};

class CategoryTabbarModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CategoryTabbarModel(DataManager *dataManager);

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void beginEndResetModel();

private:
    DataManager *mDataManager;
};

//-----------------------------------------NodeListModel------------------------------------
class NodeListModel : public QAbstractTableModel
{
    Q_OBJECT

protected:
    friend class NodeList;

public:
    explicit NodeListModel(DataManager *dataManager);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void selectionRow(int rowCount, int idxRow);
    Q_INVOKABLE QItemSelectionModel *selectModel();

    // TODO: write select attacker and show targets
    QVector<NodeData> *DataAttacker = new QVector<NodeData>;
    //    Q_INVOKABLE void attacker(QString name);
    //    Q_INVOKABLE void setChangeModel(QString checkModel);

    DataManager *dataManager() const;
    void setDataManager(DataManager *newDataManager);

public slots:
    void onNodeUpated(int index);
    void beginEndResetModel();

private:
    DataManager *mDataManager;
    QItemSelectionModel *selectionModel;
    QString modelType;
};

//--------------------------------------NodeProxyModel-------------------------------------
class NodeProxyModel : public QSortFilterProxyModel
{
    Q_PROPERTY(CategoryTabbarModel *tabbarModel READ tabbarModel WRITE setTabbarModel NOTIFY
                   tabbarModelChanged FINAL)
    Q_PROPERTY(CategoryTagModel *categoryTagModel READ categoryTagModel WRITE setCategoryTagModel
                   NOTIFY categoryTagModelChanged FINAL)

    struct FilterTag
    {
        QString name;
        QString value;
    };

    struct FilterTag1
    {
        QString name;
        QString value;
    };
    struct FilterTag2
    {
        QString name;
        int valueFrom;
        int valueTo;
    };
    struct FilterTag3
    {
        QString name;
        int value;
        QString mark;
    };

    Q_OBJECT
    //Q_PROPERTY(QStringList comboItem READ comboItem WRITE setComboItem NOTIFY comboItemChanged FINAL)
    //Q_PROPERTY(QStringList comboItemList READ comboItemList WRITE setComboItemList NOTIFY comboItemListChanged FINAL)

protected:
    friend class NodeList;

public:
    explicit NodeProxyModel(QObject *parent = nullptr);

    void invalidateRowFilterInvoker();
    void invalidateColumnFilterInvoker();

    Q_INVOKABLE void sortTable(int column);
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;

    Q_INVOKABLE void filterString(QString search, QString value);
    Q_INVOKABLE void filterStringColumn(QString tabName);

    Q_INVOKABLE QList<QString> getDataComboBox();
    Q_INVOKABLE QList<QString> getDataComboBoxInt();
    //    Q_INVOKABLE QList<QString> getColorFilter();
    Q_INVOKABLE void addTagColor(QString name, QString value);
    Q_INVOKABLE void addTag1(QString name, QString value);
    Q_INVOKABLE void addTag2(QString name, int value1, int value2);
    Q_INVOKABLE void addTag3(QString name, int value, QString mark);

    Q_INVOKABLE void removeTag(QString filterSearch, QString name, QString value);
    Q_INVOKABLE void filterCategoryTag(QString name);

    //QStringList comboItem() const;
    //void setComboItem(const QStringList &newComboItem);

    //QStringList comboItemList() const;
    //void setComboItemList(const QStringList &newComboItemList);

    Q_INVOKABLE QStringList filterCombo(QString text, QString nameFilter);

    // dynamic_cast to NodeListModel
    Q_INVOKABLE void selectionRow(int Row, int Column);
    Q_INVOKABLE QItemSelectionModel *selectModel();
    //    Q_INVOKABLE void attacker(QString name);
    //    Q_INVOKABLE void setChangeModel(QString checkModel);
    //    Q_INVOKABLE QList<QString> getFilterData();

    Q_INVOKABLE CategoryTabbarModel *tabbarModel() const;
    void setTabbarModel(CategoryTabbarModel *newTabbarModel);

    CategoryTagModel *categoryTagModel() const;
    void setCategoryTagModel(CategoryTagModel *newCategoryTagModel);

signals:
    //void comboItemChanged();

    //void comboItemListChanged();

    void tabbarModelChanged();

    void categoryTagModelChanged();

public slots:
    void modelChanged();

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    //QString m_filterColor;
    QString mFilterType = "All";
    QString m_filterName;
    QString m_filterColumn = "Essential";
    //int m_filterFrom;
    //int m_filterTo;
    //QString m_condition;
    QString m_searchTextCombo;

    QString m_search = ""; //filter check
    bool Asc = true;       //ascending or descending sort
    NodeListModel *nodeListModel = nullptr;

    QVector<QString> columnName;
    QVector<QString> columnNameInt;
    QVector<QString> colorList;
    QVector<QString> tabList;
    QVector<QString> FilterDataList;
    CategoryTabbarModel *mTabbarModel;

    QVector<QString> comboSearch;
    //QStringList m_comboItem;
    //QStringList m_comboItemList;

    QList<FilterTag> TagColor;
    QList<FilterTag1> TagFilter1;
    QList<FilterTag2> TagFilter2;
    QList<FilterTag3> TagFilter3;

    QStringList attakerList;

    enum Ecolumn {
        EColor = 0,
        EIcon,
        EName,
        ETn,
        ECallsing,
        EIFFcode,
        EIdentification,
        EType,
        EMaster,
        ELatitude,
        ELongitude,
        EAltitude,
        EPos,
        EHeading,
        ESpeed,
        EBattle,
        ETarget,
        EMore
    };
    CategoryTagModel *m_categoryTagModel = nullptr;
};

//--------------------------------NodeList-----------------------
class NodeList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *qmlItem READ qmlItem WRITE setQmlItem NOTIFY qmlItemChanged FINAL)

public:
    explicit NodeList(MapControllerItem *mapItem, DataManager *dataManager);

    QQuickItem *qmlItem() const;
    void setQmlItem(QQuickItem *newQmlItem);
    NodeProxyModel *proxyModel() const;
    void setProxyModel(NodeProxyModel *newProxyModel);

signals:
    void qmlItemChanged();

private:
    void createQml();

private:
    MapControllerItem *mMapItem = nullptr;
    QQuickItem *mQmlItem = nullptr;
    DataManager *mDataManager = nullptr;
    NodeProxyModel *mProxyModel = nullptr;
};

#endif // NodeList_H
