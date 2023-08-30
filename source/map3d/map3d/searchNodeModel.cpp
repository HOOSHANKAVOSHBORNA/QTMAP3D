#include "searchNodeModel.h"
#include "mapObject.h"

#include <osgEarth/ModelLayer>
#include <osg/Node>
#include <osgEarthAnnotation/GeoPositionNode>

SearchNodeModel::SearchNodeModel(MapItem *mapItem, QObject *parent):
    QAbstractListModel(parent), mMapItem(mapItem)
{
//    init();

    connect(mMapItem->getMapObject(), &MapObject::nodeToLayerAdded, this , &SearchNodeModel::addNode);
    connect(mMapItem->getMapObject(), &MapObject::nodeFromLayerRemoved,  this , &SearchNodeModel::removeNode);
}

int SearchNodeModel::rowCount(const QModelIndex &parent) const
{
//    qDebug() << "addddd";
    return mNodes.size();
}

QVariant SearchNodeModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return QVariant::fromValue<QString>(QString::fromStdString(mNodes[index.row()].get()->getName()));
        break;
    default:
        return QVariant::fromValue<QString>(QString(""));
        break;
    };
}


void SearchNodeModel::addNode(osg::Node *node, osgEarth::Layer *layer)
{
    beginResetModel();
//    beginInsertRows(createIndex(0, 0), mNodes.size(), mNodes.size()+1);
    mNodes.push_back(node);
//    endInsertRows();
    endResetModel();
}

void SearchNodeModel::removeNode(osg::Node *node, osgEarth::Layer *layer)
{
    beginResetModel();
    auto iterator = std::remove_if(mNodes.begin(),mNodes.end(),[&](const osg::Node* item){
        return node == item;

    });

    if (iterator!= mNodes.end())
        mNodes.erase(iterator);
    endResetModel();
}

void SearchNodeModel::onNodeClicked(const QModelIndex &current)
{
    osgEarth::Annotation::GeoPositionNode *node = dynamic_cast<osgEarth::Annotation::GeoPositionNode*>(mNodes[current.row()].get());
    if (node){
        mMapItem->getCameraController()->goToPosition(node->getPosition(),
                                                      mMapItem->getCameraController()->getViewpoint().getRange(), 0);
    }
}

void SearchNodeModel::init()
{
    std::vector< osg::ref_ptr<osgEarth::Layer>> layers;
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



