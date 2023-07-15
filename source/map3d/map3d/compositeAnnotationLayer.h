#ifndef COMPOSITELAYER_H
#define COMPOSITELAYER_H

#include <osgEarthAnnotation/AnnotationLayer>

#include <osgEarthAnnotation/AnnotationNode>
#include <QObject>
#include <osgEarth/MapCallback>
#include <osgEarth/VisibleLayer>

class CompositeAnnotationLayer;
class ParenticAnnotationLayer: public QObject, public osgEarth::Annotation::AnnotationLayer
{
public:
    ParenticAnnotationLayer(QObject *parent = nullptr);

    osgEarth::Annotation::AnnotationNode *node() const;
    void setNode(osgEarth::Annotation::AnnotationNode *newNode);
    virtual osg::Node* getNode() const;

    bool getIsNode() const;
    ParenticAnnotationLayer *getParentAtIndex(unsigned index);
    unsigned getIndexOfparent(const ParenticAnnotationLayer* layer) const;
    unsigned getNumParents() const;

protected:
    virtual void addParent(CompositeAnnotationLayer* parent);
    virtual void removeParent(CompositeAnnotationLayer* parent);
    void insertParent(CompositeAnnotationLayer *layer, unsigned index);

private:
    std::vector<osg::ref_ptr<CompositeAnnotationLayer>> mParents;
    osg::ref_ptr<osgEarth::Annotation::AnnotationNode> mNode;
    void setIsNode(bool newIsNode);
    bool isNode{true};
    friend class CompositeAnnotationLayer;
};

class CompositeLayerCallback : public QObject, public osgEarth::VisibleLayerCallback
{
    Q_OBJECT
public:
    virtual void onLayerAdded(ParenticAnnotationLayer* layer, CompositeAnnotationLayer* parentLayer){}
    virtual void onLayerRemoved(ParenticAnnotationLayer* layer, CompositeAnnotationLayer* parentLayer){}
    virtual void onLayerMoved(ParenticAnnotationLayer* layer, CompositeAnnotationLayer* parentLayer, unsigned oldIndex, unsigned newIndex){}
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
    unsigned getIndexOfLayer(const ParenticAnnotationLayer* layer) const;
    unsigned getNumLayers() const;
    void addLayer(ParenticAnnotationLayer *layer);
    void insertLayer(ParenticAnnotationLayer *layer, unsigned index);
    void removeLayer(ParenticAnnotationLayer *layer);
    void clearLayers();
    void moveLayer(ParenticAnnotationLayer *layer, unsigned index);
    void removeLayerByName(const QString& layerName);
    ParenticAnnotationLayer *getLayerByName(const QString& layerName);
    void fireCallback(CompositeLayerCallback::MethodPtr, ParenticAnnotationLayer *layer);
    int getNumChildren() const;
    inline ParenticAnnotationLayer* getChild(int i ) { return mChilds[i].get(); }

private:
    osg::ref_ptr<osg::Group> mRoot;
    std::vector<osg::ref_ptr<ParenticAnnotationLayer>> mChilds;
};



#endif // COMPOSITELAYER_H
