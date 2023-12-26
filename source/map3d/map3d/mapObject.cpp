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

void CompositeCallback::onLayerAdded(ParenticAnnotationLayer *layer, CompositeAnnotationLayer *parentLayer, unsigned int index){
    if(mMapObject){
        emit mMapObject->layerAdded(layer, parentLayer, index);
        if (layer->asCompositeAnnotationLayer())
            for (auto &it : layer->asCompositeAnnotationLayer()->childildren())
                layer->asCompositeAnnotationLayer()->fireCallback(&CompositeLayerCallback::onLayerAdded, it, 0);
        else {
            for (auto &it : layer->children()) {
                osg::ref_ptr<osgEarth::Annotation::AnnotationNode> node = dynamic_cast<osgEarth::Annotation::AnnotationNode*>(it);
                if (node)
                    layer->fireCallback(&ParenticLayerCallback::onNodeAdded, node);
            }
        }
    }
}

void CompositeCallback::onLayerRemoved(ParenticAnnotationLayer *layer, CompositeAnnotationLayer *parentLayer, unsigned int index){
    if(mMapObject){
        if (layer->asCompositeAnnotationLayer())
            for (auto &it : layer->asCompositeAnnotationLayer()->childildren())
                layer->asCompositeAnnotationLayer()->fireCallback(&CompositeLayerCallback::onLayerRemoved, it, 0);
        else {
            for (auto &it : layer->children()) {
                osg::ref_ptr<osgEarth::Annotation::AnnotationNode> node = dynamic_cast<osgEarth::Annotation::AnnotationNode*>(it);
                if (node)
                    layer->fireCallback(&ParenticLayerCallback::onNodeRemoved, node);
            }
        }
        emit mMapObject->layerRemoved(layer, parentLayer, index);
    }
}

void CompositeCallback::onLayerMoved(ParenticAnnotationLayer *layer, CompositeAnnotationLayer *parentLayer, unsigned int oldIndex, unsigned int newIndex)
{
    if (mMapObject)
        emit mMapObject->layerMoved(layer, oldIndex, newIndex);
}

void CompositeCallback::onNodeAdded(osgEarth::Annotation::AnnotationNode *node, ParenticAnnotationLayer *layer)
{
    if (mMapObject)
        emit mMapObject->nodeToLayerAdded(node, layer);
}

void CompositeCallback::onNodeRemoved(osgEarth::Annotation::AnnotationNode *node, ParenticAnnotationLayer *layer)
{
    if (mMapObject)
        emit mMapObject->nodeFromLayerRemoved(node, layer);
}

//-------------------------------------------------------------------------------------
MainMapCallback::MainMapCallback(MapObject *mapObject) :
    mMapObject(mapObject)
{

}

void MainMapCallback::onLayerAdded(osgEarth::Layer* layer, unsigned index)
{
    if(mMapObject)
        emit mMapObject->layerAdded(layer, nullptr, index);

    ParenticAnnotationLayer* parenticLayer = dynamic_cast<ParenticAnnotationLayer*>(layer);
    if (parenticLayer) {
        if (parenticLayer->asCompositeAnnotationLayer())
            for (auto &it : parenticLayer->asCompositeAnnotationLayer()->childildren())
                parenticLayer->asCompositeAnnotationLayer()->fireCallback(&CompositeLayerCallback::onLayerAdded, it, 0);
        else {
            for (auto &it : parenticLayer->children()) {
                osg::ref_ptr<osgEarth::Annotation::AnnotationNode> node = dynamic_cast<osgEarth::Annotation::AnnotationNode*>(it);
                if (node)
                    parenticLayer->fireCallback(&ParenticLayerCallback::onNodeAdded, node);
            }
        }
    }
}

void MainMapCallback::onLayerRemoved(osgEarth::Layer* layer, unsigned index)
{
    if(mMapObject) {
        ParenticAnnotationLayer* parenticLayer = dynamic_cast<ParenticAnnotationLayer*>(layer);
        if (parenticLayer) {
            if (parenticLayer->asCompositeAnnotationLayer())
                for (auto &it : parenticLayer->asCompositeAnnotationLayer()->childildren())
                    parenticLayer->asCompositeAnnotationLayer()->fireCallback(&CompositeLayerCallback::onLayerRemoved, it, 0);
            else {
                for (auto &it : parenticLayer->children()) {
                    osg::ref_ptr<osgEarth::Annotation::AnnotationNode> node = dynamic_cast<osgEarth::Annotation::AnnotationNode*>(it);
                    if (node)
                        parenticLayer->fireCallback(&ParenticLayerCallback::onNodeRemoved, node);
                }
            }

        }
        emit mMapObject->layerRemoved(layer, nullptr, index);
    }
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
        if(compositeLayer->userId() >= 0)
            mParenticLayers[compositeLayer->userId()] = compositeLayer;

        auto compositCallback = new CompositeCallback(this);
        mCompositeCallbacks[layer] = compositCallback;
        compositeLayer->addCallback(compositCallback);

    }
    osgEarth::Map::addLayer(layer);
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
    osgEarth::Map::removeLayer(layer);
    return true;
}

void MapObject::addCompositeCallback(osgEarth::Layer* layer, CompositeCallback* callback)
{
    mCompositeCallbacks[layer] = callback;
}

void MapObject::removeCompositeCallback(osgEarth::Layer* layer)
{
    mCompositeCallbacks.remove(layer);
}

CompositeCallback *MapObject::getCompositeCallback(osgEarth::Layer *layer)
{
    return mCompositeCallbacks[layer];
}

ParenticAnnotationLayer *MapObject::getLayerByUserId(int userid)
{
    for (auto& l: mParenticLayers){
        ParenticAnnotationLayer *p = l.second->asCompositeAnnotationLayer()->getHierarchicalLayerByUserId(userid);
        if (p){
            return p;
        }
    }
    return nullptr;
}

void MapObject::clearParenticLayers()
{
    for (auto &i : mParenticLayers){
        removeLayer(i.second);
    }
}
