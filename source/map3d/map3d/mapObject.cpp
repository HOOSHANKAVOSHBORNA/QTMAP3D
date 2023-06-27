#include "mapObject.h"
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/AnnotationLayer>

MainMapCallback::MainMapCallback(MapObject *mapObject) :
    mMapObject(mapObject)
{

}

void MainMapCallback::onLayerAdded  (osgEarth::Layer* layer, unsigned index)
{
    if(mMapObject)
        emit mMapObject->layerAdded(layer, index);
}

void MainMapCallback::onLayerRemoved(osgEarth::Layer* layer, unsigned index)
{
    if(mMapObject)
        emit mMapObject->layerRemoved(layer, index);
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

bool MapObject::addNodeToExistLayer(osg::Node *node, osgEarth::Annotation::AnnotationLayer *layer)
{
    if (!layer)
        return false;
    //--layer is not in map----------------
    unsigned int index = getIndexOfLayer(layer);
    if(index >= getNumLayers())
        return false;
    //-------------------------------------
    layer->getGroup()->addChild(node);
    emit nodeToLayerAdded(node, layer);
    return true;
}

bool MapObject::removeNodeFromExistLayer(osg::Node *node, osgEarth::Annotation::AnnotationLayer *layer)
{
    if (!layer)
        return false;
    //--layer is not in map----------------
    unsigned int index = getIndexOfLayer(layer);
    if(index >= getNumLayers())
        return false;
    //-------------------------------------
    layer->getGroup()->removeChild(node);
    emit nodeFromLayerRemoved(node, layer);
    return true;
}

bool MapObject::addExistLayerToExistLayer(osgEarth::Layer *layer, osgEarth::Layer *destlayer)
{
    if (!layer || destlayer)
        return false;
    //--layers are not in map----------------
    unsigned int indexDest = getIndexOfLayer(destlayer);
    unsigned int index = getIndexOfLayer(layer);
    if(index >= getNumLayers() || indexDest >= getNumLayers())
        return false;
    //-----------------------------------------
    auto dataContainer = destlayer->getOrCreateUserDataContainer();
    dataContainer->addUserObject(layer);
    layer->setUserData(destlayer);

    emit layerToLayerAdded(layer, destlayer);
    return true;
}

bool MapObject::removeExistLayerFromExistLayer(osgEarth::Layer *layer, osgEarth::Layer *destlayer)
{
    if (!layer || destlayer)
        return false;
    //--layers are not in map----------------
    unsigned int indexDest = getIndexOfLayer(destlayer);
    unsigned int index = getIndexOfLayer(layer);
    if(index >= getNumLayers() || indexDest >= getNumLayers())
        return false;
    //-----------------------------------------
    auto dataContainer = destlayer->getOrCreateUserDataContainer();
    unsigned int indexChild = dataContainer->getUserObjectIndex(layer);
    dataContainer->removeUserObject(indexChild);
    layer->setUserData(nullptr);

    emit layerFromLayerRemoved(layer, destlayer);
    return true;
}
