#include "parenticAnnotationLayer.h"

ParenticAnnotationLayer::ParenticAnnotationLayer(QObject *parent):
    osgEarth::Annotation::AnnotationLayer(),
    QObject(parent)
{

}

osg::Node *ParenticAnnotationLayer::getNode() const
{
    return mRoot.get();
}

void ParenticAnnotationLayer::init()
{
    osgEarth::Annotation::AnnotationLayer::init();

    mRoot = new osg::Group();
}

void ParenticAnnotationLayer::setParent(CompositeAnnotationLayer *parent)
{
    mParents.push_back(parent);
}
