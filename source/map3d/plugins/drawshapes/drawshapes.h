#ifndef GENERICPLUGIN_H
#define GENERICPLUGIN_H

#include <QMouseEvent>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthSymbology/GeometryFactory>
#include "plugininterface.h"
#include "line.h"
#include "sphere.h"
#include "rect.h"
#include "polygone.h"
#include "circle.h"
#include "ellipse.h"
#include "cone.h"
#include "cylinder.h"
#include "capsule.h"
#include "box.h"

class DrawShapes : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawshapes.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit DrawShapes(QWidget *parent = nullptr);
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    virtual bool setup(MapController *mapController,
                       NetworkManager *networkManager,
                       UIHandle *UIHandle) override;
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
private:
    bool mIsFinished{true};
    enum class Shape {noOne ,line, sphere, cone, cylinder, capsule, box, polygon, imgOvly,
               circle, rect, ellipse};
    Shape shape;
//    osgEarth::Symbology::Geometry* mLinePath = new osgEarth::Symbology::LineString();


private slots:
    void onLineBtnClick(QMouseEvent* event);
    void onSphereBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onConeBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onCylinderBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onCapsuleBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onBoxBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onPolygoneBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onImgOvlyBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onCircleBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onRectBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onEllipseBtnClick(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void onMouseMove(QMouseEvent* event);


private:
    Line* mLine{nullptr};
    Sphere* mSphere;
    Cone* mCone;
    Cylinder* mCylinder;
    Box* mBox;
    Rect* mRect;
    Polygone* mPoly;
    Circle* mCircle;
    Ellipse* mEllipse;
    Capsule* mCapsule;
    MapController* mMapController{nullptr};
    osgEarth::Annotation::FeatureNode* mPathNode;
    float mRadius{20000};
//    osgEarth::Annotation::ModelNode* mSphere(float radius);
//    osgEarth::Annotation::ModelNode* mDrawSphere(float radius);

};

#endif // GENERICPLUGIN_H
