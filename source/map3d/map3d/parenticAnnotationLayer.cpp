#include "parenticAnnotationLayer.h"

ParenticAnnotationLayer::ParenticAnnotationLayer(QObject *parent):
    osgEarth::Annotation::AnnotationLayer(),
    QObject(parent)
{
    init();
}

void ParenticAnnotationLayer::setParent(CompositeAnnotationLayer *parent)
{
    mParents.push_back(parent);
}
