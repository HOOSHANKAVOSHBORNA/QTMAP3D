#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QQuickItem>
#include <QItemSelectionModel>
#include <QUrl>
#include <QObject>

struct BookmarkItem: public QObject {
    Q_OBJECT

public:
    BookmarkItem(){};

    void setInfo(QString category, QString text, QQuickWindow *window, QString categoryIconUrl)
    {
        this->category = category;
        this->text = text;
        this->categoryIconUrl = categoryIconUrl;
        this->window = window;
    }

    QString category;
    QString text;
    QQuickWindow *window;
    QString categoryIconUrl;

signals:
    void fromBookmarkRemoved();
    void goToPosition();
    void track();
};

class BookmarkProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(
        QString searchedText READ searchedText WRITE setSearchedText NOTIFY searchedTextChanged)

    enum CustomRoles { CategoryIconRole = Qt::UserRole + 1, WindowRole, BookmarkItemRole };

public:
    BookmarkProxyModel(QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;

    void addBookmarkItem(BookmarkItem *bookmarkItem);
    void removeBookmarkItem(BookmarkItem *bookmarkItem);

    QString searchedText() const;
    Q_INVOKABLE void setSearchedText(const QString &newSearchedText);

    Q_INVOKABLE QItemSelectionModel *selectioModel() const;
    Q_INVOKABLE void select(const QModelIndex index);

    Q_INVOKABLE void removeBookmarkItem(const QModelIndex index);
    Q_INVOKABLE void goToPosition(const QModelIndex index);
    Q_INVOKABLE void trackItem(const QModelIndex index);

    //    void select(BookmarkItem *bookmarkItem);

signals:
    void searchedTextChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    void removeItem(QStandardItem *item);

private:
    QString mSearchedText;
    QStandardItem *rootItem;
    QStandardItemModel *mStandardItemModel;
    QItemSelectionModel *mSelectioModel;
};

class BookmarkManager : public QObject
{
    Q_OBJECT

public:
    explicit BookmarkManager();
    ~BookmarkManager();

    void addBookmarkItem(BookmarkItem *bookmarkItem);
    void removeBookmarkItem(BookmarkItem *bookmarkItem);
//    void select(BookmarkItem *bookmarkItem);

    Q_INVOKABLE QItemSelectionModel *getSelectioModel() const;
    Q_INVOKABLE BookmarkProxyModel *getBookmarkProxyModel() const;

private:
    BookmarkProxyModel *mBookmarkProxyModel;
};

#endif // BOOKMARK_H
