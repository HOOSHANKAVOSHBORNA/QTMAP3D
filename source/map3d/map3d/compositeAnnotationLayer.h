#ifndef COMPOSITELAYER_H
#define COMPOSITELAYER_H

#include <osgEarthAnnotation/AnnotationLayer>

#include <QObject>
//#include "mapObject.h"
#include <osgEarth/MapCallback>
#include <osgEarth/VisibleLayer>
#include "parenticAnnotationLayer.h"
class CompositeAnnotationLayer;
class CompositeLayerCallback : public QObject, public osgEarth::VisibleLayerCallback
{
    Q_OBJECT
public:
//    CompositeLayerCallback();
    virtual void onLayerAdded(ParenticAnnotationLayer* layer, CompositeAnnotationLayer* parentLayer){}
    virtual void onLayerRemoved(ParenticAnnotationLayer* layer, CompositeAnnotationLayer* parentLayer){}
    typedef void (CompositeLayerCallback::*MethodPtr)(class ParenticAnnotationLayer* layer, class CompositeAnnotationLayer* parentLayer);
};

class CompositeAnnotationLayer: public ParenticAnnotationLayer
{
    Q_OBJECT
public:
    CompositeAnnotationLayer(QObject *parent = nullptr);

    virtual osg::Node* getNode() const override;
    virtual void init() override;

public:
    virtual void setVisible(bool value) override;
    virtual void setOpacity(float value) override;
    void addLayer(ParenticAnnotationLayer *layer);
    void removeLayer(ParenticAnnotationLayer *layer);
    void removeLayerByName(const QString& layerName);
    ParenticAnnotationLayer *getLayerByName(const QString& layerName);
    void fireCallback(CompositeLayerCallback::MethodPtr, ParenticAnnotationLayer *layer);
//    void addParent(CompositeAnnotationLayer *layer);
    int getNumChildren() const;
    inline ParenticAnnotationLayer* getChild(int i ) { return mChilds[i].get(); }

private:
    osg::ref_ptr<osg::Group> mRoot;
    std::vector<osg::ref_ptr<ParenticAnnotationLayer>> mChilds;
};

#endif // COMPOSITELAYER_H
