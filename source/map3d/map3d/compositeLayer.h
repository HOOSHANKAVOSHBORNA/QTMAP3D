#ifndef COMPOSITELAYER_H
#define COMPOSITELAYER_H

#include <osgEarthAnnotation/AnnotationLayer>

#include <QObject>



class CompositeLayer: public QObject, osgEarth::Annotation::AnnotationLayer
{
    Q_OBJECT
public:
    CompositeLayer(QObject *parent = nullptr);

    virtual osg::Node* getNode() const override;
    virtual void init() override;

private:
    osg::ref_ptr<osg::Group> _root;
};

#endif // COMPOSITELAYER_H
