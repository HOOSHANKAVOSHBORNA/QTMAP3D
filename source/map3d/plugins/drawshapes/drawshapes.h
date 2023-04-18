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
#include "ellipse.h"
#include "cylinder.h"
#include "box.h"
#include "spherenodeeditor.h"
#include "linenode.h"

#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/AnnotationLayer>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/PlaceNode>
#include <QQmlEngine>
#include <QQmlComponent>

#define DRAW_LAYER_NAME "DrawShapes"

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
               RECT, ELLIPSE};
    Shape mShape;
    enum class DrawingState {NONE, START, FINISH, DELETE, INITIAL};
    DrawingState mDrawingState;
    osgEarth::Annotation::ImageOverlay* mImageOverlay{nullptr};
    osgEarth::Annotation::ModelNode* mLineSphereNode{nullptr};
    osgEarth::Annotation::RectangleNodeEditor* mRectEditor{nullptr};
    osgEarth::Annotation::EllipseNodeEditor* mElpsEditor{nullptr};
    osgEarth::Annotation::ImageOverlayEditor* mImgOvlEditor{nullptr};
    osg::Group* mCircleGr;
    osgEarth::Annotation::AnnotationLayer* mAnnoLayer;

    osgEarth::Annotation::SphereDragger* mPolyHdragger;

    bool addNodeToLayer(osg::Node *node);
    void removeNodeFromLayer(osg::Node *node);
//    osgEarth::Symbology::Geometry* mLinePath = new osgEarth::Symbology::LineString();


private slots:
    void onLineBtnClick(QMouseEvent* event);
    void onSphereBtnClick(QMouseEvent* event);
    void onNodeBtnDoubleClick(QMouseEvent* event, osg::Node *nodeEditor = nullptr);
    void onConeBtnClick(QMouseEvent* event);
    void onCylinderBtnClick(QMouseEvent* event);
//    void onCapsuleBtnClick(QMouseEvent* event);
    void onBoxBtnClick(QMouseEvent* event);
    void onPolygoneBtnClick(QMouseEvent* event);
    void onImgOvlyBtnClick(QMouseEvent* event);
    void onRectBtnClick(QMouseEvent* event);
    void onEllipseBtnClick(QMouseEvent* event);
    void onLineMouseMove(QMouseEvent* event);
    void onPolyMouseMove(QMouseEvent* event);


private:
    LineNode* mLine{nullptr};
//    Sphere* mSphere;
    Cylinder* mCylinder;
    Box* mBox;
    Rect* mRect{nullptr};
    Polygone* mPoly;
    Ellipse* mEllipse{nullptr};
    MapController* mMapController{nullptr};
    osgEarth::Annotation::FeatureNode* mPathNode;

//    osg::ref_ptr<SphereNode> mSphereNode;
//    osg::ref_ptr<SphereNodeEditor> mSphereNodeEditor;

    double mRadius{200000};
    QQmlEngine *mQmlEngine = nullptr;
    QQuickItem *mItem = nullptr;
//    osgEarth::Annotation::ModelNode* mSphere(float radius);
//    osgEarth::Annotation::ModelNode* mDrawSphere(float radius);
    bool mEnterShapeZone{false};

};

#endif // GENERICPLUGIN_H
