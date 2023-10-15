#include "bookmark.h"

BookmarkProxyModel::BookmarkProxyModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    mStandardItemModel = new QStandardItemModel;
    setSourceModel(mStandardItemModel);
    rootItem = mStandardItemModel->invisibleRootItem();
    mSelectioModel = new QItemSelectionModel(this);
}

BookmarkProxyModel::~BookmarkProxyModel()
{

}

QHash<int, QByteArray> BookmarkProxyModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[imageSource] = "imageSource";
    hash[itemSource] = "itemSource";

    return hash;
}

void BookmarkProxyModel::select(const QModelIndex index)
{
    mSelectioModel->select(index, QItemSelectionModel::SelectionFlag::Toggle);
}

void BookmarkProxyModel::addBookmarkItem(BookmarkItem *bookmarkItem)
{
    QStandardItem *item = new QStandardItem;
    item->setText(bookmarkItem->text);
    item->setData(QVariant::fromValue(bookmarkItem->item), itemSource);
    QString parent = bookmarkItem->parent;
    if (mItems.find(parent) == mItems.end()){
        QStandardItem *p = new QStandardItem(parent);
        mItems[parent].second = p;
        rootItem->appendRow(p);
        p->setData(bookmarkItem->imgUrl, imageSource);
    }
    mItems[bookmarkItem->text].second = item;
    mItems[bookmarkItem->text].first = bookmarkItem;
    mItems[parent].second->appendRow(item);
}

void BookmarkProxyModel::removeBookmarkItem(BookmarkItem *bookmarkItem)
{
    mItems[bookmarkItem->parent].second->removeRow(mItems[bookmarkItem->text].second->row());
    mItems.erase(bookmarkItem->text);
    if (mItems[bookmarkItem->parent].second->rowCount() == 0){
        rootItem->removeRow(mItems[bookmarkItem->parent].second->row());
        mItems.erase(bookmarkItem->parent);
    }
    emit bookmarkItem->itemDeleted();
}

void BookmarkProxyModel::removeBookmarkItem(const QModelIndex index)
{
    QModelIndex indexSource = mapToSource(index);
    QStandardItem* node = mStandardItemModel->itemFromIndex(indexSource);
    removeBookmarkItem(mItems[node->text()].first);
}

QString BookmarkProxyModel::searchedText() const
{
    return mSearchedText;
}

void BookmarkProxyModel::setSearchedText(const QString &newSearchedText)
{
    if (mSearchedText == newSearchedText)
        return;
    mSearchedText = newSearchedText;
    emit searchedTextChanged();

    invalidateFilter();
}

bool BookmarkProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex indexText = sourceModel()->index(sourceRow, 0, sourceParent);
    if(sourceModel()->data(indexText).toString().toLower().contains(mSearchedText.toLower()))
        return true;

    int childCount = sourceModel()->rowCount(indexText);
    for(int i=0; i<childCount; i++) {
        if(filterAcceptsRow(i, indexText)) {
            return true;
        }
    }
    if (sourceParent.isValid() && sourceParent.data().toString().contains(mSearchedText, Qt::CaseInsensitive))
        return true;
    return false;
}

QItemSelectionModel *BookmarkProxyModel::selectioModel() const
{
    return mSelectioModel;
}

BookmarkItem::BookmarkItem(QString parent, QString text, QQuickItem *item, QString imgUrl):
    parent{parent}, text{text}, item{item}, imgUrl{imgUrl}
{

}
