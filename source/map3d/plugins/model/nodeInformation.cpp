#include "nodeInformation.h"

NodeInformation::NodeInformation(QQmlEngine* Engine,QObject *parent)
{
    setColumnCount(1);
    rootItem = invisibleRootItem();

    QQmlComponent* comp = new QQmlComponent(Engine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [&](const QQmlComponent::Status &status){
        if(status == QQmlComponent::Error){
            qDebug()<<"Can not load this: "<<comp->errorString();
        }

        if(status == QQmlComponent::Ready){
            wnd = qobject_cast<QQuickWindow*>(comp->create());
        }
    });
    comp->loadUrl(QUrl("qrc:/NodeInformation.qml"));
    wnd->show();
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
        mItems[category]->appendRow(item);
    }

    wnd->setProperty("nodeinfo", QVariant::fromValue<NodeInformation*>(this));
}

QHash<int, QByteArray> NodeInformation::roleNames() const
{
    QHash<int, QByteArray> textroles = QAbstractItemModel::roleNames();

    textroles[nameText] = "nameText";
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


