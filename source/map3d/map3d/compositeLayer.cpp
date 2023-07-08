#include "compositeLayer.h"

CompositeAnnotationLayer::CompositeAnnotationLayer(QObject *parent):
    osgEarth::Annotation::AnnotationLayer(),
    QObject(parent)
{
    addCallback(new CompositeLayerCallback());
}

osg::Node *CompositeAnnotationLayer::getNode() const
{
    return _root.get();
}

void CompositeAnnotationLayer::init()
{
    osgEarth::Annotation::AnnotationLayer::init();

    _root = new osg::Group();
}

void CompositeAnnotationLayer::addLayer(osgEarth::Annotation::AnnotationLayer *layer)
{
    mChilds.append(layer);
    _root->addChild(layer->getNode());
    fireCallback(&CompositeLayerCallback::onLayerAdded, layer);
}

void CompositeAnnotationLayer::removeLayer(osgEarth::Annotation::AnnotationLayer *layer)
{
    mChilds.removeIf([&](osgEarth::Annotation::AnnotationLayer* l){
        return l == layer;
    });
    _root->removeChild(layer->getNode());
    fireCallback(&CompositeLayerCallback::onLayerRemoved, layer);
}

//void CompositeAnnotationLayer::addParent(CompositeAnnotationLayer *layer)
//{

//}

void CompositeAnnotationLayer::fireCallback(CompositeLayerCallback::MethodPtr method, AnnotationLayer *layer)
{
    for (CallbackVector::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
    {
        CompositeLayerCallback* cb = dynamic_cast<CompositeLayerCallback*>(i->get());
        if (cb) (cb->*method)(layer);
    }
}

CompositeLayerCallback::CompositeLayerCallback()
{

}

