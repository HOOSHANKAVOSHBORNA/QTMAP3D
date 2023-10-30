#include "nodeInformation.h"
#include "qquickwindow.h"

#include <QQmlEngine>

NodeInformation::NodeInformation(QQmlEngine* Engine,QObject *parent)
{
    setColumnCount(1);
    rootItem = invisibleRootItem();

    QQmlComponent* comp = new QQmlComponent(Engine, this);
    QObject::connect(comp, &QQmlComponent::statusChanged, [&](const QQmlComponent::Status &status){
        if(status == QQmlComponent::Error){
            qDebug()<<"Can not load this: "<<comp->errorString();
        }

        if(status == QQmlComponent::Ready){
            mWnd = qobject_cast<QQuickWindow*>(comp->create());
        }
    });
    comp->loadUrl(QUrl("qrc:/NodeInformation.qml"));
    mWnd->setProperty("nodeinfo", QVariant::fromValue<NodeInformation*>(this));
}

NodeInformation::~NodeInformation()
{

}

void NodeInformation::addUpdateNodeInformationItem(NodeData *nodeData)
{
    mNodeData = nodeData;
    emit informationChanged();
    // ----------------------------------------remove category if is not in new data ---------------
    for (auto& [key, value] : mCategories) {
        value->removeRows(0, value->rowCount());
        auto it = std::find_if(nodeData->fieldData.begin(), nodeData->fieldData.end(), [&](const NodeFieldData& n){
            return n.name == key;
        });
        if (it != nodeData->fieldData.end())
            mCategories.erase(it->name);
    }

    for(NodeFieldData nodeFieldData:nodeData->fieldData){
        QStandardItem *item = new QStandardItem;
        QString category = nodeFieldData.category;
        // -----------------add category if it doesn't exist ---------------------------------------------------
        if (mCategories.find(category) == mCategories.end()){
            QStandardItem *p = new QStandardItem(category);
            p->setData(QVariant::fromValue(nodeFieldData.categorySrc), iconImageSource);
            mCategories[category] = p;
            rootItem->appendRow(p);
        }
        item->setData(QVariant::fromValue(nodeFieldData.name), nameText);
        item->setData(QVariant::fromValue(nodeFieldData.value), valueText);
        mCategories[category]->appendRow(item);
    }
}

QHash<int, QByteArray> NodeInformation::roleNames() const
{
    QHash<int, QByteArray> textroles = QAbstractItemModel::roleNames();

    textroles[nameText] = "nameText";
    textroles[valueText] = "valueText";
    textroles[iconImageSource] = "iconImageSource";
    return textroles;
}

void NodeInformation::show()
{
    mWnd->show();
}

QQuickWindow *NodeInformation::wnd() const
{
    return mWnd;
}

QString NodeInformation::imageUrl() const
{
    return mNodeData ? QString::fromStdString(mNodeData->imgSrc) : "";
}

QString NodeInformation::icnUrl() const
{
    return mNodeData ? QString::fromStdString(mNodeData->iconSrc) : "";
}

QString NodeInformation::title() const
{
    return mNodeData ? QString::fromStdString(mNodeData->name) : "";
}

bool NodeInformation::bookmarkStatus() const
{
    return mBookmarkStatus;
}

void NodeInformation::changeBookmarkStatus(bool status)
{
    mBookmarkStatus = status;
    emit bookmarkChecked(mBookmarkStatus);
}


