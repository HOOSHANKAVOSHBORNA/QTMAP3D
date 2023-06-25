#ifndef DRAWCONE_H
#define DRAWCONE_H

#include <osgEarth/GLUtils>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/PlaceNode>

#include "drawShape.h"
#include "cone.h"
#include "coneProperties.h"

#define CONE "Cone"

class DrawCone : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawCone.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawCone(QObject *parent = nullptr);
    bool setup() override;
private slots:
    void onConeItemCheck (bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;

private:
    osg::ref_ptr<Cone> mCone{nullptr};
    ConeProperties *mConeProperties{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mConeLayer;
    static int mCount;
};

#endif // DRAWCONE_H
