#include "nodeInformation.h"


NodeInformation::NodeInformation(QObject *parent)
{
    setColumnCount(1);
    rootItem = invisibleRootItem();
}

NodeInformation::~NodeInformation()
{

}

void NodeInformation::addUpdateNodeInformationItem(NodeData *nodeData, QString imageUrl)
{
    windowName = QString::fromStdString(nodeData->name);
    mainImageUrl = imageUrl;
    for(NodeFieldData nodeFieldData:nodeData->fieldData){

        QStandardItem *item = new QStandardItem;
        QString category = nodeFieldData.category;
        if (mItems.find(category) == mItems.end()){
            QStandardItem *p = new QStandardItem(category);
            mItems[category] = p;
            rootItem->appendRow(p);
        }
        item->setData(QVariant::fromValue(nodeFieldData.name), nameText);
        item->setData(QVariant::fromValue(nodeFieldData.value), valueText);
    }
}

QHash<int, QByteArray> NodeInformation::roleNames() const
{
    QHash<int, QByteArray> textroles = QAbstractItemModel::roleNames();

    textroles[nameText] = "propertyText";
    textroles[valueText] = "valueText";
    textroles[iconImageSource] = "iconImageSource";
    return textroles;
}

QString NodeInformation::getMainImageUrl()
{
    return mainImageUrl;
}

QString NodeInformation::getWindowName()
{
    return windowName;
}


