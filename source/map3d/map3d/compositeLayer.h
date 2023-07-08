#ifndef COMPOSITELAYER_H
#define COMPOSITELAYER_H

#include <osgEarthAnnotation/AnnotationLayer>

#include <QObject>
//#include "mapObject.h"
#include <osgEarth/MapCallback>
#include <osgEarth/VisibleLayer>

class CompositeLayerCallback : public QObject, public osgEarth::VisibleLayerCallback
{
    Q_OBJECT
public:
    CompositeLayerCallback();
    virtual void onLayerAdded(osgEarth::Annotation::AnnotationLayer* layer){}
    virtual void onLayerRemoved(osgEarth::Annotation::AnnotationLayer* layer){}
    typedef void (CompositeLayerCallback::*MethodPtr)(class osgEarth::Annotation::AnnotationLayer* layer);
};

class CompositeAnnotationLayer: public QObject, public osgEarth::Annotation::AnnotationLayer
{
    Q_OBJECT
public:
    CompositeAnnotationLayer(QObject *parent = nullptr);

    virtual osg::Node* getNode() const override;
    virtual void init() override;

public:
    void addLayer(osgEarth::Annotation::AnnotationLayer *layer);
    void removeLayer(osgEarth::Annotation::AnnotationLayer *layer);
    void fireCallback(CompositeLayerCallback::MethodPtr, osgEarth::Annotation::AnnotationLayer *layer);
//    void addParent(CompositeAnnotationLayer *layer);

private:
    osg::ref_ptr<osg::Group> _root;
    QList<osg::ref_ptr<osgEarth::Annotation::AnnotationLayer>> mChilds;
//    QList<osg::ref_ptr<CompositeAnnotationLayer>> mParents;
};

#endif // COMPOSITELAYER_H
