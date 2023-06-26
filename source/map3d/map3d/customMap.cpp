#include "customMap.h"
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/AnnotationLayer>

MainMapCallback::MainMapCallback(CustomMap *customMap) :
    mCustomMap(customMap)
{

}

void MainMapCallback::onLayerAdded  (osgEarth::Layer* layer, unsigned index)
{
    if(mCustomMap)
        emit mCustomMap->layerAdded(layer, index);
}

void MainMapCallback::onLayerRemoved(osgEarth::Layer* layer, unsigned index)
{
    if(mCustomMap)
        emit mCustomMap->layerRemoved(layer, index);
}

void MainMapCallback::onLayerMoved(osgEarth::Layer* layer, unsigned oldIndex, unsigned newIndex)
{
    if(mCustomMap)
        emit mCustomMap->layerMoved(layer, oldIndex, newIndex);
}

void MainMapCallback::onLayerEnabled(osgEarth::Layer* layer)
{
    if(mCustomMap)
        emit mCustomMap->layerEnabled(layer);
}

void MainMapCallback::onLayerDisabled(osgEarth::Layer* layer)
{
    if(mCustomMap)
        emit mCustomMap->layerDisabled(layer);
}

//----------------------------------------------------------------------------------------------
CustomMap::CustomMap(QObject *parent):
    osgEarth::Map(),
    QObject(parent)
{
    addMapCallback(new MainMapCallback(this));
}

bool CustomMap::addNodeToExistLayer(osg::Node *node, osgEarth::Annotation::AnnotationLayer *layer)
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

bool CustomMap::removeNodeFromExistLayer(osg::Node *node, osgEarth::Annotation::AnnotationLayer *layer)
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

bool CustomMap::addExistLayerToExistLayer(osgEarth::Layer *layer, osgEarth::Layer *destlayer)
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

bool CustomMap::removeExistLayerFromExistLayer(osgEarth::Layer *layer, osgEarth::Layer *destlayer)
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
    layer->getUserData();

    emit layerFromLayerRemoved(layer, destlayer);
    return true;
}
