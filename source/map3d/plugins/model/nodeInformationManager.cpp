#include "nodeInformationManager.h"
#include "qquickwindow.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickView>

NodeInformationModel::NodeInformationModel(QObject *parent):QStandardItemModel(parent)
{
    setColumnCount(1);
    mRootItem = invisibleRootItem();
}

NodeInformationModel::~NodeInformationModel()
{

}

void NodeInformationModel::setNodeData(NodeData *nodeData)
{
    mNodeData = nodeData;
    // --clear category child and remove category if is not in new data ---------------
    for (auto& [categoryText, categoryItem] : mCategoryItemMap) {
        categoryItem->removeRows(0, categoryItem->rowCount());
        auto it = std::find_if(nodeData->fieldData.begin(), nodeData->fieldData.end(), [&](const NodeFieldData& n){
            return n.category == categoryText;
        });
        if (it == nodeData->fieldData.end())
            mCategoryItemMap.erase(categoryText);
    }

    for(NodeFieldData& nodeFieldData: nodeData->fieldData){
        QStandardItem *item = new QStandardItem;
        QString category = nodeFieldData.category;
        // --add category if it doesn't exist ---------------------------------------------------
        if (mCategoryItemMap.find(category) == mCategoryItemMap.end()){
            QStandardItem *categoryItem = new QStandardItem(category);
            categoryItem->setData(QVariant::fromValue(nodeFieldData.categoryIconSrc), iconImageSource);
            mCategoryItemMap[category] = categoryItem;
            mRootItem->appendRow(categoryItem);
        }
        item->setData(QVariant::fromValue(nodeFieldData.name), nameText);
        item->setData(QVariant::fromValue(nodeFieldData.value), valueText);
        mCategoryItemMap[category]->appendRow(item);
    }
}


QHash<int, QByteArray> NodeInformationModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();

    hash[nameText] = "nameText";
    hash[valueText] = "valueText";
    hash[iconImageSource] = "iconImageSource";
    return hash;
}

//QString NodeInformationModel::imageUrl() const
//{
//    return mNodeData ? QString::fromStdString(mNodeData->imgSrc) : "";
//}

//QString NodeInformationModel::icnUrl() const
//{
//    return mNodeData ? QString::fromStdString(mNodeData->iconSrc) : "";
//}

//QString NodeInformationModel::title() const
//{
//    return mNodeData ? QString::fromStdString(mNodeData->name) : "";
//}

//bool NodeInformationModel::bookmarkStatus() const
//{
//    return mBookmarkStatus;
//}

//void NodeInformationModel::changeBookmarkStatus(bool status)
//{
//    mBookmarkStatus = status;
//    emit bookmarkChecked(mBookmarkStatus);
//}

NodeInformationManager::NodeInformationManager(QQmlEngine *Engine, QQuickWindow *parent):QObject(parent)
{


    mNodeInformationModel = new NodeInformationModel(Engine);

    QQmlComponent* comp = new QQmlComponent(Engine, this);
    QObject::connect(comp, &QQmlComponent::statusChanged, [&](const QQmlComponent::Status &status){
        if(status == QQmlComponent::Error){
            qDebug()<<"Can not load this: "<<comp->errorString();
        }

        if(status == QQmlComponent::Ready){
            mWindow = qobject_cast<QQuickWindow*>(comp->create());
            mWindow->setProperty("nodeInfoModel", QVariant::fromValue<NodeInformationModel*>(mNodeInformationModel));

            connect(parent, &QQuickWindow::closing, mWindow, &QQuickWindow::close);
            connect(mWindow, SIGNAL(goToPosition()), this, SIGNAL(goToPosition()));
            connect(mWindow, SIGNAL(track()), this, SIGNAL(track()));
        }
    });
    comp->loadUrl(QUrl("qrc:/NodeInformation.qml"));

}

NodeInformationManager::~NodeInformationManager()
{

}

void NodeInformationManager::setNodeData(NodeData *nodeData)
{
    qDebug()<<QString::fromStdString(nodeData->imgSrc);
    mWindow->setTitle(QString::fromStdString(nodeData->name));
    mWindow->setProperty("iconUrl", QVariant(QString::fromStdString(nodeData->iconSrc)));
    mWindow->setProperty("imageUrl", QVariant::fromValue<QString>(QString::fromStdString(nodeData->imgSrc)));

    mNodeInformationModel->setNodeData(nodeData);
}

void NodeInformationManager::show()
{
    mWindow->show();
    mWindow->requestActivate();
}

NodeInformationModel *NodeInformationManager::nodeInformationModel() const
{
    return mNodeInformationModel;
}

//void NodeInformationManager::changeBookmarkStatus(bool status)
//{
////    mNodeInformationModel->changeBookmarkStatus(status);
//}

QQuickWindow *NodeInformationManager::window() const
{
    return mWindow;
}

