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
    void addUpdatePolygon(PolygonData *polygonData);
private slots:
    void onPolygonItemCheck (bool check);
    void polygonDataReceived(PolygonData *polygonData);
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
    QMap<int, osg::ref_ptr<Polygon>> mPolygonMap;
//    QQuickItem *mItem;


};

#endif // DRAWPOLYGON_H
