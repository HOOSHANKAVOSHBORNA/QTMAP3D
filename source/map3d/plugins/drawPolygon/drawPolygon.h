#ifndef DRAWPOLYGON_H
#define DRAWPOLYGON_H

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include "drawShape.h"
#include "polygonProperty.h"
#include "polygon.h"

#define POLYGON "Polygon"

class DrawPolygon : public DrawShape
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawPolygon.json")
    Q_INTERFACES(PluginInterface)

public:
    DrawPolygon(QObject *parent = nullptr);
    bool setup() override;
private slots:
    void onPolygonItemCheck (bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void tempDrawing(const osgEarth::GeoPoint &geoPos) override;
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;

private:
    osg::ref_ptr<Polygon> mPolygon{nullptr};
    PolygonProperty *mPolygonProperty{nullptr};
    osg::ref_ptr<ParenticAnnotationLayer> mPolygonLayer;
    static int mCount;
//    QQuickItem *mItem;


};

#endif // DRAWPOLYGON_H
