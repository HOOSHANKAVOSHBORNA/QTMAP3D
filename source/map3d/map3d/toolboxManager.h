#ifndef TOOLBOXMANAGER_H
#define TOOLBOXMANAGER_H

#include <QAbstractItemModel>
#include <QList>
#include <QMap>
#include <QQmlEngine>
#include <QQuickItem>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QVariant>
#include <QtQml>

struct ItemDesc;
struct ToolboxItem;

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
        bool    _checkable = false
    ):
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
        checked,
        checkable
    };

public:
    Toolbox(QObject *parent = nullptr);
    ~Toolbox();

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

public:
    explicit ToolboxProxyModel(QObject *parent = nullptr);

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

    Q_PROPERTY(QQuickItem *propertyItem READ propertyItem WRITE setPropertyItem NOTIFY
                   propertyItemChanged FINAL)
    Q_PROPERTY(QString propertyItemTitle READ propertyItemTitle WRITE setPropertyItemTitle NOTIFY
                   propertyItemTitleChanged FINAL)

public:
    explicit ToolboxManager(QObject* parent = nullptr);
    ~ToolboxManager();

    Q_INVOKABLE ToolboxProxyModel *toolboxProxyModel() const;

    void addItem(ToolboxItem *item);

    QQuickItem *propertyItem() const;
    void setPropertyItem(QQuickItem *newPropertyItem);
    void addPropertyItem(QQuickItem *newPropertyItem, QString title);
    Q_INVOKABLE void removePropertyItem();

    QString propertyItemTitle() const;
    void setPropertyItemTitle(const QString &newPropertyItemTitle);

    ToolboxProxyModel *getToolboxProxyModel();

signals:
    void propertyItemChanged();

    void propertyItemTitleChanged();

private:
    ToolboxProxyModel *mToolboxModel = nullptr;
    QQuickItem *mPropertyItem = nullptr;
    QString mPropertyItemTitle;
};

#endif // TOOLBOXMANAGER_H
