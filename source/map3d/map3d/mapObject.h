#ifndef CUSTOMMAP_H
#define CUSTOMMAP_H

#include <QMap>
#include <QObject>
#include <osgEarth/Map>
#include "compositeAnnotationLayer.h"

namespace osgEarth {
namespace Annotation {
class AnnotationLayer;
}

}
class MapObject;

class CompositeCallback: public CompositeLayerCallback
{
    Q_OBJECT
public:
    CompositeCallback(MapObject *mapObject);
    void onLayerAdded(ParenticAnnotationLayer* layer, CompositeAnnotationLayer *parentLayer, unsigned index) override;
    void onLayerRemoved(ParenticAnnotationLayer* layer, CompositeAnnotationLayer *parentLayer, unsigned index) override;
    void onLayerMoved(ParenticAnnotationLayer* layer, CompositeAnnotationLayer* parentLayer, unsigned oldIndex, unsigned newIndex) override;
    void onNodeAdded(osgEarth::Annotation::AnnotationNode *node, ParenticAnnotationLayer *layer) override;
    void onNodeRemoved(osgEarth::Annotation::AnnotationNode *node, ParenticAnnotationLayer *layer) override;

private:
    MapObject *mMapObject{nullptr};
};

//class
//--------------------------------------------
class MainMapCallback : public QObject, public osgEarth::MapCallback
{
    Q_OBJECT
public:
    MainMapCallback(MapObject *mapObject);
    void onLayerAdded(osgEarth::Layer* layer, unsigned index) override;
    void onLayerRemoved(osgEarth::Layer* layer, unsigned index) override;
    void onLayerMoved(osgEarth::Layer* layer, unsigned oldIndex, unsigned newIndex) override;
    void onLayerEnabled(osgEarth::Layer* layer) override;
    void onLayerDisabled(osgEarth::Layer* layer) override;

private:
    MapObject *mMapObject{nullptr};
};
//--------------------------------------------
class MapObject: public QObject, public osgEarth::Map
{
    Q_OBJECT
public:
    MapObject(QObject *parent = nullptr);
    MapObject(const osgEarth::MapOptions& options, QObject *parent = nullptr);
    bool addLayer(osgEarth::Layer* layer, osgEarth::Layer *parentLayer = nullptr);
    bool removeLayer(osgEarth::Layer* layer, osgEarth::Layer *parentLayer = nullptr);
    void addCompositeCallback(osgEarth::Layer* layer, CompositeCallback* callback);
    void removeCompositeCallback(osgEarth::Layer* layer);
    CompositeCallback* getCompositeCallback(osgEarth::Layer* layer);
    ParenticAnnotationLayer *getLayerByUserId(int userid);
    void setFilterManager(FilterManager *newFilterManager);

    void addLayerFromServeice(ParenticAnnotationLayer *layer);
    ParenticAnnotationLayer *getServiceLayerBiId(int id);
    void clearOldParenticLayers();
public slots:
    void clearCompositeLayers();
    void filterNodes();
signals:
    void layerAdded(osgEarth::Layer* layer, osgEarth::Layer* parentLayer, unsigned index);
    void layerRemoved(osgEarth::Layer* layer, osgEarth::Layer* parentLayer, unsigned index);
    void layerMoved(osgEarth::Layer* layer, unsigned oldIndex, unsigned newIndex);
    void layerEnabled(osgEarth::Layer* layer);
    void layerDisabled(osgEarth::Layer* layer);

    void nodeToLayerAdded(osg::Node *node, osgEarth::Layer *layer);
    void nodeFromLayerRemoved(osg::Node *node, osgEarth::Layer *layer);
//    void parentLayerChanged(osgEarth::Layer *layer, osgEarth::Layer *oldParentLayer, osgEarth::Layer *newParentLayer);

private:
    QMap<osgEarth::Layer*, CompositeCallback*> mCompositeCallbacks;
    std::map<int, osg::ref_ptr<ParenticAnnotationLayer>> mParenticLayers;
    std::map<int, osg::ref_ptr<CompositeAnnotationLayer>> mCompositeLayers;
    FilterManager *mFilterManager;
    int mRefrehsTime{0};
};

#endif // CUSTOMMAP_H
