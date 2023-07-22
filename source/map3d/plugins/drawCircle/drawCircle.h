#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include "drawShape.h"
#include "circle.h"
#include "circleproperties.h"

#define CIRCLE "Circle"

class DrawCircle : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawCircle.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawCircle(QObject *parent = nullptr);
    bool setup() override;
private slots:
    void onCircleItemCheck (bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;
    void createProperty();

private:
    osg::ref_ptr<Circle> mCircle{nullptr};
    CircleProperties *mCircleProperties{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mCircleLayer;
    static int mCount;
};

#endif // DRAWCIRCLE_H
