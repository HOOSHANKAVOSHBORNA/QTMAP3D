#include "circularMenu.h"

#include "simpleModelNode.h"
#include <QQmlComponent>

#include "qmlNode.h"

// ------------------------------------------------------- model manager
CircularMenu::CircularMenu(MapItem *mapItem, QQmlEngine *qQmlEngine, QObject *parent) : QObject(parent)
{
    mMapItem = mapItem;
    mEngine = qQmlEngine;
}

void CircularMenu::createQML(SimpleModelNode *smn)
{
    QQmlComponent* comp = new QQmlComponent(mEngine, smn);
    QObject::connect(comp, &QQmlComponent::statusChanged, [&](const QQmlComponent::Status &status) {
        if(status == QQmlComponent::Error) {
            qDebug() << "Can not load this: " << comp->errorString();
        }

        if(status == QQmlComponent::Ready) {
            // creating menu model
            CircularMenuModel *cmm = new CircularMenuModel;

            qDebug() << "in ready";
            mQmlNode = qobject_cast<QmlNode*>(comp->create());
            mQmlNode->setParentItem(mMapItem);
            mQmlNode->setOsgNode(smn);
            mQmlNode->setVisible(false);
            mQmlNode->setProperty("cppModel", QVariant::fromValue(cmm));
        }
    });
    comp->loadUrl(QUrl("qrc:/QmlNodeItem.qml"));
    qDebug() << "waiting for creating";
}

void CircularMenu::setQmlNode(QmlNode *newQmlNode)
{
    mQmlNode = newQmlNode;
}

QmlNode *CircularMenu::qmlNode() const
{
    return mQmlNode;
}



// ------------------------------------------------------- model
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

