#ifndef PARENTICANNOTATIONLAYER_H
#define PARENTICANNOTATIONLAYER_H

#include <QObject>

#include <osgEarthAnnotation/AnnotationLayer>
class CompositeAnnotationLayer;

class ParenticAnnotationLayer: public QObject, public osgEarth::Annotation::AnnotationLayer
{
public:
    ParenticAnnotationLayer(QObject *parent = nullptr);

protected:
    virtual void setParent(CompositeAnnotationLayer* parent);
    friend class CompositeAnnotationLayer;
private:
    std::vector<CompositeAnnotationLayer*> mParents;
};

#endif // PARENTICANNOTATIONLAYER_H
