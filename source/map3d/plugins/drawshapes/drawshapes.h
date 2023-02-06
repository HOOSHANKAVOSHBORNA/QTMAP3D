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
#include "spherenodeeditor.h"
#include "linenode.h"

#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/PlaceNode>

class DrawShapes : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE  "drawshapes.json")
    Q_INTERFACES(PluginInterface)

public:
    explicit DrawShapes(QWidget *parent = nullptr);
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) override;
    virtual void onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked) override;
    bool setup(MapController *mapController,
               UIHandle *UIHandle) override;
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
private:
    enum class Shape {NONE ,LINE, SPHERE, CONE, CYLINDER, CAPSULE, BOX, POLYGON, IMGOVLY,
               CIRCLE, RECT, ELLIPSE};
    Shape mShape;
    enum class DrawingState {NONE, START, FINISH, DELETE, INITIAL};
    DrawingState mDrawingState;
    osgEarth::Annotation::ImageOverlay* mImageOverlay{nullptr};
    osgEarth::Annotation::ModelNode* mLineSphereNode{nullptr};
    osgEarth::Annotation::CircleNodeEditor* mCircleEditor{nullptr};
    osgEarth::Annotation::RectangleNodeEditor* mRectEditor{nullptr};
    osgEarth::Annotation::EllipseNodeEditor* mElpsEditor{nullptr};
    osgEarth::Annotation::ImageOverlayEditor* mImgOvlEditor{nullptr};
    osg::Group* mCircleGr;
    osgEarth::Annotation::AnnotationLayer* mAnnoLayer;

    osgEarth::Annotation::SphereDragger* mCircleHdragger;
    osgEarth::Annotation::SphereDragger* mPolyHdragger;
//    osgEarth::Symbology::Geometry* mLinePath = new osgEarth::Symbology::LineString();


private slots:
    void onLineBtnClick(QMouseEvent* event);
    void onSphereBtnClick(QMouseEvent* event);
    void onNodeBtnDoubleClick(QMouseEvent* event, osg::Node *nodeEditor = nullptr);
    void onConeBtnClick(QMouseEvent* event);
    void onCylinderBtnClick(QMouseEvent* event);
    void onCapsuleBtnClick(QMouseEvent* event);
    void onBoxBtnClick(QMouseEvent* event);
    void onPolygoneBtnClick(QMouseEvent* event);
    void onImgOvlyBtnClick(QMouseEvent* event);
    void onCircleBtnClick(QMouseEvent* event);
    void onRectBtnClick(QMouseEvent* event);
    void onEllipseBtnClick(QMouseEvent* event);
    void onLineMouseMove(QMouseEvent* event);
    void onCircleMouseMove(QMouseEvent* event);
    void onPolyMouseMove(QMouseEvent* event);


private:
    LineNode* mLine{nullptr};
//    Sphere* mSphere;
    Cone* mCone;
    Cylinder* mCylinder;
    Box* mBox;
    Rect* mRect{nullptr};
    Polygone* mPoly;
    Circle* mCircle{nullptr};
    Ellipse* mEllipse{nullptr};
    Capsule* mCapsule;
    MapController* mMapController{nullptr};
    osgEarth::Annotation::FeatureNode* mPathNode;

    osg::ref_ptr<SphereNode> mSphereNode;
    osg::ref_ptr<SphereNodeEditor> mSphereNodeEditor;

    double mRadius{200000};
//    osgEarth::Annotation::ModelNode* mSphere(float radius);
//    osgEarth::Annotation::ModelNode* mDrawSphere(float radius);

};

#endif // GENERICPLUGIN_H
