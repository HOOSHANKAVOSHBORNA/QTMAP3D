#ifndef COMPOSITELAYER_H
#define COMPOSITELAYER_H

#include <osgEarthAnnotation/AnnotationLayer>

#include <osgEarthAnnotation/AnnotationNode>
#include <QObject>
#include <osgEarth/MapCallback>
#include <osgEarth/VisibleLayer>

class CompositeAnnotationLayer;
class ParenticAnnotationLayer;

class ParenticLayerCallback : public QObject, public osgEarth::VisibleLayerCallback
{
    Q_OBJECT
public:
    virtual void onNodeAdded(osgEarth::Annotation::AnnotationNode *node, ParenticAnnotationLayer *layer){};
    virtual void onNodeRemoved(osgEarth::Annotation::AnnotationNode *node, ParenticAnnotationLayer *layer){};
    typedef void (ParenticLayerCallback::*MethodPtr)(class osgEarth::Annotation::AnnotationNode* node, ParenticAnnotationLayer *layer);
};
class ParenticAnnotationLayer: public QObject, public osgEarth::Annotation::AnnotationLayer
{
public:
    ParenticAnnotationLayer(int id = -1, QObject *parent = nullptr);

    CompositeAnnotationLayer *getParentAtIndex(unsigned index);
    unsigned getIndexOfparent(const ParenticAnnotationLayer* layer) const;
    unsigned getNumParents() const;
    void addChild(osgEarth::Annotation::AnnotationNode* node);
    void removeChild(osgEarth::Annotation::AnnotationNode* node);
    void clear();
    bool hasNode() const;
    int getNumberOfNodes() const;
    void fireCallback(ParenticLayerCallback::MethodPtr, osgEarth::Annotation::AnnotationNode *node);
    virtual CompositeAnnotationLayer* asCompositeAnnotationLayer() { return nullptr; }

    void setOrder(int newOrder);

    int userId() const;

protected:
    virtual void addParent(CompositeAnnotationLayer* parent);
    virtual void removeParent(CompositeAnnotationLayer* parent);
    void insertParent(CompositeAnnotationLayer *layer, unsigned index);

private:
    std::vector<osg::ref_ptr<CompositeAnnotationLayer>> mParents;
//    osg::ref_ptr<osgEarth::Annotation::AnnotationNode> mNode;
    friend class CompositeAnnotationLayer;
    int mUserId;
    int mOrder{-1};
};

class CompositeLayerCallback : public ParenticLayerCallback
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
    CompositeAnnotationLayer(int id = -1, QObject *parent = nullptr);

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
    bool containsLayer( const ParenticAnnotationLayer* layer ) const;
    ParenticAnnotationLayer *getLayerByName(const QString& layerName);
    ParenticAnnotationLayer *getHierarchicalLayerByUserId(int userId);
    void fireCallback(CompositeLayerCallback::MethodPtr, ParenticAnnotationLayer *layer);
//    int getNumChildren() const;
    inline ParenticAnnotationLayer* getChild(int i ) { return mChildildren[i].get(); }
    //! Adds a property notification callback to this layer
    void addCallback(osgEarth::LayerCallback* cb);

    //! Removes a property notification callback from this layer
    void removeCallback(osgEarth::LayerCallback* cb);
    virtual CompositeAnnotationLayer* asCompositeAnnotationLayer() override { return this; }

private:
    osg::ref_ptr<osg::Group> mRoot;
    std::vector<osg::ref_ptr<ParenticAnnotationLayer>> mChildildren;
};



#endif // COMPOSITELAYER_H
