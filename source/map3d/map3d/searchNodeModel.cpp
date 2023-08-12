#include "searchNodeModel.h"
#include "mapObject.h"

#include <osgEarth/ModelLayer>
#include <osg/Node>

SearchNodeModel::SearchNodeModel(MapObject *mapObject, QObject *parent):
    QAbstractListModel(parent), mMapObject(mapObject)
{
//    init();

    connect(mMapObject, &MapObject::nodeToLayerAdded, this , &SearchNodeModel::addNode);
    connect(mMapObject, &MapObject::nodeFromLayerRemoved,  this , &SearchNodeModel::removeNode);
}

int SearchNodeModel::rowCount(const QModelIndex &parent) const
{
//    qDebug() << "addddd";
    return mNodes.size();
}

QVariant SearchNodeModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case displayTextt:
        return QVariant::fromValue<QString>(QString::fromStdString(mNodes[index.row()].get()->getName()));
        break;
    default:
        return QVariant::fromValue<QString>(QString(""));
        break;
    };
}

QHash<int, QByteArray> SearchNodeModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[displayTextt] = "displayText";
    return hash;
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

void SearchNodeModel::init()
{
    std::vector< osg::ref_ptr<osgEarth::Layer>> layers;
    mMapObject->getLayers(layers);
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
