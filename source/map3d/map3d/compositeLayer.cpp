#include "compositeLayer.h"

CompositeAnnotationLayer::CompositeAnnotationLayer(QObject *parent):
    osgEarth::Annotation::AnnotationLayer(),
    QObject(parent)
{
    addCallback(new CompositeCallback());
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
}

void CompositeAnnotationLayer::removeLayer(osgEarth::Annotation::AnnotationLayer *layer)
{
    mChilds.removeIf([&](osgEarth::Annotation::AnnotationLayer* l){
        return l == layer;
    });
    _root->removeChild(layer->getNode());
}

//void CompositeAnnotationLayer::addParent(CompositeAnnotationLayer *layer)
//{

//}

void CompositeAnnotationLayer::fireCallback(CompositeCallback::MethodPtr method)
{
    for (CallbackVector::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
    {
        CompositeCallback* cb = dynamic_cast<CompositeCallback*>(i->get());
        if (cb) (cb->*method)(this);
    }
}

CompositeCallback::CompositeCallback()
{

}

void CompositeCallback::onVisibleChanged(osgEarth::VisibleLayer *layer)
{

}

void CompositeCallback::onOpacityChanged(osgEarth::VisibleLayer *layer)
{

}

void CompositeCallback::onVisibleRangeChanged(osgEarth::VisibleLayer *layer)
{

}
