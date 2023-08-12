#ifndef DRAWCapsule_H
#define DRAWCapsule_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>
#include "drawShape.h"

#define CAPSULE "Capsule"

class Capsule;
class CapsuleProperty;
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
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;

private:
    osg::ref_ptr<Capsule> mCapsule{nullptr};
    CapsuleProperty *mCapsuleProperty{nullptr};
//    osg::ref_ptr<CompositeAnnotationLayer> mCompositeCapsuleLayer;
    osg::ref_ptr<ParenticAnnotationLayer> mCapsuleLayer = nullptr;
    static int mCount;
    QQuickItem *mItem;

};

#endif // DRAWCapsule_H
