#ifndef CUSTOMMAP_H
#define CUSTOMMAP_H

#include <QObject>
#include <osgEarth/Map>

namespace osgEarth {
namespace Annotation {
class AnnotationLayer;
}

}
class CustomMap;
//--------------------------------------------
class MainMapCallback : public QObject, public osgEarth::MapCallback
{
    Q_OBJECT
public:
    MainMapCallback(CustomMap *customMap);
    void onLayerAdded(osgEarth::Layer* layer, unsigned index) override;
    void onLayerRemoved(osgEarth::Layer* layer, unsigned index) override;
    void onLayerMoved(osgEarth::Layer* layer, unsigned oldIndex, unsigned newIndex) override;
    void onLayerEnabled(osgEarth::Layer* layer) override;
    void onLayerDisabled(osgEarth::Layer* layer) override;

private:
    CustomMap *mCustomMap{nullptr};
};
//--------------------------------------------
class CustomMap: public QObject, public osgEarth::Map
{
    Q_OBJECT
public:
    CustomMap(QObject *parent = nullptr);
    CustomMap(const osgEarth::MapOptions& options, QObject *parent = nullptr);
    bool addNodeToExistLayer(osg::Node *node, osgEarth::Annotation::AnnotationLayer *layer);
    bool removeNodeFromExistLayer(osg::Node *node, osgEarth::Annotation::AnnotationLayer *layer);
    bool addExistLayerToExistLayer(osgEarth::Layer *layer, osgEarth::Layer* destlayer);
    bool removeExistLayerFromExistLayer(osgEarth::Layer *layer, osgEarth::Layer *destlayer);
signals:
    void layerAdded(osgEarth::Layer* layer, unsigned index);
    void layerRemoved(osgEarth::Layer* layer, unsigned index);
    void layerMoved(osgEarth::Layer* layer, unsigned oldIndex, unsigned newIndex);
    void layerEnabled(osgEarth::Layer* layer);
    void layerDisabled(osgEarth::Layer* layer);

    void nodeToLayerAdded(osg::Node *node, osgEarth::Layer* layer);
    void nodeFromLayerRemoved(osg::Node *node, osgEarth::Layer* layer);
    void layerToLayerAdded(osgEarth::Layer *layer, osgEarth::Layer* destlayer);
    void layerFromLayerRemoved(osgEarth::Layer *layer, osgEarth::Layer *destlayer);

private:
};

#endif // CUSTOMMAP_H
