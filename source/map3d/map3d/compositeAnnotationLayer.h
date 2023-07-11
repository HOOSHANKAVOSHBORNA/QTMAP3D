#ifndef COMPOSITELAYER_H
#define COMPOSITELAYER_H

#include <osgEarthAnnotation/AnnotationLayer>

#include <QObject>
//#include "mapObject.h"
#include <osgEarth/MapCallback>
#include <osgEarth/VisibleLayer>

class CompositeAnnotationLayer;
class CompositeLayerCallback : public QObject, public osgEarth::VisibleLayerCallback
{
    Q_OBJECT
public:
//    CompositeLayerCallback();
    virtual void onLayerAdded(osgEarth::Annotation::AnnotationLayer* layer, CompositeAnnotationLayer* parentLayer){}
    virtual void onLayerRemoved(osgEarth::Annotation::AnnotationLayer* layer, CompositeAnnotationLayer* parentLayer){}
    typedef void (CompositeLayerCallback::*MethodPtr)(class osgEarth::Annotation::AnnotationLayer* layer, class CompositeAnnotationLayer* parentLayer);
};

class CompositeAnnotationLayer: public QObject, public osgEarth::Annotation::AnnotationLayer
{
    Q_OBJECT
public:
    CompositeAnnotationLayer(QObject *parent = nullptr);

    virtual osg::Node* getNode() const override;
    virtual void init() override;

public:
    virtual void setVisible(bool value) override;
    virtual void setOpacity(float value) override;
    void addLayer(osgEarth::Annotation::AnnotationLayer *layer);
    void removeLayer(osgEarth::Annotation::AnnotationLayer *layer);
    void fireCallback(CompositeLayerCallback::MethodPtr, osgEarth::Annotation::AnnotationLayer *layer);
//    void addParent(CompositeAnnotationLayer *layer);
    int getNumChildren() const;
    inline osgEarth::Annotation::AnnotationLayer* getChild(int i ) { return mChilds[i].get(); }

private:
    osg::ref_ptr<osg::Group> mRoot;
    QList<osg::ref_ptr<osgEarth::Annotation::AnnotationLayer>> mChilds;
};

#endif // COMPOSITELAYER_H
