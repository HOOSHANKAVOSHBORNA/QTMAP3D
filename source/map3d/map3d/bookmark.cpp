#include "bookmark.h"

#include <QQuickWindow>

BookmarkProxyModel::BookmarkProxyModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    mStandardItemModel = new QStandardItemModel(this);
    setSourceModel(mStandardItemModel);
    rootItem = mStandardItemModel->invisibleRootItem();
    mSelectioModel = new QItemSelectionModel(this);
}

BookmarkProxyModel::~BookmarkProxyModel()
{
    qDebug()<<"~BookmarkProxyModel";
}

QHash<int, QByteArray> BookmarkProxyModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[CategoryIconRole] = "categoryIcon";
    hash[WindowRole] = "window";

    return hash;
}

void BookmarkProxyModel::addBookmarkItem(BookmarkItem *bookmarkItem)
{
    QStandardItem *categoryItem{nullptr};
    auto catItemList = mStandardItemModel->findItems(bookmarkItem->category);
    if(!catItemList.empty())
        categoryItem = catItemList.first();

    if(!categoryItem){
        categoryItem = new QStandardItem(bookmarkItem->category);
        rootItem->appendRow(categoryItem);
        if(!bookmarkItem->categoryIconUrl.contains("qrc"))
            bookmarkItem->categoryIconUrl = "file:" + bookmarkItem->categoryIconUrl;
        categoryItem->setData(bookmarkItem->categoryIconUrl, CategoryIconRole);
    }

    QStandardItem *item = new QStandardItem;
    item->setText(bookmarkItem->text);
    item->setData(QVariant::fromValue(bookmarkItem->window), WindowRole);
    item->setData(QVariant::fromValue(bookmarkItem), BookmarkItemRole);

    categoryItem->appendRow(item);
}

void BookmarkProxyModel::removeBookmarkItem(BookmarkItem *bookmarkItem)
{
    auto catItemList = mStandardItemModel->findItems(bookmarkItem->category);
    if(!catItemList.empty()){
        QStandardItem *categoryItem = catItemList.first();
        for(int i = 0; i < categoryItem->rowCount(); ++i){
            auto item = categoryItem->child(i);
            if(item->text() == bookmarkItem->text){
                removeItem(item);
                emit bookmarkItem->fromBookmarkRemoved();
                break;
            }
        }
    }
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

QItemSelectionModel *BookmarkProxyModel::selectioModel() const
{
    return mSelectioModel;
}

void BookmarkProxyModel::select(const QModelIndex index)
{
    mSelectioModel->select(index, QItemSelectionModel::SelectionFlag::Toggle);
}

//void BookmarkProxyModel::select(BookmarkItem *bookmarkItem)
//{
//    QModelIndex indexSource = mapToSource(mStandardItemModel->indexFromItem(mItems[bookmarkItem->text].second));
//    select(indexSource);
//}

void BookmarkProxyModel::removeBookmarkItem(const QModelIndex index)
{
    QModelIndex indexSource = mapToSource(index);
    QStandardItem* item = mStandardItemModel->itemFromIndex(indexSource);
    if(item){
        BookmarkItem* bookmarkItem = item->data(BookmarkItemRole).value<BookmarkItem*>();
        removeItem(item);
        if(bookmarkItem)
            emit bookmarkItem->fromBookmarkRemoved();
    }

}

void BookmarkProxyModel::goToPosition(const QModelIndex index)
{
    QModelIndex indexSource = mapToSource(index);
    QStandardItem* item = mStandardItemModel->itemFromIndex(indexSource);
    if(item){
        BookmarkItem* bookmarkItem = item->data(BookmarkItemRole).value<BookmarkItem*>();
        if(bookmarkItem)
            emit bookmarkItem->goToPosition();
    }
}

void BookmarkProxyModel::trackItem(const QModelIndex index)
{
    QModelIndex indexSource = mapToSource(index);
    QStandardItem* item = mStandardItemModel->itemFromIndex(indexSource);
    if(item){
        BookmarkItem* bookmarkItem = item->data(BookmarkItemRole).value<BookmarkItem*>();
        if(bookmarkItem)
            emit bookmarkItem->track();
    }
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

void BookmarkProxyModel::removeItem(QStandardItem *item)
{
    auto parent = item->parent();
    parent->removeRow(item->row());
    // remove category if has not childeren
    if(!parent->hasChildren()){
        removeRow(parent->row());
    }
}

//--bookmark manager----------------------------------------------------------------------------
BookmarkManager::BookmarkManager(QObject *parent)
    :QObject(parent)
{
    mBookmarkProxyModel = new BookmarkProxyModel(this);
}

BookmarkManager::~BookmarkManager()
{
    qDebug()<<"~BookmarkManager";
}

void BookmarkManager::addBookmarkItem(BookmarkItem *bookmarkItem)
{
    mBookmarkProxyModel->addBookmarkItem(bookmarkItem);
}

void BookmarkManager::removeBookmarkItem(BookmarkItem *bookmarkItem)
{
    mBookmarkProxyModel->removeBookmarkItem(bookmarkItem);
}

//void BookmarkManager::select(BookmarkItem *bookmarkItem)
//{
//    if (bookmarkItem)
//        mBookmarkProxyModel->select(bookmarkItem);
//}

QItemSelectionModel *BookmarkManager::getSelectioModel() const
{
    return mBookmarkProxyModel->selectioModel();
}

BookmarkProxyModel *BookmarkManager::getBookmarkProxyModel() const
{
    return mBookmarkProxyModel;
}

BookmarkItem::~BookmarkItem()
{
    qDebug()<<"~BookmarkItem";
}
