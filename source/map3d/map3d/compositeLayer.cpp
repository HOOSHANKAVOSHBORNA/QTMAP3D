#include "compositeLayer.h"

CompositeAnnotationLayer::CompositeAnnotationLayer(QObject *parent):
    osgEarth::Annotation::AnnotationLayer(),
    QObject(parent)
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
