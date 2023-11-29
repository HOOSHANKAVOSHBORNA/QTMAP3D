#include "searchNodeModel.h"
#include "mapObject.h"
#include "serviceManager.h"

#include <osgEarth/ModelLayer>
#include <osg/Node>
#include <osgEarthAnnotation/GeoPositionNode>

SearchNodeModel::SearchNodeModel(MapItem *mapItem, QObject *parent):
    QAbstractListModel(parent), mMapItem(mapItem)
{
    //    init();

    connect(mMapItem->getMapObject(), &MapObject::nodeToLayerAdded, this , &SearchNodeModel::addNode);
    connect(mMapItem->getMapObject(), &MapObject::nodeFromLayerRemoved,  this , &SearchNodeModel::removeNode);
    mTypeListModel = new TypeListModel;

}

int SearchNodeModel::rowCount(const QModelIndex &parent) const
{

    return mNodes1.size();
}

QVariant SearchNodeModel::data(const QModelIndex &index, int role) const
{

    switch (role) {
    case Qt::DisplayRole:
        return QVariant::fromValue<QString>(QString::fromStdString(mNodes1[index.row()]->data->name));
        break;
    case iD_:
        return mNodes1[index.row()]->data->id ;
        break;
    default:
        return QVariant::fromValue<QString>(QString(""));
        break;
    };
}


void SearchNodeModel::addNode(osg::Node *node, osgEarth::Layer *layer) {
    NodeData *nodeData = dynamic_cast<NodeData*>(node->getUserData());
    if (nodeData) {
        QString typeToAdd = QString::fromStdString(nodeData->type);
        auto typeExists = std::find(mTypeListModel->mTypes.begin(), mTypeListModel->mTypes.end(), typeToAdd);
        if (typeExists == mTypeListModel->mTypes.end()) {
            mTypeListModel->append(typeToAdd);
        }
        auto geonode = dynamic_cast<osgEarth::Annotation::GeoPositionNode*>(node);
        if (std::find(mNodes1.begin(), mNodes1.end(), new NodeInfo{geonode, nodeData}) == mNodes1.end()) {
            beginInsertRows(QModelIndex(), mNodes1.size(), mNodes1.size());
            mNodes1.push_back(new NodeInfo{geonode, nodeData});
            endInsertRows();
        }
    }
}

void SearchNodeModel::removeNode(osg::Node *node, osgEarth::Layer *layer) {
    auto iterator = std::remove_if(
        mNodes1.begin(),
        mNodes1.end(),
        [&](NodeInfo *item) {
            return item->node == node;
        }
        );

    if (iterator != mNodes1.end()) {
        int d = std::distance(mNodes1.begin(), iterator);
        beginRemoveRows(QModelIndex(), d, d);
        delete *iterator;
        mNodes1.erase(iterator);
        endRemoveRows();
        emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
    }
}



void SearchNodeModel::onNodeClicked(const QModelIndex &current) {
    if (current.isValid() && current.row() >= 0 && current.row() < static_cast<int>(mNodes1.size())) {
        osgEarth::Annotation::GeoPositionNode *node = mNodes1[current.row()]->node;
        if (node) {
            mMapItem->getCameraController()->goToPosition(
                node->getPosition(),mMapItem->getCameraController()->getViewpoint().getRange(),0);
        }
    }
}

QHash<int, QByteArray> SearchNodeModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[iD_] = "id_";
    hash[text_]="text_";
    return hash;

}

TypeListModel *SearchNodeModel::getTypeListModel() const
{
    return mTypeListModel;
}

void SearchNodeModel::init()
{
    std::vector<osg::ref_ptr<osgEarth::Layer>> layers;
    mMapItem->getMapObject()->getLayers(layers);
    for (auto& layer : layers){
        ParenticAnnotationLayer* l1 = dynamic_cast<ParenticAnnotationLayer*>(layer.get());
        osgEarth::Annotation::AnnotationLayer* l2 = dynamic_cast<osgEarth::Annotation::AnnotationLayer*>(layer.get());
        if (!(l1 || l2)){
            mNodes.push_back(layer->getNode());
        }
        else{
            if(l1)
                for (int i = 0; i < l1->getNumberOfNodes(); ++i)
                    mNodes.push_back(l1->getGroup()->getChild(i));
            else
                for (int i = 0; i < l1->getNumberOfNodes(); ++i)
                    mNodes.push_back(l2->getGroup()->getChild(i));
        }
    }
}




/////////////////////////////////////////////////////////////////ProxyModel///////////////////////////////
SearchNodeProxyModel::SearchNodeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

bool SearchNodeProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    if (index.data().toString().contains(mFilterString, Qt::CaseInsensitive))
        return true;
    return false;
}

QString SearchNodeProxyModel::filterString() const
{
    return mFilterString;
}


void SearchNodeProxyModel::setFilterString(const QString &filterString)
{
    mFilterString = filterString;
    invalidateFilter();
}

void SearchNodeProxyModel::onNodeClicked(const int current)
{
    QModelIndex mindex = mapToSource(index(current, 0));
    static_cast<SearchNodeModel*>(sourceModel())->onNodeClicked(mindex);
}

TypeListModel *SearchNodeProxyModel::getTypeListModel() const
{
    return dynamic_cast<SearchNodeModel*>(sourceModel())->getTypeListModel();
}



//----------------------------------------------
TypeListModel::TypeListModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

void TypeListModel::append(QString type)
{
    beginInsertRows(QModelIndex(), mTypes.size(), mTypes.size());
    mTypes.push_back(type);
    endInsertRows();

}

QVariant TypeListModel::data(const QModelIndex &index, int role) const
{
    return mTypes[index.row()];
}

int TypeListModel::rowCount(const QModelIndex &parent) const
{
    return mTypes.size();
}
///////////////////////////////////////////////// --------------


//SearchNodeManager::SearchNodeManager(MapItem *mapItem, QObject *parent) : QObject(parent)
//{
//    mSearchNodeModel = new SearchNodeModel(mapItem, this);
//    mSearchNodeProxyModel = new SearchNodeProxyModel(this);
//    mSearchNodeProxyModel->setSourceModel(mSearchNodeModel);
//}

//void SearchNodeManager::addNode(osg::Node *node, osgEarth::Layer *layer)
//{
//    mSearchNodeModel->addNode(node, layer);

//}

//void SearchNodeManager::removeNode(osg::Node *node, osgEarth::Layer *layer)
//{
//    mSearchNodeModel->removeNode(node, layer);
//}

//void SearchNodeManager::onNodeClicked(int current)
//{
//    mSearchNodeProxyModel->onNodeClicked(current);
//}

//SearchNodeModel *SearchNodeManager::getSearchNodeModel() const
//{
//    return mSearchNodeModel;
//}

//SearchNodeProxyModel *SearchNodeManager::getSearchNodeProxyModel() const
//{
//    return mSearchNodeProxyModel;
//}

//TypeListModel *SearchNodeManager::getTypeListModel() const
//{
//    return mSearchNodeModel->getTypeListModel();
//}
