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
#include "spherenode.h"
#include "osgEarthAnnotation/AnnotationEditing"

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
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
private:
    bool mIsFinished{true};
    enum class Shape {none ,line, sphere, cone, cylinder, capsule, box, polygon, imgOvly,
               circle, rect, ellipse};
    Shape shape;
    osgEarth::Annotation::CircleNode* circle;
    osgEarth::Annotation::CircleNodeEditor* mCircleEditor{nullptr};
    osgEarth::Annotation::RectangleNodeEditor* mRectEditor{nullptr};
    osg::Group* mCircleGr;
//    osgEarth::Symbology::Geometry* mLinePath = new osgEarth::Symbology::LineString();


private slots:
    void onLineBtnClick(QMouseEvent* event);
    void onSphereBtnClick(QMouseEvent* event);
    void onConeBtnClick(QMouseEvent* event);
    void onCylinderBtnClick(QMouseEvent* event);
    void onCapsuleBtnClick(QMouseEvent* event);
    void onBoxBtnClick(QMouseEvent* event);
    void onPolygoneBtnClick(QMouseEvent* event);
    void onImgOvlyBtnClick(QMouseEvent* event);
    void onCircleBtnClick(QMouseEvent* event);
    void onRectBtnClick(QMouseEvent* event);
    void onEllipseBtnClick(QMouseEvent* event);
    void onMouseMove(QMouseEvent* event);


private:
    Line* mLine{nullptr};
    Sphere* mSphere;
    Cone* mCone;
    Cylinder* mCylinder;
    Box* mBox;
    Rect* mRect;
    Polygone* mPoly;
    Circle* mCircle{nullptr};
    Ellipse* mEllipse;
    Capsule* mCapsule;
    MapController* mMapController{nullptr};
    osgEarth::Annotation::FeatureNode* mPathNode;
    SphereNode* mSphereNode;
    double mRadius{20000};
//    osgEarth::Annotation::ModelNode* mSphere(float radius);
//    osgEarth::Annotation::ModelNode* mDrawSphere(float radius);

};

#endif // GENERICPLUGIN_H
