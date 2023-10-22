#include "nodeInformation.h"


NodeInformation::NodeInformation(QObject *parent)
{
    setColumnCount(1);
    rootItem = invisibleRootItem();
}

NodeInformation::~NodeInformation()
{

}

NodeInformationItem *NodeInformation::addNodeInformationItem(NodeInformationItem *nodeInformationItem)
{
    QStandardItem *item = new QStandardItem;
    QString parent = nodeInformationItem->parent;
    if (mItems.find(parent) == mItems.end()){
        QStandardItem *p = new QStandardItem(parent);
        mItems[parent].second = p;
        rootItem->appendRow(p);
        p->setData(nodeInformationItem->iconImageUrl,iconImageSource);
    }
    item->setData(QVariant::fromValue(nodeInformationItem->property), propertyText);
    item->setData(QVariant::fromValue(nodeInformationItem->value), valueText);
    mItems[nodeInformationItem->property].second = item;
    mItems[nodeInformationItem->property].first = nodeInformationItem;
    mItems[nodeInformationItem->value].second = item;
    mItems[nodeInformationItem->value].first = nodeInformationItem;
    mItems[parent].second->appendRow(item);

    return nodeInformationItem;
}

void NodeInformation::updateNodeInformationItem(NodeInformationItem *node, QString value)
{
    mItems[node->value].second->setData(value,valueText);
}

QHash<int, QByteArray> NodeInformation::roleNames() const
{
    QHash<int, QByteArray> textroles = QAbstractItemModel::roleNames();

    textroles[propertyText] = "propertyText";
    textroles[valueText] = "valueText";
    textroles[iconImageSource] = "iconImageSource";
    return textroles;
}

void NodeInformation::setHeaderItem(HeaderItem *headerItem)
{
    mHeaderItem = headerItem;
}

QString NodeInformation::getMainImageUrl()
{
    return mHeaderItem->mainImageUrl;
}

QString NodeInformation::getHeaderImageUrl()
{
    return mHeaderItem->headerImageUrl;
}

QString NodeInformation::getWindowName()
{
    return mHeaderItem->windowName;
}

bool NodeInformation::bookmarkStatus()
{
    return mHeaderItem->isBookmarked;
}

NodeInformationItem::NodeInformationItem(QString parent, QString property, QString value, QString iconImageUrl):
parent{parent}, property{property}, value{value}, iconImageUrl{iconImageUrl}
{

}

HeaderItem::HeaderItem(QString mainImageUrl, QString headerImageUrl, QString windowName, bool isBookmarked):
mainImageUrl{mainImageUrl}, headerImageUrl{headerImageUrl}, windowName{windowName}, isBookmarked{isBookmarked}
{

}
