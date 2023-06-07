#include "toolbox.h"

#include <QQmlComponent>
#include <QQuickItem>

//ToolboxModel::ToolboxModel(QObject *parent):
//    QAbstractItemModel(parent)
//{
//    QList<QVariant> rootData;
//    rootData << "Title" << "Summary";
//    rootItem = new TreeItem(rootData);
//    QList<TreeItem*> parents;
//    QList<QVariant> vars;
//    vars << "dddd" << "ddd";
//    rootItem->appendChild(new TreeItem(vars));
//    rootItem->appendChild(new TreeItem(vars));
//    rootItem->appendChild(new TreeItem(vars));
//}

//QVariant ToolboxModel::data(const QModelIndex &index, int role) const
//{
//    if (!index.isValid())
//        return QVariant();

////    if (role != TreeModelRoleName && role != TreeModelRoleDescription)
////        return QVariant();

//    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

//    return item->data(role - Qt::UserRole - 1);
//}

//Qt::ItemFlags ToolboxModel::flags(const QModelIndex &index) const
//{
//    if (!index.isValid())
//        return QAbstractItemModel::flags(QModelIndex());

//    return QAbstractItemModel::flags(index);
//}

//QVariant ToolboxModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
//        return rootItem->data(section);

//    return QVariant();
//}

//QModelIndex ToolboxModel::index(int row, int column, const QModelIndex &parent) const
//{
//    if (!hasIndex(row, column, parent))
//        return QModelIndex();

//    TreeItem *parentItem;

//    if (!parent.isValid())
//        parentItem = rootItem;
//    else
//        parentItem = static_cast<TreeItem*>(parent.internalPointer());

//    TreeItem *childItem = parentItem->child(row);
//    if (childItem)
//        return createIndex(row, column, childItem);
//    else
//        return QModelIndex();
//}

//QModelIndex ToolboxModel::parent(const QModelIndex &index) const
//{
//    if (!index.isValid())
//        return QModelIndex();

//    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
//    TreeItem *parentItem = childItem->parentItem();

//    if (parentItem == rootItem)
//        return QModelIndex();

//    return createIndex(parentItem->row(), 0, parentItem);
//}

//int ToolboxModel::rowCount(const QModelIndex &parent) const
//{
//    TreeItem *parentItem;
//    if (parent.column() > 0)
//        return 0;

//    if (!parent.isValid())
//        parentItem = rootItem;
//    else
//        parentItem = static_cast<TreeItem*>(parent.internalPointer());

//    return parentItem->childCount();
//}

//int ToolboxModel::columnCount(const QModelIndex &parent) const
//{
//    if (parent.isValid())
//        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
//    else
//        return rootItem->columnCount();
//}

//QVariant ToolboxModel::newCustomType(const QString &text, int position)
//{

//}

//TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
//{
//    m_parentItem = parent;
//    m_itemData = data;
//}
////! [0]

////! [1]
//TreeItem::~TreeItem()
//{
//    qDeleteAll(m_childItems);
//}
////! [1]

////! [2]
//void TreeItem::appendChild(TreeItem *item)
//{
//    m_childItems.append(item);
//}
////! [2]

////! [3]
//TreeItem *TreeItem::child(int row)
//{
//    return m_childItems.value(row);
//}
////! [3]

////! [4]
//int TreeItem::childCount() const
//{
//    return m_childItems.count();
//}
////! [4]

////! [5]
//int TreeItem::columnCount() const
//{
//    return m_itemData.count();
//}
////! [5]

////! [6]
//QVariant TreeItem::data(int column) const
//{
//    return m_itemData.value(column);
//}
////! [6]

////! [7]
//TreeItem *TreeItem::parentItem()
//{
//    return m_parentItem;
//}
////! [7]

////! [8]
//int TreeItem::row() const
//{
//    if (m_parentItem)
//        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

//    return 0;
//}
////! [8]

//Toolbox::Toolbox(QQmlEngine *engine, QObject *parent)
//{
//    QQmlComponent *comp = new QQmlComponent(engine);
//    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
//        if (comp->status() == QQmlComponent::Ready) {
//            QQuickItem *item = static_cast<QQuickItem*>(comp->create(nullptr));
//            model = new ToolboxModel;
//            item->setProperty("toolModel", QVariant::fromValue<ToolboxModel*>(model));
//        }

//    });
//    comp->loadUrl(QUrl("qrc:/CustomTreeView.qml"));


//}

Toolbox::Toolbox(QObject *parent):
    QStandardItemModel(parent)
{

}

void Toolbox::addItem(ItemDesc *item)
{
    if (!mItems.contains(item->category)){
        QStandardItem* catItem = new QStandardItem(item->category);
        mItems[item->category] = catItem;
        QStandardItem *rootItem = invisibleRootItem();
        rootItem->appendRow(catItem);
    }
    mItems[item->category]->appendRow(new QStandardItem(item->name));
}
