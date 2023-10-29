#ifndef TOOLBOXMANAGER_H
#define TOOLBOXMANAGER_H

#include <QAbstractItemModel>
#include <QQmlEngine>
#include <QStandardItemModel>
#include <QMap>
struct ItemDesc;
struct ToolboxItem;

#include <QVariant>
#include <QList>
#include <QSortFilterProxyModel>
#include <QtQml>

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, ToolboxItem *toolbox = nullptr, TreeItem *parentItem = nullptr);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;

    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();
    ToolboxItem* getToolboxItem() const;
    TreeItem *child(TreeItem* row);
    QString imageSource();

private:
    QList<TreeItem *> m_childItems;
    QList<QVariant> m_itemData;
    ToolboxItem *mToolboxItem;
    TreeItem *m_parentItem;
};


struct ToolboxItem: public QObject
{
    Q_OBJECT
public:
    ToolboxItem(
        QString _name      = QString(),
        QString _category  = QString(),
        QString _iconUrl   = QString(),
        bool    _checkable = false):
        name     (_name     ),
        category (_category ),
        iconUrl  (_iconUrl  ),
        checkable(_checkable)
    {

    }

    QString name;
    QString category;
    QString iconUrl;
    bool    checkable = false;
    bool    checked = false;
    void changeCheck(bool check){checked = checkable ? check : checked;}
signals:
    void itemClicked();
    void itemChecked(bool check);
};

class Toolbox : public QAbstractItemModel
{
    Q_OBJECT

    enum CustomRoles {
        imageSource = Qt::UserRole + 100,
        checked = Qt::UserRole + 101,
        checkable = Qt::UserRole + 102
    };

public:
    Toolbox(QObject *parent = nullptr);

    void addItem(ToolboxItem *item);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE int childCount(QModelIndex index);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public slots:
    void onItemClicked(const QModelIndex &current);
private:
    QMap<QString, QStandardItem*> mItems;
    QModelIndex previous;
    QMap<QString, ToolboxItem*> mToolboxItems;
    TreeItem *rootItem;
};

// define proxy model
class ToolboxProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

private:
    explicit ToolboxProxyModel();

public:
    static ToolboxProxyModel* createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);

    Q_INVOKABLE int childCount(QModelIndex index);
    QString filterString() const;

public slots:
    void onItemClicked(const QModelIndex &current);
    void setFilterString(const QString &filterString);

signals:
    void filterStringChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    static ToolboxProxyModel* mInstance;
    QString mFilterString = "";
};

class ToolboxManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

private:
    explicit ToolboxManager();

public:
    static ToolboxManager* createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);

    Q_INVOKABLE ToolboxProxyModel *toolboxProxyModel() const;

private:
    inline static ToolboxManager* mInstance;
    ToolboxProxyModel *mToolboxModel;
};

#endif // TOOLBOXMANAGER_H
