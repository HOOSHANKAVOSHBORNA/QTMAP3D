#ifndef DRAWCapsule_H
#define DRAWCapsule_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>
#include "drawShape.h"
#include "capsule.h"
#include "capsuleProperties.h"

#define CAPSULE "Capsule"

class DrawCapsule : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawCapsule.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawCapsule(QObject *parent = nullptr);
    bool setup() override;
private slots:
    void onCapsuleItemCheck (bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;

private:
    osg::ref_ptr<Capsule> mCapsule{nullptr};
    CapsuleProperties *mCapsuleProperties{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mCapsuleLayer;
    static int mCount;
};

#endif // DRAWCapsule_H
