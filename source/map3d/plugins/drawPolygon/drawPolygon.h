#ifndef DRAWPOLYGON_H
#define DRAWPOLYGON_H

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include "drawShape.h"
#include "polygonproperties.h"
#include "polygon.h"

#define POLYGON "Polygon"
#define MEASUREAREA "Area"

class DrawPolygon : public DrawShape
{
public:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawPolygon.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit DrawPolygon(QObject *parent = nullptr);
    bool setup() override;
private slots:
    void onPolygonItemCheck(bool check);
    void onMeasureAreaItemCheck(bool check);
private:
    void initDraw(const osgEarth::GeoPoint &geoPos) override;
    void tempDrawing(const osgEarth::GeoPoint &geoPos) override;
    void drawing(const osgEarth::GeoPoint &geoPos) override;
    void cancelDraw()override;
private:
    enum class Type{NONE, POLYGONN, AREA};
    Type mType;
    osg::ref_ptr<Polygon> mPolygon{nullptr};
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mPolygonLayer;
    osg::ref_ptr<osgEarth::Annotation::AnnotationLayer> mAreaLayer;
    static int mCount;
//    PolygonProperties* mPolygonProperties{nullptr};
};

#endif // DRAWPOLYGON_H
