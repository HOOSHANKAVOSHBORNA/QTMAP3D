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
    bool cancelBtn;
    void drawLine();
    osgEarth::Annotation::ModelNode* mDrawSphere(float radius);
    void drawPolygone();
    void drawExtrPoly();
    void drawImageOverlay();
    void drawCircle();
    void drawEllipse();
    void drawRect();
    osgEarth::Symbology::Geometry* linePath = new osgEarth::Symbology::LineString();
    osgEarth::Annotation::FeatureNode* pathNode;

private slots:
    void onLineBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onSphereBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onPolygoneBtnClick(QMouseEvent* event);
    void onExtrPolyBtnClick(QMouseEvent* event);
    void onImgOvlyBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onCircleBtnClick(QMouseEvent* event);
    void onRectBtnClick(QMouseEvent* event);

    void onMouseMove(QMouseEvent* event, osgEarth::GeoPoint geoPos);

    osgEarth::Annotation::ModelNode* mSphere(float radius);
};

#endif // GENERICPLUGIN_H
