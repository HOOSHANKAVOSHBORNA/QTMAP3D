#include "compositeLayer.h"

CompositeLayer::CompositeLayer(QObject *parent):
    osgEarth::Annotation::AnnotationLayer(),
    QObject(parent)
{
    init();
}

osg::Node *CompositeLayer::getNode() const
{
    _root.get();
}

void CompositeLayer::init()
{
    osgEarth::Annotation::AnnotationLayer::init();

    _root = new osg::Group();
}
