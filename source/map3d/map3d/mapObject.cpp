#include "mapObject.h"
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/AnnotationLayer>
#include <QDebug>
#include "compositeAnnotationLayer.h"

//------------------------------------------------------------------------------
CompositeCallback::CompositeCallback(MapObject *mapObject):
    mMapObject(mapObject)
{

}

void CompositeCallback::onLayerAdded(osgEarth::Annotation::AnnotationLayer *layer, CompositeAnnotationLayer *parentLayer){
    if(mMapObject)
        emit mMapObject->layerAdded(layer, parentLayer, mMapObject->getIndexOfLayer(parentLayer));
}

void CompositeCallback::onLayerRemoved(osgEarth::Annotation::AnnotationLayer *layer, CompositeAnnotationLayer *parentLayer){
    if(mMapObject)
        emit mMapObject->layerRemoved(layer, parentLayer, mMapObject->getIndexOfLayer(parentLayer));
}

//-------------------------------------------------------------------------------------
MainMapCallback::MainMapCallback(MapObject *mapObject) :
    mMapObject(mapObject)
{

}

void MainMapCallback::onLayerAdded  (osgEarth::Layer* layer, unsigned index)
{
    if(mMapObject)
        emit mMapObject->layerAdded(layer, mMapObject->getParentLayer(layer), index);
}

void MainMapCallback::onLayerRemoved(osgEarth::Layer* layer, unsigned index)
{
    if(mMapObject)
        emit mMapObject->layerRemoved(layer, mMapObject->getParentLayer(layer), index);
}

void MainMapCallback::onLayerMoved(osgEarth::Layer* layer, unsigned oldIndex, unsigned newIndex)
{
    if(mMapObject)
        emit mMapObject->layerMoved(layer, oldIndex, newIndex);
}

void MainMapCallback::onLayerEnabled(osgEarth::Layer* layer)
{
    if(mMapObject)
        emit mMapObject->layerEnabled(layer);
}

void MainMapCallback::onLayerDisabled(osgEarth::Layer* layer)
{
    if(mMapObject)
        emit mMapObject->layerDisabled(layer);
}

//----------------------------------------------------------------------------------------------
MapObject::MapObject(QObject *parent):
    osgEarth::Map(),
    QObject(parent)
{
    addMapCallback(new MainMapCallback(this));
}

MapObject::MapObject(const osgEarth::MapOptions &options, QObject *parent):
    osgEarth::Map(options),
    QObject(parent)
{
    addMapCallback(new MainMapCallback(this));
}

bool MapObject::addLayer(osgEarth::Layer *layer, osgEarth::Layer *parentLayer)
{
    if (!layer)
        return false;
    CompositeAnnotationLayer* compositeLayer = dynamic_cast<CompositeAnnotationLayer*>(layer);
    if (compositeLayer){
        auto compositCallback = new CompositeCallback(this);
        mCompositeCallbacks[layer] = compositCallback;
        compositeLayer->addCallback(compositCallback);
    }
//    beginUpdate();
//    endUpdate();
//    if(parentLayer){
//        auto dataContainer = parentLayer->getOrCreateUserDataContainer();
//        dataContainer->addUserObject(layer);
//    }
//    layer->setUserData(parentLayer);
    osgEarth::Map::addLayer(layer);
//    emit layerAdded(layer, parentLayer, getIndexOfLayer(layer));
    return true;
}

bool MapObject::removeLayer(osgEarth::Layer *layer, osgEarth::Layer *parentLayer)
{
    if (!layer)
        return false;

    CompositeAnnotationLayer* compositeLayer = dynamic_cast<CompositeAnnotationLayer*>(layer);
    if (compositeLayer){
        compositeLayer->removeCallback(mCompositeCallbacks[layer]);
        mCompositeCallbacks.remove(layer);
    }
//    auto index = getIndexOfLayer(layer);
//    beginUpdate();
//    osgEarth::Map::removeLayer(layer);
//    endUpdate();
//    if(parentLayer){
//        auto dataContainer = parentLayer->getOrCreateUserDataContainer();
//        auto objectIndex = dataContainer->getUserObjectIndex(layer);
//        dataContainer->removeUserObject(objectIndex);
//    }
    osgEarth::Map::removeLayer(layer);
//    emit layerRemoved(layer, parentLayer, index);
    return true;
}

bool MapObject::addNodeToLayer(osg::Node *node, osgEarth::Annotation::AnnotationLayer *layer)
{
    if (!layer)
        return false;
    //--layer is not in map----------------
//    unsigned int index = getIndexOfLayer(layer);
//    if(index >= getNumLayers())
//        return false;
    //-------------------------------------
    if(!layer->getGroup()->addChild(node))
        return false;
    emit nodeToLayerAdded(node, layer);
    return true;
}

bool MapObject::removeNodeFromLayer(osg::Node *node, osgEarth::Annotation::AnnotationLayer *layer)
{
    if (!layer)
        return false;
    //--layer is not in map----------------
//    unsigned int index = getIndexOfLayer(layer);
//    if(index >= getNumLayers())
//        return false;
    //-------------------------------------
    if(!layer->getGroup()->removeChild(node))
        return false;
    emit nodeFromLayerRemoved(node, layer);
    return true;
}

bool MapObject::setParentLayer(osgEarth::Layer *layer, osgEarth::Layer *parentLayer)
{
    if (!layer)
        return false;
    //--layers are not in map----------------
    unsigned int index = getIndexOfLayer(layer);
    if(index >= getNumLayers())
        return false;
    //-----------------------------------------
    if(parentLayer && getIndexOfLayer(parentLayer)>= getNumLayers())
        return false;
    if(parentLayer){
        auto dataContainer = parentLayer->getOrCreateUserDataContainer();
        dataContainer->addUserObject(layer);
    }
    auto oldParent = getParentLayer(layer);
    if(oldParent){
        auto dataContainer = oldParent->getOrCreateUserDataContainer();
        auto objectIndex = dataContainer->getUserObjectIndex(layer);
        dataContainer->removeUserObject(objectIndex);
    }
    layer->setUserData(parentLayer);

    emit parentLayerChanged(layer, oldParent, parentLayer);
    return true;
}

osgEarth::Layer *MapObject::getParentLayer(osgEarth::Layer *layer)
{

    if (layer)
        return dynamic_cast<osgEarth::Layer*>(layer->getUserData());
    return nullptr;
}
