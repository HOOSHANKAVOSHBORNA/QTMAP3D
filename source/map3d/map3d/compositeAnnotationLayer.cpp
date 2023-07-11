#include "compositeAnnotationLayer.h"

CompositeAnnotationLayer::CompositeAnnotationLayer(QObject *parent):
    ParenticAnnotationLayer(parent)
{
    //addCallback(new CompositeLayerCallback());
    init();
}

osg::Node *CompositeAnnotationLayer::getNode() const
{
    return mRoot.get();
}

void CompositeAnnotationLayer::init()
{
    osgEarth::Annotation::AnnotationLayer::init();

    mRoot = new osg::Group();
}

void CompositeAnnotationLayer::setVisible(bool value)
{
    osgEarth::Annotation::AnnotationLayer::setVisible(value);
    for(auto& layer:mChilds)
        layer->setVisible(value);
}

void CompositeAnnotationLayer::setOpacity(float value)
{
    osgEarth::Annotation::AnnotationLayer::setOpacity(value);
    for (auto& layer:mChilds)
        layer->setOpacity(value);
}

void CompositeAnnotationLayer::addLayer(osgEarth::Annotation::AnnotationLayer *layer)
{
    mChilds.append(layer);
    mRoot->addChild(layer->getNode());
    fireCallback(&CompositeLayerCallback::onLayerAdded, layer);
}

void CompositeAnnotationLayer::removeLayer(osgEarth::Annotation::AnnotationLayer *layer)
{
    mChilds.removeIf([&](osgEarth::Annotation::AnnotationLayer* l){
        return l == layer;
    });
    mRoot->removeChild(layer->getNode());
    fireCallback(&CompositeLayerCallback::onLayerRemoved, layer);
}

//void CompositeAnnotationLayer::addParent(CompositeAnnotationLayer *layer)
//{

//}

void CompositeAnnotationLayer::fireCallback(CompositeLayerCallback::MethodPtr method, osgEarth::Annotation::AnnotationLayer *layer)
{
    for (CallbackVector::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
    {
        CompositeLayerCallback* cb = dynamic_cast<CompositeLayerCallback*>(i->get());
        if (cb) (cb->*method)(layer, this);
    }
}

int CompositeAnnotationLayer::getNumChildren() const
{
    return mChilds.length();
}
