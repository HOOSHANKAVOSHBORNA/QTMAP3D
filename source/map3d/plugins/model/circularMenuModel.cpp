#include "circularMenuModel.h"

CircularMenuModel::CircularMenuModel(QObject *parent)
{
    // ------------------------------ tests
    CircularMenuItem *item1 = new CircularMenuItem{"Info", "qrc:/Resources/info.png", false};
    QObject::connect(item1, &CircularMenuItem::itemClicked, [&] {qDebug() << "info slot called";});

    CircularMenuItem *item2 = new CircularMenuItem{"Bookmark", "qrc:/Resources/bookmark.png", false};
    QObject::connect(item2, &CircularMenuItem::itemClicked, [&] {qDebug() << "bookmark slot called";});

    CircularMenuItem *item3 = new CircularMenuItem{"Attack", "qrc:/Resources/radar.png", false};
    QObject::connect(item3, &CircularMenuItem::itemClicked, [&] {qDebug() << "attack slot called";});

    CircularMenuItem *item4 = new CircularMenuItem{"Target", "qrc:/Resources/hand.png", false};
    QObject::connect(item4, &CircularMenuItem::itemClicked, [&] {qDebug() << "target slot called";});

    mItems.append(item1);
    mItems.append(item2);
    mItems.append(item3);
    mItems.append(item4);
}

int CircularMenuModel::rowCount(const QModelIndex &parent) const
{
    return mItems.size();
}

QVariant CircularMenuModel::data(const QModelIndex &index, int role) const
{
    CircularMenuItem *cmi = mItems.at(index.row());

    switch (role) {
    case NameRole:
        return QVariant(cmi->name);
    case IconUrlRole:
        return QVariant(cmi->iconUrl);
    case CheckableRole:
        return QVariant(cmi->checkable);
    case CheckedRole:
        return QVariant(cmi->checked);
    default:
        break;
    }

    return QVariant("Rule Not Found!");
}

QHash<int, QByteArray> CircularMenuModel::roleNames() const
{
    QHash<int, QByteArray> itemFields;

    itemFields[NameRole] = "name";
    itemFields[IconUrlRole] = "iconUrl";
    itemFields[CheckableRole] = "checkable";
    itemFields[CheckedRole] = "checked";

    return itemFields;
}

void CircularMenuModel::onItemClicked(const QModelIndex &current)
{
    mItems.at(current.row())->itemClicked();
}
