#include "compositeAnnotationLayer.h"

ParenticAnnotationLayer::ParenticAnnotationLayer(QObject *parent):
    osgEarth::Annotation::AnnotationLayer(),
    QObject(parent)
{
    init();
}

void ParenticAnnotationLayer::addParent(CompositeAnnotationLayer *parent)
{
    osg::ref_ptr<CompositeAnnotationLayer> layer( parent );
    mParents.push_back(layer);
}

void ParenticAnnotationLayer::removeParent(CompositeAnnotationLayer *parent)
{
    auto it = std::find_if(mParents.begin(), mParents.end(), [&](const CompositeAnnotationLayer* p){
        return p == parent;
    });
    if (it != mParents.end()) mParents.erase(it);
}

CompositeAnnotationLayer::CompositeAnnotationLayer(QObject *parent):
    ParenticAnnotationLayer(parent)
{
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

unsigned int CompositeAnnotationLayer::getIndexOfLayer(const ParenticAnnotationLayer *layer) const
{
    unsigned index = 0;
    for (; index < mChilds.size(); ++index)
    {
        if (mChilds[index] == layer)
            break;
    }
    return index;
}

unsigned int CompositeAnnotationLayer::getNumLayers() const
{
    return mChilds.size();
}

void CompositeAnnotationLayer::addLayer(ParenticAnnotationLayer *layer)
{
    mChilds.push_back(layer);
    layer->addParent(this);
    mRoot->addChild(layer->getNode());
    fireCallback(&CompositeLayerCallback::onLayerAdded, layer);
}

void CompositeAnnotationLayer::insertLayer(ParenticAnnotationLayer *layer, unsigned int index)
{
    if (getIndexOfLayer(layer) != getNumLayers())
        return;
    if (index == mChilds.size())
        mChilds.push_back(layer);
    else
        mChilds.insert(mChilds.begin() + index, layer);
}

void CompositeAnnotationLayer::removeLayer(ParenticAnnotationLayer *layer)
{
    auto it = std::remove_if(mChilds.begin(), mChilds.end(), [&](ParenticAnnotationLayer* l){
        return l == layer;
    });
    if (it != mChilds.end()) mChilds.erase(it);
    mRoot->removeChild(layer->getNode());
    fireCallback(&CompositeLayerCallback::onLayerRemoved, layer);
}

void CompositeAnnotationLayer::moveLayer(ParenticAnnotationLayer *layer, unsigned int index)
{
    unsigned int oldIndex = 0;
    unsigned int actualIndex = 0;
    osg::ref_ptr<ParenticAnnotationLayer> layerToMove( layer );
    auto i_oldIndex = mChilds.end();
    for(auto i = mChilds.begin(); i != mChilds.end(); i++, actualIndex++ )
    {
        if ( i->get() == layer )
        {
            i_oldIndex = i;
            oldIndex = actualIndex;
            break;
        }
    }
    if ( i_oldIndex == mChilds.end() )
        return; // layer not found in list
    mChilds.erase( i_oldIndex );
    mChilds.insert(mChilds.begin() + index, layerToMove);
    for (CallbackVector::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i){
        CompositeLayerCallback* cb = dynamic_cast<CompositeLayerCallback*>(i->get());
        if (cb) cb->onLayerMoved(layer, this, oldIndex, index);
    }
}

void CompositeAnnotationLayer::removeLayerByName(const QString &layerName)
{
    auto layer = getLayerByName(layerName);
    if (!layer)
        return;
    removeLayer(layer);
}

ParenticAnnotationLayer *CompositeAnnotationLayer::getLayerByName(const QString &layerName)
{
    auto it = std::find_if(mChilds.begin(), mChilds.end(), [&](const osgEarth::Annotation::AnnotationLayer *layer){
        return layer->getName() == layerName.toStdString();
    });
    if (it == mChilds.end())
        return nullptr;
    return *it;
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
