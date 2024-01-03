#include "nodeInformation.h"
#include "qquickwindow.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlComponent>

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

NodeInformation::NodeInformation(QQmlEngine *Engine, QQuickWindow *parent):QObject(parent)
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

NodeInformation::~NodeInformation()
{

}

void NodeInformation::setNodeData(NodeData *nodeData)
{
    mWindow->setTitle(QString::fromStdString(nodeData->name));

    QString iconUrl = QString::fromStdString(nodeData->iconSrc);
    if(!iconUrl.contains("qrc"))
        iconUrl = "file:" + iconUrl;
    mWindow->setProperty("iconUrl", iconUrl);

    QString imgUrl = QString::fromStdString(nodeData->imgSrc);
    if(!imgUrl.contains("qrc"))
        imgUrl = "file:" + imgUrl;
    mWindow->setProperty("imageUrl", imgUrl);

    mNodeInformationModel->setNodeData(nodeData);
}

void NodeInformation::show()
{
    mWindow->show();
    mWindow->requestActivate();
}

NodeInformationModel *NodeInformation::model() const
{
    return mNodeInformationModel;
}

QQuickWindow *NodeInformation::window() const
{
    return mWindow;
}

