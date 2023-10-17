#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QQuickItem>
#include <QItemSelectionModel>
#include <QUrl>

struct BookmarkItem: public QObject {
    Q_OBJECT
public:
    BookmarkItem(QString parent, QString text, QQuickItem *item, QString imgUrl);
    QString parent;
    QString text;
    QQuickItem *item;
    QString imgUrl;
signals:
    void itemDeleted();
};

class BookmarkProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QString searchedText READ searchedText WRITE setSearchedText NOTIFY searchedTextChanged)
    enum CustomRoles {
        imageSource = Qt::UserRole + 100,
        itemSource = Qt::UserRole + 101
    };
public:
    static BookmarkProxyModel* createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);
    ~BookmarkProxyModel();
    QHash<int, QByteArray> roleNames() const override;
    void addBookmarkItem(BookmarkItem *bookmarkItem);
    void removeBookmarkItem(BookmarkItem *bookmarkItem);
    QString searchedText() const;
    Q_INVOKABLE void setSearchedText(const QString &newSearchedText);
    Q_INVOKABLE void select(const QModelIndex index);

public slots:
    void removeBookmarkItem(const QModelIndex index);
    QItemSelectionModel *selectioModel() const;
signals:
    void searchedTextChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    BookmarkProxyModel(QObject *parent = nullptr);
private:
    QString mSearchedText;
    QStandardItem *rootItem;
    QStandardItemModel *mStandardItemModel;
    QItemSelectionModel *mSelectioModel;
    std::map<QString, std::pair<BookmarkItem*, QStandardItem*>> mItems;
    static inline BookmarkProxyModel* mInstance{nullptr};

};

#endif // BOOKMARK_H
