#include "bookmark.h"

#include <QQuickWindow>

BookmarkProxyModel::BookmarkProxyModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    mStandardItemModel = new QStandardItemModel;
    setSourceModel(mStandardItemModel);
    rootItem = mStandardItemModel->invisibleRootItem();
    mSelectioModel = new QItemSelectionModel(this);
}

BookmarkManager *BookmarkManager::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(mInstance == nullptr){ mInstance = new BookmarkManager(); }
    return mInstance;
}

BookmarkManager::~BookmarkManager()
{
    delete mBookmarkProxyModel;
}

void BookmarkManager::addBookmarkItem(BookmarkItem *bookmarkItem)
{
    mBookmarkProxyModel->addBookmarkItem(bookmarkItem);
}

void BookmarkManager::removeBookmarkItem(BookmarkItem *bookmarkItem)
{
    mBookmarkProxyModel->removeBookmarkItem(bookmarkItem);
}

QHash<int, QByteArray> BookmarkProxyModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[imageSource] = "imageSource";
    hash[itemSource] = "itemSource";

    return hash;
}

void BookmarkManager::select(BookmarkItem *bookmarkItem)
{
    if (bookmarkItem)
        mBookmarkProxyModel->select(bookmarkItem);
}

QItemSelectionModel *BookmarkManager::getSelectioModel() const
{
    return mBookmarkProxyModel->selectioModel();
}

BookmarkProxyModel *BookmarkManager::getBookmarkProxyModel() const
{
    return mBookmarkProxyModel;
}

BookmarkManager::BookmarkManager(QObject *parent): QObject(parent)
{
    mBookmarkProxyModel= new BookmarkProxyModel();
}

void BookmarkProxyModel::addBookmarkItem(BookmarkItem *bookmarkItem)
{
    QStandardItem *item = new QStandardItem;
    item->setText(bookmarkItem->text);
    item->setData(QVariant::fromValue(bookmarkItem->wnd), itemSource);
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

void BookmarkProxyModel::goToPosition(BookmarkItem *bookmarkItem)
{
    emit bookmarkItem->itemGoToPostition();
}

void BookmarkProxyModel::trackItem(BookmarkItem *bookmarkItem)
{
    emit bookmarkItem->itemTracked();
}

void BookmarkProxyModel::select(const QModelIndex index)
{
    mSelectioModel->select(index, QItemSelectionModel::SelectionFlag::Toggle);
}

void BookmarkProxyModel::select(BookmarkItem *bookmarkItem)
{
    QModelIndex indexSource = mapToSource(mStandardItemModel->indexFromItem(mItems[bookmarkItem->text].second));
    select(indexSource);
}

void BookmarkProxyModel::removeBookmarkItem(const QModelIndex index)
{
    QModelIndex indexSource = mapToSource(index);
    QStandardItem* node = mStandardItemModel->itemFromIndex(indexSource);
    removeBookmarkItem(mItems[node->text()].first);
}

void BookmarkProxyModel::goToPosition(const QModelIndex index)
{
    QModelIndex indexSource = mapToSource(index);
    QStandardItem* node = mStandardItemModel->itemFromIndex(indexSource);
    goToPosition(mItems[node->text()].first);
}

void BookmarkProxyModel::trackItem(const QModelIndex index)
{
    QModelIndex indexSource = mapToSource(index);
    QStandardItem* node = mStandardItemModel->itemFromIndex(indexSource);
    trackItem(mItems[node->text()].first);
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

BookmarkItem::BookmarkItem(QString parent, QString text, QQuickWindow *wnd, QString imgUrl):
    parent{parent}, text{text}, wnd{wnd}, imgUrl{imgUrl}
{

}
