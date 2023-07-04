#ifndef CUSTOMMAP_H
#define CUSTOMMAP_H

#include <QObject>
#include <osgEarth/Map>

namespace osgEarth {
namespace Annotation {
class AnnotationLayer;
}

}
class MapObject;
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
    bool addNodeToLayer(osg::Node *node, osgEarth::Annotation::AnnotationLayer *layer);
    bool removeNodeFromLayer(osg::Node *node, osgEarth::Annotation::AnnotationLayer *layer);
    bool setParentLayer(osgEarth::Layer *layer, osgEarth::Layer *parentLayer);
    osgEarth::Layer *getParentLayer(osgEarth::Layer *layer);
signals:
    void layerAdded(osgEarth::Layer* layer, unsigned index);
    void layerRemoved(osgEarth::Layer* layer, unsigned index);
    void layerMoved(osgEarth::Layer* layer, unsigned oldIndex, unsigned newIndex);
    void layerEnabled(osgEarth::Layer* layer);
    void layerDisabled(osgEarth::Layer* layer);

    void nodeToLayerAdded(osg::Node *node, osgEarth::Layer *layer);
    void nodeFromLayerRemoved(osg::Node *node, osgEarth::Layer *layer);
    void parentLayerChanged(osgEarth::Layer *layer, osgEarth::Layer *oldParentLayer, osgEarth::Layer *newParentLayer);

private:
};

#endif // CUSTOMMAP_H