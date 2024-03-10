#include "compositeAnnotationLayer.h"
#include "qdebug.h"
#include "serviceManager.h"
#include "filterManager.h"

ParenticAnnotationLayer::ParenticAnnotationLayer(int id, QObject *parent):
    osgEarth::Annotation::AnnotationLayer(),
    QObject(parent),
    mUserId(id)
{
    init();
    mFilterManager = nullptr;
}

ParenticAnnotationLayer::~ParenticAnnotationLayer()
{
    qDebug() << "~Parentic";
    clear();
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

int ParenticAnnotationLayer::refreshTime() const
{
    return mRefreshTime;
}

void ParenticAnnotationLayer::setRefreshTime(int newRefreshTime)
{
    mRefreshTime = newRefreshTime;
}

void ParenticAnnotationLayer::setFilterManager(FilterManager *newFilterManager)
{
    mFilterManager = newFilterManager;
}

int ParenticAnnotationLayer::userId() const
{
    return mUserId;
}

void ParenticAnnotationLayer::setOrder(int newOrder)
{
    mOrder = newOrder;
}

void ParenticAnnotationLayer::filter()
{
    if (!mFilterManager)
        return;
    for (int i{0}; i < getGroup()->getNumChildren(); ++i){
        auto node = getGroup()->getChild(i);
        auto nodeData = dynamic_cast<NodeData*>(node->getUserData());
        if (nodeData) {
            bool t = mFilterManager->checkNodeToShow(nodeData);
            node->setNodeMask(t);
        }
    }
}

void ParenticAnnotationLayer::clearFilter()
{
    for (int i{0}; i < getGroup()->getNumChildren(); ++i){
        getGroup()->getChild(i)->setNodeMask(true);
    }
}

CompositeAnnotationLayer *ParenticAnnotationLayer::getParentAtIndex(unsigned int index)
{
    if (index >= getNumParents())
        return nullptr;
    return mParents[index].get();
}

CompositeAnnotationLayer::CompositeAnnotationLayer(int id, QObject *parent):
    ParenticAnnotationLayer(id, parent)
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
    for(auto& layer:mChildildren)
        layer->setVisible(value);
}

void CompositeAnnotationLayer::setOpacity(float value)
{
    ParenticAnnotationLayer::setOpacity(value);
    for (auto& layer:mChildildren)
        layer->setOpacity(value);
}

unsigned int CompositeAnnotationLayer::getIndexOfLayer(const ParenticAnnotationLayer *layer) const
{
    unsigned index = 0;
    for (; index < mChildildren.size(); ++index)
    {
        if (mChildildren[index] == layer)
            break;
    }
    return index;
}

unsigned int CompositeAnnotationLayer::getNumLayers() const
{
    return mChildildren.size();
}

void CompositeAnnotationLayer::addLayer(ParenticAnnotationLayer *layer)
{
    layer->setFilterManager(mFilterManager);
    mChildildren.push_back(layer);
    layer->addParent(this);
    mRoot->addChild(layer->getNode());
    for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it){
        layer->addCallback(it->get());
    }

    // if (layer->asCompositeAnnotationLayer()) {
    //     for (auto it : layer->asCompositeAnnotationLayer()->mChildildren)
    //         fireCallback(&CompositeLayerCallback::onLayerAdded, it, it->asCompositeAnnotationLayer()->mChildildren.size() -1);
    // }
    // else {
    //     for (auto it : layer->children()) {
    //         osg::ref_ptr<osgEarth::Annotation::AnnotationNode> node = dynamic_cast<osgEarth::Annotation::AnnotationNode*>(it);
    //         layer->fireCallback(&ParenticLayerCallback::onNodeAdded, node);
    //     }
    // }

    fireCallback(&CompositeLayerCallback::onLayerAdded, layer, mChildildren.size() - 1);
}

void CompositeAnnotationLayer::insertLayer(ParenticAnnotationLayer *layer, unsigned int index)
{
    if (getIndexOfLayer(layer) != getNumLayers())
        return;
    if (index == mChildildren.size()) {
        addLayer(layer);
        return;
    }
    else
        mChildildren.insert(mChildildren.begin() + index, layer);

    layer->addParent(this);
    mRoot->addChild(layer->getNode());
    for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it){
        layer->addCallback(it->get());
    }
    std::sort(mChildildren.begin(), mChildildren.end(), [this](const ParenticAnnotationLayer* p1, const ParenticAnnotationLayer* p2){
        return p1->mOrder < p2->mOrder;
    });

    fireCallback(&CompositeLayerCallback::onLayerAdded, layer, index);
}

void CompositeAnnotationLayer::removeLayer(ParenticAnnotationLayer *layer)
{
    unsigned int index = getIndexOfLayer(layer);
    layer->removeParent(this);
    for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it){
        layer->removeCallback(it->get());
    }
    mRoot->removeChild(layer->getNode());

    auto it = std::remove_if(mChildildren.begin(), mChildildren.end(), [&](ParenticAnnotationLayer* l){
        return l == layer;
    });
    if (it != mChildildren.end()) mChildildren.erase(it);

    fireCallback(&CompositeLayerCallback::onLayerRemoved, layer, index);
}

void CompositeAnnotationLayer::clearLayers()
{
    for(auto& layer: mChildildren){
        layer->removeParent(this);
        for (auto it = _callbacks.begin(); it != _callbacks.end(); ++it){
            layer->removeCallback(it->get());
        }
    }
    mChildildren.clear();
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
//    auto i_oldIndex = mChildildren.end();
//    for(auto i = mChildildren.begin(); i != mChildildren.end(); i++, actualIndex++ )
//    {
//        if ( i->get() == layer )
//        {
//            i_oldIndex = i;
//            oldIndex = actualIndex;
//            break;
//        }
//    }
//    if ( i_oldIndex == mChildildren.end() )
//        return; // layer not found in list
    mChildildren.erase( mChildildren.begin() + oldIndex );
    mChildildren.insert(mChildildren.begin() + index, layerToMove);

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
    for (auto itr = mChildildren.begin();
         itr!=mChildildren.end();
         ++itr)
    {
        if (itr->get()== layer) return true;
    }
    return false;
}

ParenticAnnotationLayer *CompositeAnnotationLayer::getLayerByName(const QString &layerName)
{
    auto it = std::find_if(mChildildren.begin(), mChildildren.end(), [&](const osgEarth::Annotation::AnnotationLayer *layer){
        return layer->getName() == layerName.toStdString();
    });
    if (it == mChildildren.end())
        return nullptr;
    return *it;
}

ParenticAnnotationLayer *CompositeAnnotationLayer::getHierarchicalLayerByUserId(int userId)
{
    if (mUserId == userId)
        return this;
    for (auto &layer: mChildildren){
        if(layer->userId() == userId)
            return layer;
        auto l = layer->asCompositeAnnotationLayer();
        if (l) {
            auto it = l->getHierarchicalLayerByUserId(userId);
            if (it)
                return it;
        }
    }
    return nullptr;
}

void CompositeAnnotationLayer::fireCallback(CompositeLayerCallback::MethodPtr method, ParenticAnnotationLayer *layer, unsigned int index)
{
    for (CallbackVector::iterator i = _callbacks.begin(); i != _callbacks.end(); ++i)
    {
        CompositeLayerCallback* cb = dynamic_cast<CompositeLayerCallback*>(i->get());
        if (cb) (cb->*method)(layer, this, index);
    }
}

void CompositeAnnotationLayer::addCallback(osgEarth::LayerCallback *cb)
{
    ParenticAnnotationLayer::addCallback(cb);
    for (auto& layer: mChildildren) {
        layer->addCallback(cb);
    }
}

void CompositeAnnotationLayer::removeCallback(osgEarth::LayerCallback *cb)
{
    ParenticAnnotationLayer::removeCallback(cb);
    for (auto& layer: mChildildren) {
        layer->removeCallback(cb);
    }
}

std::vector<osg::ref_ptr<ParenticAnnotationLayer> > CompositeAnnotationLayer::childildren() const
{
    return mChildildren;
}

bool CompositeAnnotationLayer::hasChild() const
{
    return getNumLayers() > 0;
}

//int CompositeAnnotationLayer::getNumChildren() const
//{
//    return mChildildren.size();
//}
