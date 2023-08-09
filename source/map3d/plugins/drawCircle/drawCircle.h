#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H


#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include "drawShape.h"

#define CIRCLE "Circle"
class Circle;
class CircleProperty;

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
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;


private:
    osg::ref_ptr<Circle> mCircle{nullptr};
    CircleProperty *mCircleProperty{nullptr};

    osg::ref_ptr<ParenticAnnotationLayer> mCircleLayer;
    static int mCount;
    QQuickItem *mItem;


};

#endif // DRAWCIRCLE_H
