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
class CategoryTabbarModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CategoryTabbarModel(DataManager *dataManager);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void onCategoryAppended(int index);

private:
    DataManager *mDataManager;
};

//-----------------------------------------NodeListModel------------------------------------
class NodeListModel : public QAbstractTableModel
{
    Q_OBJECT

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
    void onNodeAppended(int index);
    void onNodeUpated(int index);

    void onColumnAppended(int index);

private:
    DataManager *mDataManager;
    QItemSelectionModel *selectionModel;
    QString modelType;

    //    QMap<EColumn, QString> columnToName = {{Color, "Color"},
    //                                           {Icon, "Icon"},
    //                                           {Name, "Name"},
    //                                           {Type, "Type"},
    //                                           {Latitude, "Latitude"},
    //                                           {Longitude, "Longitude"},
    //                                           {Altitude, "Altitude"},
    //                                           {Speed, "Speed"},
    //                                           {Battle, "Battle"},
    //                                           {Target, "Target"},
    //                                           {More, "More"}};
};

//--------------------------------------NodeProxyModel-------------------------------------
class NodeProxyModel : public QSortFilterProxyModel
{
    Q_PROPERTY(CategoryTabbarModel *tabbarModel READ tabbarModel WRITE setTabbarModel NOTIFY
                   tabbarModelChanged FINAL)
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
public:
    explicit NodeProxyModel(QObject *parent = nullptr);
    Q_INVOKABLE void sortTable(int column);
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    Q_INVOKABLE void nodeTypeFilter(QString type);
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

signals:
    //void comboItemChanged();

    //void comboItemListChanged();

    void tabbarModelChanged();

public slots:
    void modelChanged();

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    //QString m_filterColor;
    QString mFilterType;
    QString m_filterName;
    QString m_filterColumn;
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
