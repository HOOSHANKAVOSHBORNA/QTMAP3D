#include "circularMenuModel.h"

CircularMenuModel::CircularMenuModel(QObject *parent)
{
    // ------------------------------ test
    CircularMenuItem *item1 = new CircularMenuItem{"Info", "qrc:/Resources/info.png", false, false};
    CircularMenuItem *item2 = new CircularMenuItem{"Bookmark", "qrc:/Resources/bookmark.png", false, false};
    CircularMenuItem *item3 = new CircularMenuItem{"Attack", "qrc:/Resources/radar.png", false, false};
    CircularMenuItem *item4 = new CircularMenuItem{"Target", "qrc:/Resources/hand.png", false, false};

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
