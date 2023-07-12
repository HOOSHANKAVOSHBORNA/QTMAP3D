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
    ParenticAnnotationLayer::init();

    mRoot = new osg::Group();
}

void CompositeAnnotationLayer::setVisible(bool value)
{
    ParenticAnnotationLayer::setVisible(value);
    for(auto& layer:mChilds)
        layer->setVisible(value);
}

void CompositeAnnotationLayer::setOpacity(float value)
{
    ParenticAnnotationLayer::setOpacity(value);
    for (auto& layer:mChilds)
        layer->setOpacity(value);
}

void CompositeAnnotationLayer::addLayer(ParenticAnnotationLayer *layer)
{
    mChilds.push_back(layer);
    layer->setParent(this);
    mRoot->addChild(layer->getNode());
    fireCallback(&CompositeLayerCallback::onLayerAdded, layer);
}

void CompositeAnnotationLayer::removeLayer(ParenticAnnotationLayer *layer)
{
    auto it = std::remove_if(mChilds.begin(), mChilds.end(), [&](ParenticAnnotationLayer* l){
        return l == layer;
    });
    mRoot->removeChild(layer->getNode());
    fireCallback(&CompositeLayerCallback::onLayerRemoved, layer);
}

void CompositeAnnotationLayer::fireCallback(CompositeLayerCallback::MethodPtr method, ParenticAnnotationLayer *layer)
{
    for (CallbackVector::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
    {
        CompositeLayerCallback* cb = dynamic_cast<CompositeLayerCallback*>(i->get());
        if (cb) (cb->*method)(layer, this);
    }
}

int CompositeAnnotationLayer::getNumChildren() const
{
    return mChilds.size();
}
