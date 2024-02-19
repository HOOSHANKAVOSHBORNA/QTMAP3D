#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QQuickWindow>
#include <QQuickItem>
#include <QAbstractListModel>

class TabbarModel;
class ListWindow;

// --------------------------------------------- struct
struct TabbarItem
{
public:
    QString title;
    QQuickItem *item;
};

enum {
    TitleRole = Qt::UserRole + 1,
    ItemRolee
};

// --------------------------------------------- model
class TabbarModel : public QAbstractListModel
{
    Q_OBJECT

public:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void appendRow(TabbarItem *newLocationItem);
    Q_INVOKABLE void removeRow(const QModelIndex &index);

private:
    QVector<TabbarItem *> mTabbarItems;
};

// --------------------------------------------- list window
class ListWindow : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TabbarModel *tabbarModel READ tabbarModel WRITE setTabbarModel NOTIFY tabbarModelChanged FINAL)

public:
    ListWindow(QQuickWindow *parent = nullptr);

    Q_INVOKABLE TabbarModel *tabbarModel() const;
    void setTabbarModel(TabbarModel *newTabbarModel);
    void appendItem(QString newTitle, QQuickItem *newItem);

signals:
    void tabbarModelChanged();

private:
    TabbarModel *mTabbarModel = nullptr;
};

#endif // LISTWINDOW_H
