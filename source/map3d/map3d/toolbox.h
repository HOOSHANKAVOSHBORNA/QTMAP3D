#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QAbstractItemModel>
#include <QQmlEngine>
#include <QStandardItemModel>
#include <QMap>
struct ItemDesc;
struct ToolboxItem;
//class TreeItem;

//class ToolboxModel : public QAbstractItemModel
//{
//    Q_OBJECT
//public:
//    enum CustomRoles {

//    };
//    ToolboxModel(QObject *parent = nullptr);


//    QVariant data(const QModelIndex &index, int role) const override;
//    Qt::ItemFlags flags(const QModelIndex &index) const override;
//    QVariant headerData(int section, Qt::Orientation orientation,
//                        int role = Qt::DisplayRole) const override;
//    QModelIndex index(int row, int column,
//                      const QModelIndex &parent = QModelIndex()) const override;
//    QModelIndex parent(const QModelIndex &index) const override;
//    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

//    QVariant newCustomType(const QString &text, int position);
//    void setupModelData(const QStringList &lines, TreeItem *parent);

//private:
//    TreeItem *rootItem;
//};

//class TreeItem
//{
//public:
//    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = 0);
//    ~TreeItem();

//    void appendChild(TreeItem *child);

//    TreeItem *child(int row);
//    int childCount() const;
//    int columnCount() const;
//    QVariant data(int column) const;
//    int row() const;
//    TreeItem *parentItem();

//private:
//    QList<TreeItem*> m_childItems;
//    QList<QVariant> m_itemData;
//    TreeItem *m_parentItem;
//};

//class Toolbox : public QObject
//{
//    Q_OBJECT
//public:
//    Toolbox(QQmlEngine *engine, QObject* parent = nullptr);

//private:
//    ToolboxModel *model;
//};

class Toolbox : public QStandardItemModel
{
    Q_OBJECT
    enum CustomRoles {
        imageSource = Qt::UserRole + 100
    };

public:
    Toolbox(QObject *parent = nullptr);

    void addItem(ToolboxItem *item);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
public slots:
    void onItemClicked(QString name);
private:
    QMap<QString, QStandardItem*> mItems;
    QMap<QString, ToolboxItem*> mToolboxItems;
};

#endif // TOOLBOX_H
