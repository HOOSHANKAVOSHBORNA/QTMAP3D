#include "treeModel.h"

TreeModel::TreeModel(QObject *parent):
    QStandardItemModel(parent)
{
    rootItem = invisibleRootItem();
    //    treeMap["root"] = rootItem;
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


void TreeModel::addItem(QStandardItem *item, const QString parent)
{

    //    beginResetModel();

    if(treeMap.contains(parent)){
        treeMap[parent]->appendRow(item);
        treeMap[item->text()] = item;
    }else{
        qDebug() << "error while adding Item to Tree !!!  parent:" << parent << "item:" << item->text() ;
    }
}

void TreeModel::removeItem(QString item, const QString parent)
{
    if(treeMap.contains(item) && treeMap.contains(parent)){
        QStandardItem *parentItem =  treeMap[parent];
        qDebug()<<parentItem->text();
        qDebug()<<parentItem->rowCount();
        for (int i = 0; i < parentItem->rowCount(); ++i){
            qDebug()<<parentItem->child(i)->text();
            if (parentItem->child(i)->text() == item){
                parentItem->removeRow(i);
            }
        }
        treeMap.remove(item);
        //        if(){
        //            QStandardItem *parentItem =  treeMap.take(parent);
        //            for (int i = 0; i < parentItem->rowCount(); ++i){
        //                if (parentItem->child(i)->data() == item){
        //                    parentItem->removeRow(i);
        //                }
        //            }
        //        }else{
        //            qDebug() << rootItem->rowCount();
        //            for (int i = 0; i < rootItem->rowCount(); ++i){
        //                if (rootItem->child(i)->data() == item){
        //                    rootItem->removeRow(i);
        //                }
        //            }
        //        }
    }
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
    if (index.data().toString().contains(mFilterString, Qt::CaseInsensitive))
        return true;
    for (int i = 0; i < sourceModel()->rowCount(index); ++i) {
        if (filterAcceptsRow(i, index))
            return true;
    }
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
    QModelIndex index = mapToSource(current);
    //        static_cast<TreeModel*>(sourceModel())->onItemClicked(index);
}
