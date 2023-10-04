#include "toolbox.h"

#include <QHeaderView>
#include <QQmlComponent>
#include <QQuickItem>

#include "plugininterface.h"

TreeItem::TreeItem(const QList<QVariant> &data, ToolboxItem *toolbox, TreeItem *parent)
    : m_itemData(data), m_parentItem(parent), mToolboxItem(toolbox)
{}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return m_itemData.count();
}


QVariant TreeItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return mToolboxItem->name;
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

ToolboxItem *TreeItem::getToolboxItem() const
{
    return mToolboxItem;
}

TreeItem *TreeItem::child(TreeItem *row)
{
    auto it = std::find_if(m_childItems.begin(), m_childItems.end(), [&](const TreeItem *temp){
        return temp->mToolboxItem->category == row->mToolboxItem->category;
    });
    if (it == m_childItems.end())
        return nullptr;
    return *it;
}

QString TreeItem::imageSource()
{
    return mToolboxItem->iconUrl;
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

Toolbox::Toolbox(QObject *parent):
    QAbstractItemModel(parent)
{
    rootItem = new TreeItem({tr("Title")});
}

void Toolbox::addItem(ToolboxItem *toolboxItem)
{
    beginResetModel();
    TreeItem* cat = new TreeItem({toolboxItem->category}, new ToolboxItem(toolboxItem->category, toolboxItem->category), rootItem);
    if (!rootItem->child(cat))
        rootItem->appendChild(cat);
    rootItem->child(cat)->appendChild(new TreeItem({toolboxItem->name}, toolboxItem, rootItem->child(cat)));
    endResetModel();
}

QVariant Toolbox::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    switch (role) {
    case Qt::DisplayRole:
        return item->data(index.column());
        break;
    case imageSource:
        return item->imageSource();
        break;
    case checked:
        return item->getToolboxItem()->checked;
        break;
    default:
        break;
    }
    return QVariant();
}

QHash<int, QByteArray> Toolbox::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[imageSource] = "imageSource";
    hash[checked] = "checkedd";
    hash[checkable] = "checkable";
    return hash;
}

Qt::ItemFlags Toolbox::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant Toolbox::headerData(int section, Qt::Orientation orientation,
                             int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex Toolbox::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex Toolbox::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int Toolbox::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int Toolbox::childCount1(QModelIndex index)
{
    const QStandardItemModel* model = static_cast<const QStandardItemModel*>(index.model());
    QStandardItem* node = model->itemFromIndex(index);
    return node->rowCount();
}

int Toolbox::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

void Toolbox::onItemClicked(const QModelIndex &current)
{
    if (previous.isValid()){
        TreeItem *previousItem = static_cast<TreeItem*>(previous.internalPointer());
        previousItem->getToolboxItem()->changeCheck(false);
        if (previousItem->getToolboxItem()->checkable)
            emit previousItem->getToolboxItem()->itemChecked(false);
        else
            previous = QModelIndex();
//            emit previousItem->getToolboxItem()->itemClicked();
        emit dataChanged(previous, previous);
    }

    if (current.isValid() && current != previous){
        TreeItem *currentItem = static_cast<TreeItem*>(current.internalPointer());
        if (currentItem->getToolboxItem()->checkable){
            currentItem->getToolboxItem()->changeCheck(true);
            emit currentItem->getToolboxItem()->itemChecked(true);
        }else
            emit currentItem->getToolboxItem()->itemClicked();

        emit dataChanged(current, current);
    }
    if (previous == current)
        previous = QModelIndex();
    else
        previous = current;
}

ToolboxProxyModel::ToolboxProxyModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

int ToolboxProxyModel::childCount1(QModelIndex index)
{
    return dynamic_cast<Toolbox*>(sourceModel())->childCount1(mapToSource(index));
}

bool ToolboxProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    // check if item name or its children contain filter string (case insensitive)
    if (index.data().toString().contains(mFilterString, Qt::CaseInsensitive))
        return true;
    for (int i = 0; i < sourceModel()->rowCount(index); ++i) {
        if (filterAcceptsRow(i, index))
            return true;
    }
    // show its children if its name contains filter string
    if (source_parent.isValid() && source_parent.data().toString().contains(mFilterString, Qt::CaseInsensitive))
        return true;
    return false;
}

QString ToolboxProxyModel::filterString() const
{
    return mFilterString;
}

void ToolboxProxyModel::setFilterString(const QString &filterString)
{
    mFilterString = filterString;
    invalidateFilter();
}

void ToolboxProxyModel::onItemClicked(const QModelIndex &current)
{
    // call mapped item in toolbox model
    QModelIndex index = mapToSource(current);
    static_cast<Toolbox*>(sourceModel())->onItemClicked(index);
}
