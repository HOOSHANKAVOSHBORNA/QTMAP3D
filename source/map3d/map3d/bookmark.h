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
    BookmarkItem(QString parent, QString text, QQuickWindow *wnd, QString imgUrl);
    QString parent;
    QString text;
    QQuickWindow *wnd;
    QString imgUrl;
signals:
    void itemDeleted();
};

class BookmarkProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString searchedText READ searchedText WRITE setSearchedText NOTIFY searchedTextChanged)
    enum CustomRoles {
        imageSource = Qt::UserRole + 100,
        itemSource = Qt::UserRole + 101
    };
public:
    BookmarkProxyModel(QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    QString searchedText() const;
    void addBookmarkItem(BookmarkItem *bookmarkItem);
    Q_INVOKABLE void setSearchedText(const QString &newSearchedText);
    Q_INVOKABLE QItemSelectionModel *selectioModel() const;
    Q_INVOKABLE void removeBookmarkItem(const QModelIndex index);
    void removeBookmarkItem(BookmarkItem *bookmarkItem);
    Q_INVOKABLE void select(const QModelIndex index);
    void select(BookmarkItem *bookmarkItem);
signals:
    void searchedTextChanged();
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private:
    QString mSearchedText;
    QStandardItem *rootItem;
    QStandardItemModel *mStandardItemModel;
    QItemSelectionModel *mSelectioModel;
    std::map<QString, std::pair<BookmarkItem*, QStandardItem*>> mItems;
};

class BookmarkManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    // --------------- create singelton instance -----------------
    static BookmarkManager *createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);
    ~BookmarkManager();

    // --------------- Interface ---------------------------------
    void addBookmarkItem(BookmarkItem *bookmarkItem);
    void removeBookmarkItem(BookmarkItem *bookmarkItem);
    void select(BookmarkItem *bookmarkItem);

    // --------------- getters -----------------------------------
    Q_INVOKABLE QItemSelectionModel *getSelectioModel() const;
    Q_INVOKABLE BookmarkProxyModel *getBookmarkProxyModel() const;

protected:
    BookmarkManager(QObject *parent = nullptr); // -------------- protected constructor for singelton

private:
    static inline BookmarkManager* mInstance{nullptr};
    BookmarkProxyModel *mBookmarkProxyModel;

};

#endif // BOOKMARK_H
