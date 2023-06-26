#include "treeModel.h"

TreeModel::TreeModel(QObject *parent):
    QStandardItemModel(parent)
{
    QStandardItem *rootItem = invisibleRootItem();
    treeMap["root"] = rootItem;
}


Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QStandardItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<QStandardItem*>(parent.internalPointer());

    QStandardItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QStandardItem *childItem = static_cast<QStandardItem*>(index.internalPointer());
    QStandardItem *parentItem = childItem->parent();
    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    QStandardItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<QStandardItem*>(parent.internalPointer());

    return parentItem->rowCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<QStandardItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

void TreeModel::addItem(QStandardItem *item, const QString parent)
{
    beginResetModel();
    //    QStandardItem *rootItem = invisibleRootItem();
    //    if(parent){
    //        for(int i = 0; i < parent->rowCount(); ++i){
    //            QStandardItem *child = parent->child(i);
    //            QStandardItem *treeItem = new QStandardItem(child->text());
    //            parent->appendRow(treeItem);
    //            if(child->hasChildren()){
    //                addItem(child->child(0), treeItem);
    //            }
    //        }
    //    }else{
    //        rootItem = item;
    //    }

        if(treeMap.contains(parent)){
            treeMap[parent]->appendRow(item);
            treeMap[item->text()] = item;
        }else{
            qDebug() << "error while adding Item to Tree !!!";
        }
    endResetModel();
}



/////////////////////////////////////////////////////////////////ProxyModel///////////////////////////////
TreeProxyModel::TreeProxyModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

bool TreeProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
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

QString TreeProxyModel::filterString() const
{
    return mFilterString;
}

void TreeProxyModel::setFilterString(const QString &filterString)
{
    mFilterString = filterString;
    invalidateFilter();
}

void TreeProxyModel::onItemClicked(const QModelIndex &current)
{
    // call mapped item in toolbox model
    QModelIndex index = mapToSource(current);
    //    static_cast<TreeModel*>(sourceModel())->onItemClicked(index);
}
