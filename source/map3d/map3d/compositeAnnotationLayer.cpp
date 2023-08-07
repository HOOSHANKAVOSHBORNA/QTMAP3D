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

unsigned int ParenticAnnotationLayer::getIndexOfparent(const ParenticAnnotationLayer *layer) const
{
    unsigned index = 0;
    for (; index < mParents.size(); ++index)
    {
        if (mParents[index] == layer)
            break;
    }
    return index;
}

unsigned int ParenticAnnotationLayer::getNumParents() const
{
    return mParents.size();
}

void ParenticAnnotationLayer::addChild(osgEarth::Annotation::AnnotationNode *node)
{
    osgEarth::Annotation::AnnotationLayer::addChild(node);
    fireCallback(&ParenticLayerCallback::onNodeAdded, node);
}

void ParenticAnnotationLayer::removeChild(osgEarth::Annotation::AnnotationNode *node)
{
    getGroup()->removeChild(node);
    fireCallback(&ParenticLayerCallback::onNodeRemoved, node);
}

void ParenticAnnotationLayer::clear()
{
    unsigned int numChild = getGroup()->getNumChildren();
    unsigned int i = 0;
    while(i < numChild){
        auto node = static_cast<osgEarth::Annotation::AnnotationNode*>(getGroup()->getChild(0));
        removeChild(node);
        i++;
    }
}

bool ParenticAnnotationLayer::hasNode() const
{
    return getGroup()->getNumChildren() > 0;
}

int ParenticAnnotationLayer::getNumberOfNodes() const
{
    return getGroup()->getNumChildren();
}

void ParenticAnnotationLayer::fireCallback(ParenticLayerCallback::MethodPtr method, osgEarth::Annotation::AnnotationNode *node)
{
    for (CallbackVector::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
    {
        ParenticLayerCallback* cb = dynamic_cast<ParenticLayerCallback*>(i->get());
        if (cb) (cb->*method)(node, this);
    }
}

void ParenticAnnotationLayer::insertParent(CompositeAnnotationLayer *parent, unsigned int index)
{
    if (getIndexOfparent(parent) != getNumParents())
        return;
    if (index == mParents.size())
        mParents.push_back(parent);
    else
        mParents.insert(mParents.begin() + index, parent);
}

CompositeAnnotationLayer *ParenticAnnotationLayer::getParentAtIndex(unsigned int index)
{
    if (index >= getNumParents())
        return nullptr;
    return mParents[index].get();
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
    for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it){
        layer->addCallback(it->get());
    }

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

    layer->addParent(this);
    for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it){
        layer->addCallback(it->get());
    }

    fireCallback(&CompositeLayerCallback::onLayerAdded, layer);
}

void CompositeAnnotationLayer::removeLayer(ParenticAnnotationLayer *layer)
{
    layer->removeParent(this);
    for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it){
        layer->removeCallback(it->get());
    }
    mRoot->removeChild(layer->getNode());

    auto it = std::remove_if(mChilds.begin(), mChilds.end(), [&](ParenticAnnotationLayer* l){
        return l == layer;
    });
    if (it != mChilds.end()) mChilds.erase(it);

    fireCallback(&CompositeLayerCallback::onLayerRemoved, layer);
}

void CompositeAnnotationLayer::clearLayers()
{
    for(auto& layer: mChilds){
        layer->removeParent(this);
        for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it){
            layer->removeCallback(it->get());
        }
    }
    mChilds.clear();
    mRoot->removeChildren(0, mRoot->getNumChildren());
}

void CompositeAnnotationLayer::moveLayer(ParenticAnnotationLayer *layer, unsigned int index)
{
    unsigned int oldIndex = 0;
    unsigned int actualIndex = 0;
    osg::ref_ptr<ParenticAnnotationLayer> layerToMove( layer );
    oldIndex = getIndexOfLayer(layer);
    if (oldIndex == getNumLayers())
        return; // layer not found in list
//    auto i_oldIndex = mChilds.end();
//    for(auto i = mChilds.begin(); i != mChilds.end(); i++, actualIndex++ )
//    {
//        if ( i->get() == layer )
//        {
//            i_oldIndex = i;
//            oldIndex = actualIndex;
//            break;
//        }
//    }
//    if ( i_oldIndex == mChilds.end() )
//        return; // layer not found in list
    mChilds.erase( mChilds.begin() + oldIndex );
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

bool CompositeAnnotationLayer::containsLayer(const ParenticAnnotationLayer *layer) const
{
    for (auto itr = mChilds.begin();
         itr!=mChilds.end();
         ++itr)
    {
        if (itr->get()== layer) return true;
    }
    return false;
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

void CompositeAnnotationLayer::addCallback(osgEarth::LayerCallback *cb)
{
    ParenticAnnotationLayer::addCallback(cb);
    for (auto& layer: mChilds) {
        layer->addCallback(cb);
    }
}

void CompositeAnnotationLayer::removeCallback(osgEarth::LayerCallback *cb)
{
    ParenticAnnotationLayer::removeCallback(cb);
    for (auto& layer: mChilds) {
        layer->removeCallback(cb);
    }
}

//int CompositeAnnotationLayer::getNumChildren() const
//{
//    return mChilds.size();
//}
