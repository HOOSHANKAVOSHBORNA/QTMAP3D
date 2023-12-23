#include "circularMenu.h"

#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlContext>
#include <osgEarthAnnotation/ModelNode>

#include "qmlNode.h"

// ------------------------------------------------------- model manager
CircularMenu::CircularMenu(QQuickItem *newParentQmlItem, osgEarth::Annotation::GeoPositionNode *newOsgNode)
{
    mParentQmlItem = newParentQmlItem;
    mOsgNode = newOsgNode;
    mCircularMenuModel = new CircularMenuModel;
    createQML();
}

void CircularMenu::appendMenuItem(CircularMenuItem *item)
{
    mCircularMenuModel->appendMenuItem(item);
}

void CircularMenu::show(bool show)
{
    mQmlNode->show(show);
}

void CircularMenu::setQmlNode(QmlNode *newQmlNode)
{
    mQmlNode = newQmlNode;
}

QmlNode *CircularMenu::qmlNode() const
{
    return mQmlNode;
}

void CircularMenu::createQML()
{
    QQmlComponent* comp = new QQmlComponent(QQmlEngine::contextForObject(mParentQmlItem)->engine());
    QObject::connect(comp, &QQmlComponent::statusChanged, [&](const QQmlComponent::Status &status) {
        if(status == QQmlComponent::Error) {
            qDebug() << "Can not load this: " << comp->errorString();
        }

        if(status == QQmlComponent::Ready) {
            // creating menu model
            mQmlNode = qobject_cast<QmlNode*>(comp->create());
            mQmlNode->setParentItem(mParentQmlItem);
            mQmlNode->setOsgNode(mOsgNode);
//            mQmlNode->setVisible(true);
            mQmlNode->setProperty("cppModel", QVariant::fromValue(mCircularMenuModel));
        }
    });
    comp->loadUrl(QUrl("qrc:/QmlNodeItem.qml"));
}

// ------------------------------------------------------- model
CircularMenuModel::CircularMenuModel(QObject *parent)
{
    // TEST
    //    CircularMenuItem *item1 = new CircularMenuItem{"Info", "qrc:/Resources/info.png", false};
    //    QObject::connect(item1, &CircularMenuItem::itemClicked, [&] {qDebug() << "info slot called";});

    //    CircularMenuItem *item2 = new CircularMenuItem{"Bookmark", "qrc:/Resources/bookmark.png", false};
    //    QObject::connect(item2, &CircularMenuItem::itemClicked, [&] {qDebug() << "bookmark slot called";});

    //    CircularMenuItem *item3 = new CircularMenuItem{"Attack", "qrc:/Resources/radar.png", false};
    //    QObject::connect(item3, &CircularMenuItem::itemClicked, [&] {qDebug() << "attack slot called";});

    //    CircularMenuItem *item4 = new CircularMenuItem{"Target", "qrc:/Resources/hand.png", false};
    //    QObject::connect(item4, &CircularMenuItem::itemClicked, [&] {qDebug() << "target slot called";});

    //    mItems.append(item1);
    //    mItems.append(item2);
    //    mItems.append(item3);
    //    mItems.append(item4);
    // ENDTEST
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
    case IconChehckedUrlRole:
        return QVariant(cmi->iconCheckedUrl);
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
    itemFields[IconChehckedUrlRole] = "iconCheckedUrl";

    return itemFields;
}

void CircularMenuModel::appendMenuItem(CircularMenuItem *item)
{
    beginInsertRows(QModelIndex(), mItems.size(), mItems.size());
    mItems.append(item);
    endInsertRows();
}

void CircularMenuModel::onItemClicked(const QModelIndex &current)
{
    mItems.at(current.row())->itemClicked();
}

void CircularMenuModel::onItemChecked(const QModelIndex &current, bool checked)
{
    mItems.at(current.row())->itemChecked(checked);
}

