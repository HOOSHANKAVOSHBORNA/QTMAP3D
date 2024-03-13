#include "mapObject.h"
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/AnnotationLayer>
#include <QDebug>
#include "compositeAnnotationLayer.h"
#include "filterManager.h"

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
    if (mMapObject) {
        emit mMapObject->nodeToLayerAdded(node, layer);
    }
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

MapObject::~MapObject()
{
    qDebug() << "~MapObject";
}

MapObject::MapObject(const osgEarth::MapOptions &options, QObject *parent):
    osgEarth::Map(options),
    QObject(parent)
{
    addMapCallback(new MainMapCallback(this));
    mTimerFilterUpdate = new QTimer(this);
    connect(mTimerFilterUpdate, &QTimer::timeout, mFilterManager, &FilterManager::filterTagsEdited);
    mTimerFilterUpdate->start(1000);
}

void MapObject::setServiceManager(ServiceManager *serviceManager)
{
    if(mServiceManager)
        disconnect(mServiceManager, nullptr, this, nullptr);
    mServiceManager = serviceManager;
    if(mServiceManager){
        connect(mServiceManager, &ServiceManager::layerDataReceived, this, &MapObject::onLayerDataReceived);
    }
}

bool MapObject::addLayer(osgEarth::Layer *layer, osgEarth::Layer *parentLayer)
{
    if (!layer)
        return false;
    CompositeAnnotationLayer* compositeLayer = dynamic_cast<CompositeAnnotationLayer*>(layer);
    if (compositeLayer){
        if(compositeLayer->userId() >= 0)
            mLayerMap[compositeLayer->userId()] = compositeLayer;

        auto compositCallback = new CompositeCallback(this);
        mCompositeCallbacks[layer] = compositCallback;
        compositeLayer->addCallback(compositCallback);

        compositeLayer->setFilterManager(mFilterManager);
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
    if(mLayerMap.contains(userid))
        return mLayerMap[userid];
    return nullptr;
}

void MapObject::setFilterManager(FilterManager *newFilterManager)
{
    mFilterManager = newFilterManager;
    for (auto &l: mLayerMap) {
        l->setFilterManager(newFilterManager);
    }
    connect(mFilterManager, &FilterManager::filterTagsEdited, this, &MapObject::filterNodes);
}

ParenticAnnotationLayer *MapObject::getServiceLayerBiId(int id)
{
    ParenticAnnotationLayer *p{nullptr};
    if (mLayerMap.find(id) != mLayerMap.end())
        p = mLayerMap[id];
    return p;
}

void MapObject::clearLayers()
{
    clear();
    mLayerMap.clear();
}

void MapObject::setRefreshStatus(bool status)
{
    if(status)
        mTimerFilterUpdate->start(1000);
    else
        mTimerFilterUpdate->stop();
}

void MapObject::filterNodes()
{
    for (auto &l: mLayerMap) {
        l->filter();
    }
}

void MapObject::onLayerDataReceived(const LayerData &layerData)
{
    if(layerData.command == Command::Add){
        if(mLayerMap.contains(layerData.id)){
            auto layer = mLayerMap[layerData.id];
            if(layer->getNumParents() == 0 && layerData.parentId == -1){
                updateLayerData(layerData);
                return;
            }
            else if(layer->getNumParents() > 0)
                for(int i = 0; i < layer->getNumParents(); ++i)
                    if(layerData.parentId == layer->getParentAtIndex(i)->userId()){
                        updateLayerData(layerData);
                        return;
                    }
        }
        addLayerData(layerData);
    }
    else if(layerData.command == Command::Update)
        updateLayerData(layerData);
    else if(layerData.command == Command::Remove)
        removeLayerData(layerData);
}

void MapObject::addLayerData(const LayerData &layerData)
{
    //--parentic layer--------------------------------------
    if(!layerData.isComposite){
        osg::ref_ptr<ParenticAnnotationLayer> parenticLayer;
        if(mLayerMap.contains(layerData.id))
            parenticLayer = mLayerMap[layerData.id];
        else{
            parenticLayer = new ParenticAnnotationLayer(layerData.id);
            parenticLayer->setName(layerData.text.toStdString());
            parenticLayer->setOrder(layerData.order);
        }

        if(layerData.parentId == -1){
            addLayer(parenticLayer);
            mLayerMap[layerData.id] = parenticLayer;
        }
        else if(mLayerMap.contains(layerData.parentId)){
            auto parentLayer = mLayerMap[layerData.parentId]->asCompositeAnnotationLayer();
            if(parentLayer){
                parentLayer->addLayer(parenticLayer);
                mLayerMap[layerData.id] = parenticLayer;
            }
            else
                qDebug()<<"Parent is not composit layer: "<<layerData.id<<":"<<layerData.text;
        }
        else
            qDebug()<<"Parent not found: "<<layerData.id<<":"<<layerData.text;
    }
    //composit layer-----------------------------------------
    else{
        osg::ref_ptr<CompositeAnnotationLayer> compositLayer;
        if(mLayerMap.contains(layerData.id)){
            compositLayer = mLayerMap[layerData.id]->asCompositeAnnotationLayer();
            if(!compositLayer){
                qDebug()<<"Layer is not composit layer: "<<layerData.id<<":"<<layerData.text;
                return;
            }
        }
        else{
            compositLayer = new CompositeAnnotationLayer(layerData.id);
            compositLayer->setName(layerData.text.toStdString());
            compositLayer->setOrder(layerData.order);
        }

        if(layerData.parentId == -1){
            addLayer(compositLayer);
            mLayerMap[layerData.id] = compositLayer;
            //check in layer data receive
            for(const LayerData& layerData: layerData.children)
                onLayerDataReceived(layerData);
        }
        else if(mLayerMap.contains(layerData.parentId)){
            auto parentLayer = mLayerMap[layerData.parentId]->asCompositeAnnotationLayer();
            if(parentLayer){
                parentLayer->addLayer(compositLayer);
                mLayerMap[layerData.id] = compositLayer;
                for(const LayerData& layerData: layerData.children)
                    onLayerDataReceived(layerData);
            }
            else
                qDebug()<<"Parent is not composit layer: "<<layerData.id<<":"<<layerData.text;
        }
        else
            qDebug()<<"Parent not found: "<<layerData.id<<":"<<layerData.text;
    }
}

void MapObject::updateLayerData(const LayerData &layerData)
{
    if(mLayerMap.contains(layerData.id)){
        auto layer = mLayerMap[layerData.id];
        layer->setName(layerData.text.toStdString());
        layer->setOrder(layerData.order);

        for(const LayerData& layerData: layerData.children)
            onLayerDataReceived(layerData);

    }
}

void MapObject::removeLayerData(const LayerData &layerData)
{
    if(mLayerMap.contains(layerData.id)){
        auto layer = mLayerMap[layerData.id];
        if(layerData.parentId == -1){
            removeLayer(layer);
            for(const LayerData& layerData: layerData.children)
                removeLayerData(layerData);
            mLayerMap.remove(layerData.id);
        }
        else if(mLayerMap.contains(layerData.parentId)){
            auto parentLayer = mLayerMap[layerData.parentId]->asCompositeAnnotationLayer();
            if(parentLayer){
                parentLayer->removeLayer(layer);
                for(const LayerData& layerData: layerData.children)
                    removeLayerData(layerData);
                if(layer->getNumParents() == 0)
                    mLayerMap.remove(layerData.id);
            }
            else
                qDebug()<<"Parent is not composit layer: "<<layerData.id<<":"<<layerData.text;
        }
        else
            qDebug()<<"Parent not found: "<<layerData.id<<":"<<layerData.text;

    }
}

