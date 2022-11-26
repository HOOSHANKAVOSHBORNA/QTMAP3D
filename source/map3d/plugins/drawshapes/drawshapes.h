#ifndef GENERICPLUGIN_H
#define GENERICPLUGIN_H

#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthSymbology/GeometryFactory>
#include "plugininterface.h"

class DrawShapes : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawshapes.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit DrawShapes(QWidget *parent = nullptr);
    ~DrawShapes()override{}
    void setUpUI() override;


private:
    void drawLine();
    void drawSphere();
    void drawPolygone();
    void drawExtrPoly();
    void drawImageOverlay();
    void drawCircle();
    void drawEllipse();
    void drawRect();
    osgEarth::Symbology::Geometry* linePath = new osgEarth::Symbology::LineString();
    osgEarth::Annotation::FeatureNode* pathNode;

private slots:
    void onMouseClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onMouseMove(QMouseEvent* event, osgEarth::GeoPoint geoPos);
};

#endif // GENERICPLUGIN_H
