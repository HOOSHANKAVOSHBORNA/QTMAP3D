#include "searchNodeModel.h"
#include "mapObject.h"

#include <osgEarth/ModelLayer>


SearchNodeModel::SearchNodeModel(MapObject *mapObject, QObject *parent):
    QAbstractListModel(parent), mMapObject(mapObject)
{
    init();

//    connect(mMapObject, &MapObject::nodeToLayerAdded, )
//    connect(mMapObject, &MapObject::nodeFromLayerRemoved, )
}

int SearchNodeModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant SearchNodeModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
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
