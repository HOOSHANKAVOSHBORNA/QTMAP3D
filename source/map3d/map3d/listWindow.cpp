#include "listWindow.h"
#include "application.h"
#include <QTimer>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QVariant>

ListWindow::ListWindow(QQuickWindow *parent) :
    QQuickWindow(parent)
{
    mTabbarModel = new TabbarModel();

    //    mTabbarModel->appendRow(new TabbarItem{"something", nullptr});
}

ListWindow::~ListWindow()
{
}

TabbarModel::TabbarModel(QObject *parent) : QAbstractListModel(parent)
{

}

TabbarModel::~TabbarModel()
{
    qDeleteAll(mTabbarItems);
}

int TabbarModel::rowCount(const QModelIndex &parent) const
{
    return mTabbarItems.size();
}

QVariant TabbarModel::data(const QModelIndex &index, int role) const
{
    TabbarItem *tabbarItem = mTabbarItems.at(index.row());

    switch (role) {
    case TitleRole:
        return QVariant(tabbarItem->title);
    case ItemRolee:
        return QVariant::fromValue(tabbarItem->item);
    default:
        break;
    }

    return QVariant("null");
}

QHash<int, QByteArray> TabbarModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[TitleRole] = "title";
    roles[ItemRolee] = "item";

    return roles;
}

void TabbarModel::appendRow(TabbarItem *newTabbarItem)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    mTabbarItems.append(newTabbarItem);
    endInsertRows();
}

void TabbarModel::removeRow(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    mTabbarItems.removeAt(index.row());
    endRemoveRows();
}

TabbarModel *ListWindow::tabbarModel() const
{
    return mTabbarModel;
}

void ListWindow::setTabbarModel(TabbarModel *newTabbarModel)
{
    if (mTabbarModel == newTabbarModel)
        return;
    mTabbarModel = newTabbarModel;
    emit tabbarModelChanged();
}

void ListWindow::appendItem(QString newTitle, QQuickItem *newItem)
{
    mTabbarModel->appendRow(new TabbarItem{newTitle, newItem});
}
