#ifndef COMPOSITELAYER_H
#define COMPOSITELAYER_H

#include <osgEarthAnnotation/AnnotationLayer>

#include <QObject>
#include "mapObject.h"
#include <osgEarth/MapCallback>
#include <osgEarth/VisibleLayer>

class CompositeCallback : public QObject, public osgEarth::VisibleLayerCallback
{
    Q_OBJECT
public:
    CompositeCallback();
    void onVisibleChanged(osgEarth::VisibleLayer* layer) override;
    void onOpacityChanged(osgEarth::VisibleLayer* layer) override;
    void onVisibleRangeChanged(osgEarth::VisibleLayer* layer) override;
//    void onLayerAdded(osgEarth::Layer* layer, unsigned index) override;
//    void onLayerRemoved(osgEarth::Layer* layer, unsigned index) override;
//    void onLayerMoved(osgEarth::Layer* layer, unsigned oldIndex, unsigned newIndex) override;
//    void onLayerEnabled(osgEarth::Layer* layer) override;
//    void onLayerDisabled(osgEarth::Layer* layer) override;
    typedef void (CompositeCallback::*MethodPtr)(class osgEarth::VisibleLayer* layer);

private:
    MapObject *mMapObject{nullptr};
};

class CompositeAnnotationLayer: public QObject, osgEarth::Annotation::AnnotationLayer
{
    Q_OBJECT
public:
    CompositeAnnotationLayer(QObject *parent = nullptr);

    virtual osg::Node* getNode() const override;
    virtual void init() override;

public:
    void addLayer(osgEarth::Annotation::AnnotationLayer *layer);
    void removeLayer(osgEarth::Annotation::AnnotationLayer *layer);
    void fireCallback(CompositeCallback::MethodPtr);
//    void addParent(CompositeAnnotationLayer *layer);

private:
    osg::ref_ptr<osg::Group> _root;
    QList<osg::ref_ptr<osgEarth::Annotation::AnnotationLayer>> mChilds;
//    QList<osg::ref_ptr<CompositeAnnotationLayer>> mParents;
};

#endif // COMPOSITELAYER_H
