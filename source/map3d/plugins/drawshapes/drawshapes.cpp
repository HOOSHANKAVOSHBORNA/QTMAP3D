#include "drawshapes.h"
#include "spherenode.h"

#include <osgEarthSymbology/Style>
#include <osgEarth/ModelLayer>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/feature_wfs/WFSFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <osgEarth/GLUtils>
#include <osgViewer/Viewer>
#include <osgEarth/GLUtils>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/ModelNode>
#include "mapcontroller.h"
#include "osg/Group"
#include "osgEarth/ModelLayer"
#include "osgEarth/Layer"
#include <line.h>
#include "osgEarthAnnotation/AnnotationEditing"
#include <osgEarthAnnotation/AnnotationLayer>

using namespace osgEarth::Annotation;

DrawShapes::DrawShapes(QWidget *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(drawShapes);
}

const QString CATEGORY = "Draw";
const QString LINE = "Line";
const QString SPHERE = "Sphere";
const QString POLYGON = "Polygon";
const QString EXTRPOLY = "ExtrPoly";
const QString IMAGE_OVERLAY = "ImageOverlay";
const QString CIRCLE = "Circle";
const QString ELLIPSE = "Ellipse";
const QString RECT = "Rect";
const QString CONE = "Cone";
const QString CYLINDER = "Cylinder";
const QString CAPSULE = "Capsule";
const QString BOX = "Box";

bool DrawShapes::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    mAnnoLayer = new osgEarth::Annotation::AnnotationLayer;
    Q_UNUSED(engine)
    desc->toolboxItemsList.push_back(new ItemDesc{LINE, CATEGORY, "qrc:/resources/line.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{SPHERE, CATEGORY, "qrc:/resources/sphere.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{CONE, CATEGORY, "qrc:/resources/cone.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{CYLINDER, CATEGORY, "qrc:/resources/cylinder.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{CAPSULE, CATEGORY, "qrc:/resources/capsule.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{BOX, CATEGORY, "qrc:/resources/box.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{POLYGON, CATEGORY, "qrc:/resources/polygon.png", true});
    //desc->toolboxItemsList.push_back(new ItemDesc{EXTRPOLY, CATEGORY, "qrc:/res/extroPolygon.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{IMAGE_OVERLAY, CATEGORY, "qrc:/resources/image.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{CIRCLE, CATEGORY, "qrc:/resources/circle.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{ELLIPSE, CATEGORY, "qrc:/resources/ellipse.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{RECT, CATEGORY, "qrc:/resources/rectangle.png", true});
    return true;
}

void DrawShapes::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{

    if(CATEGORY == category && name == LINE)
    {
        if(checked)
        {
            mLine = new Line(mMapController);
            mShape = Shape::LINE;
        }
        else
        {
            mIsFinished = true;
            mLine->removePoint();
            mShape = Shape::NONE;
        }
    }
    if(CATEGORY == category && name == SPHERE)
    {
        if(checked)
        {
            mSphere = new Sphere(mMapController,mRadius, false);
            mSphereNode = new SphereNode();
            mShape = Shape::SPHERE;
        }
        else
        {
            mShape = Shape::NONE;
        }
    }
    if(CATEGORY == category && name == CONE)
    {
        if(checked)
        {
            mShape = Shape::CONE;
            mCone = new Cone(mMapController,111100, 111100,false);
        }
        else
        {
            mShape = Shape::NONE;

        }
    }
    if(CATEGORY == category && name == CYLINDER)
    {
        if(checked)
        {
            mShape = Shape::CYLINDER;
            mCylinder = new Cylinder(mMapController,mRadius, 111111,false);
        }
        else
        {
            mShape = Shape::NONE;
            mDrawingState = DrawingState::NONE;
        }
    }
    if(CATEGORY == category && name == CAPSULE)
    {
        if(checked)
        {
            mCapsule = new Capsule(mMapController,mRadius, 111111,false);
            mShape = Shape::CAPSULE;
        }
        else
        {
            mShape = Shape::NONE;
            mDrawingState = DrawingState::NONE;
        }
    }
    if(CATEGORY == category && name == BOX)
    {
        if(checked)
        {
            mBox = new Box(mMapController,50000, 60000, 40000, false);
            mShape = Shape::BOX;
        }
        else
        {
            mShape = Shape::NONE;
            mDrawingState = DrawingState::NONE;
        }
    }
    if(CATEGORY == category && name == POLYGON)
    {
        if(checked)
        {
            mShape = Shape::POLYGON;
            mDrawingState = DrawingState::START;
            mCircleGr = new osg::Group;
            mMapController->addNode(mCircleGr);
            mPoly = new Polygone(mMapController, false);
            mPoly->setFillColor(osgEarth::Color::Green);
        }
        else
        {
            mShape = Shape::NONE;
            mDrawingState = DrawingState::NONE;
        }
    }

    if(CATEGORY == category && name == IMAGE_OVERLAY)
    {
        if(checked)
        {
            mShape = Shape::IMGOVLY;
        }
        else
        {
            mShape = Shape::NONE;
            mDrawingState = DrawingState::NONE;
        }
    }
    if(CATEGORY == category && name == CIRCLE)
    {
        if(checked)
        {

            mCircleHdragger = new osgEarth::Annotation::SphereDragger(mMapController->getMapNode());
            mShape = Shape::CIRCLE;

        }
        else
        {
            mShape = Shape::NONE;
            mDrawingState = DrawingState::NONE;
        }

    }
    if(CATEGORY == category && name == ELLIPSE)
    {
        if(checked)
        {
            mShape = Shape::ELLIPSE;

        }
        else
        {
            mShape = Shape::NONE;
            mDrawingState = DrawingState::NONE;
        }
    }
    if(CATEGORY == category && name == RECT)
    {
        if(checked)
        {
            mShape = Shape::RECT;


        }
        else
        {
            mShape = Shape::NONE;
            mDrawingState = DrawingState::NONE;
        }
    }
}

bool DrawShapes::setup(MapController *mapController,
                       NetworkManager *networkManager,
                       UIHandle *UIHandle)
{
    mMapController = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());
    return true;
}

void DrawShapes::mousePressEvent(QMouseEvent *event)
{
    switch (mShape) {
    case Shape::NONE:
        break;
    case Shape::LINE:
        onLineBtnClick(event);
        break;
    case Shape::SPHERE:
        onSphereBtnClick(event);
        break;
    case Shape::CONE:
        onConeBtnClick(event);
        break;
    case Shape::CYLINDER:
        onCylinderBtnClick(event);
        break;
    case Shape::CAPSULE:
        onCapsuleBtnClick(event);
        break;
    case Shape::BOX:
        onBoxBtnClick(event);
        break;
    case Shape::POLYGON:
        onPolygoneBtnClick(event);
        break;
    case Shape::IMGOVLY:
        onImgOvlyBtnClick(event);
        break;
    case Shape::CIRCLE:
        onCircleBtnClick(event);
        break;
    case Shape::RECT:
        onRectBtnClick(event);
        break;
    case Shape::ELLIPSE:
        onEllipseBtnClick(event);
        break;
    }

}


void DrawShapes::mouseMoveEvent(QMouseEvent *event)
{
    if(mShape == Shape::LINE){
        onLineMouseMove(event);
    }
    else if (mShape == Shape::CIRCLE) {
        onCircleMouseMove(event);
    }
    else if (mShape == Shape::POLYGON) {
        onPolyMouseMove(event);
    }
}

void DrawShapes::mouseDoubleClickEvent(QMouseEvent */*event*/)
{
}

void DrawShapes::onLineBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::LeftButton && event->type() ==  QEvent::Type::MouseButtonPress)
    {
        mIsFinished = false;

        mLine->addPoint(geoPos.vec3d());
        //mLine->setLineStyle(osgEarth::Color::Purple,7,false);

        mLine->setLineColor(osgEarth::Color::Purple);
        mLine->setLineWidth(7);
        mLine->setLineClamp(false);
        mMapController->addNode(mLine->getNode());
    }
    if(event->button() == Qt::MouseButton::RightButton && event->type() ==  QEvent::Type::MouseButtonPress)
    {
        //mLine->setPointStyle(osgEarth::Color::Purple,20,true);
        mLine->setPointColor(osgEarth::Color::Yellow);
        mLine->setPointSize(20);
        mLine->setPointClamp(true);
        mLine->setLineHeight(250000);
        mLine->setLineColor(osgEarth::Color::White);

        mLine->removePoint();
        mIsFinished = true;
        mLine = new Line(mMapController);
    }

}

void DrawShapes::onLineMouseMove(QMouseEvent *event)
{
    if (event->type() ==  QEvent::Type::MouseMove)
    {
        if (!mIsFinished)
        {
            if (mLine->getSize() >= 2)
            {
                mLine->removePoint();
            }
            osg::Vec3d worldPos;
            mMapController->screenToWorld(event->x(), event->y(), worldPos);
            osgEarth::GeoPoint geoPos;
            geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

            mLine->addPoint(geoPos.vec3d());
        }
    }
}

void DrawShapes::onCircleMouseMove(QMouseEvent *event)
{
//    osg::Vec3d worldPos;
//    mMapController->screenToWorld(event->x(), event->y(), worldPos);
//    osgEarth::GeoPoint geoPos;
//    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);
      if (mCircle){
      mCircle->setCircleHeight(mCircleHdragger->Dragger::getPosition().z());
}
//    //x->setSize(30000);
//    x->setColor(osg::Vec4f(0,255,0,1));
//    x->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y()));
//    mMapController->addNode(x);
//    x->addChild(mCircle);
//    x->setDynamic(true);
//    x->setHorizonCulling(true);
//    x->Dragger::setDefaultDragMode(Dragger::DragMode::DRAGMODE_VERTICAL);
//    x->enter();
//    mCircle->setClamp(false);
      //    mCircle->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), x->getPosition().x(), x->getPosition().y(),x->getPosition().z()));
}

void DrawShapes::onPolyMouseMove(QMouseEvent *event)
{
    if (mShape == Shape::POLYGON && mDrawingState == DrawingState::FINISH){
        //qDebug()<<"h:"<< mPolyHdragger->Dragger::getPosition().z();

        mPoly->setHeight(mPolyHdragger->Dragger::getPosition().z());
        //qDebug()<< mPoly->printHeight();
    }
}

void DrawShapes::onSphereBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {

//        mSphere->model->setPosition(geoPos);
//        mMapController->addNode(mSphere->model);
//        mSphere->setClamp(false);

//        mSphere->model->setPosition(geoPos);
//        mMapController->addNode(mSphere);
//        mSphere->setClamp(false);
        mSphereNode->setRadius(osgEarth::Distance(2000, osgEarth::Units::METERS));
        mSphereNode->setColor(osg::Vec4(0.8f, 0.0f, 1.0, 0.5f));
//        mSphereNode->setPosition(geoPos);

    }
    if(event->button() == Qt::MouseButton::LeftButton && event->type() == QEvent::Type::MouseButtonPress)
    {
//        osgEarth::Symbology::Style style;
//        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
//        style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
        osg::ClipPlane* clipplane = new osg::ClipPlane();
        clipplane->setClipPlane(0, 1, 0, 1);
        osg::ClipNode* cNode = new  osg::ClipNode();
        cNode->addClipPlane(clipplane);
        mRadius = 200000;

//        mSphere->setColor(osg::Vec4(0,0,1,1));
//        mSphere->setRadius(300000);

        //mSphere->setClamp(true);

        mSphereNode->setRadius(osgEarth::Distance(mRadius, osgEarth::Units::METERS));
        geoPos.z() += 300000;
        mSphereNode->setPosition(geoPos);
        cNode->addChild(mSphereNode);
        mMapController->addNode(cNode);

//        mMapController->addNode(mSphereNode);
//        mSphere->setColor(osg::Vec4(1,0,0,1));
//        mSphere->setCenter(osg::Vec3(0,0,0));
//        mSphere->setRadius(mRadius);
//        mSphere->setClamp(true);

    }
}

void DrawShapes::onConeBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mCone->model->setPosition(geoPos);
        mMapController->addNode(mCone);
        osg::Group* abbas = mAnnoLayer->getGroup();
        abbas->addChild(mCone);
    }
}

void DrawShapes::onCylinderBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mCylinder->model->setPosition(geoPos);
        mMapController->addNode(mCylinder);

    }
}

void DrawShapes::onCapsuleBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mCapsule->model->setPosition(geoPos);
        mMapController->addNode(mCapsule);

    }
}

void DrawShapes::onBoxBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mBox->model->setPosition(geoPos);
        mMapController->addNode(mBox);

    }
}

void DrawShapes::onPolygoneBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::RightButton)
    {
        osgEarth::Symbology::Style circleStyle;
        circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::Red, 0.5);
        circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
        circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;



        osgEarth::Annotation::CircleNode* circle = new osgEarth::Annotation::CircleNode;
        circle->set(
                    osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y(), 1000, osgEarth::ALTMODE_RELATIVE),
                    osgEarth::Distance(50, osgEarth::Units::KILOMETERS),
                    circleStyle, osgEarth::Angle(0.0, osgEarth::Units::DEGREES), osgEarth::Angle(360.0, osgEarth::Units::DEGREES), true);
        mCircleGr->addChild(circle);
        geoPos.z() = 0;
        mPoly->addPoints(geoPos.vec3d());


    }

    if(event->button() == Qt::MouseButton::MiddleButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        if (mPoly->getSize() >= 3){
//            mPoly = new Polygone(mMapController,false);
//            mCircleGr = new osg::Group;
            mMapController->addNode(mPoly);
            mMapController->removeNode(mCircleGr);
            mPolyHdragger = new osgEarth::Annotation::SphereDragger(mMapController->getMapNode());
            mPolyHdragger->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y()));
            mMapController->addNode(mPolyHdragger);
            mPolyHdragger->Dragger::setDefaultDragMode(Dragger::DragMode::DRAGMODE_VERTICAL);
            mPoly->setFillColor(osgEarth::Color::Purple);
            mDrawingState = DrawingState::FINISH;
        }
    }

}


void DrawShapes::onImgOvlyBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::LeftButton && !(mDrawingState==DrawingState::START))
    {   
        osg::Image* image = osgDB::readImageFile("/home/client111/Downloads/icons8-usa-32.png");
        if (image)
        {
            mDrawingState = DrawingState::START;
            mImageOverlay = new osgEarth::Annotation::ImageOverlay(mMapController->getMapNode(), image);
            //imageOverlay->setBounds(osgEarth::Bounds(-100.0, 35.0, -90.0, 40.0));
            mImageOverlay->setCenter(geoPos.x(),geoPos.y());
            mMapController->addNode(mImageOverlay);
            mImgOvlEditor = new osgEarth::Annotation::ImageOverlayEditor(mImageOverlay, false);
            //mMapController->getMapNode()->addChild(mImgOvlEditor);


        }
    }

    if(event->button() == Qt::MouseButton::RightButton && mDrawingState==DrawingState::START)
    {
        mDrawingState = DrawingState::DELETE;
        mMapController->removeNode(mImageOverlay);
        mMapController->removeNode(mImgOvlEditor);
        mImageOverlay = nullptr;
        mImgOvlEditor = nullptr;
    }

    if(event->button() == Qt::MouseButton::MidButton && mDrawingState==DrawingState::START)
    {
        mDrawingState = DrawingState::FINISH;
        mMapController->removeNode(mImgOvlEditor);
        mImageOverlay = nullptr;
        mImgOvlEditor = nullptr;
    }
}

void DrawShapes::onCircleBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::LeftButton && !(mDrawingState==DrawingState::START))
    {
        mDrawingState = DrawingState::START;

        mCircle = new Circle(mMapController,false);
        mCircle->setArcStart(0);
        mCircle->setArcEnd(360);


        //mMapController->addNode(mCircleEditor);
            mCircleHdragger->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y()));
            mMapController->addNode(mCircleHdragger);
            mCircleHdragger->Dragger::setDefaultDragMode(Dragger::DragMode::DRAGMODE_VERTICAL);
        mCircle->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y()));
        mMapController->addNode(mCircle);
        mCircleEditor = new osgEarth::Annotation::CircleNodeEditor(mCircle);
        mCircleEditor->getPositionDragger()->setDefaultDragMode(Dragger::DragMode::DRAGMODE_VERTICAL);
        //mMapController->addNode(mCircleEditor);


        event->accept();
    }
    if(event->button() == Qt::MouseButton::RightButton && mDrawingState==DrawingState::START)
    {


        mDrawingState = DrawingState::DELETE;
        mMapController->removeNode(mCircle);
        mMapController->removeNode(mCircleEditor);
        mCircle = nullptr;
        mCircleEditor = nullptr;
////        mCircle->setClamp(false);
////        mCircle->setColor(osgEarth::Color::Blue);
////        mCircle->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(90.0),osg::Y_AXIS));
////        mCircle->setArcStart(-60);
////        mCircle->setArcEnd(60);
////        mCircle->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y(),geoPos.z()+1000000));
////        mCircle->setCircleHeight(25000);
        event->accept();
    }
//    if(event->button() == Qt::MouseButton::MidButton && mDrawingState==DrawingState::START)
//    {
//        mDrawingState = DrawingState::FINISH;
//        mMapController->removeNode(mCircleEditor);
//        mCircle = nullptr;
//        mCircleEditor = nullptr;
//        event->accept();
//    }
}

void DrawShapes::onRectBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::LeftButton && !(mDrawingState==DrawingState::START))
    {
        mDrawingState = DrawingState::START;
        mRect = new Rect(true, 600, 300);
        mRect->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y()));
        mRectEditor = new osgEarth::Annotation::RectangleNodeEditor(mRect);
        mMapController->addNode(mRect);
        mMapController->addNode(mRectEditor);
        event->accept();


    }
    if(event->button() == Qt::MouseButton::RightButton && mDrawingState==DrawingState::START)
    {
        mDrawingState = DrawingState::DELETE;
        mMapController->removeNode(mRect);
        mMapController->removeNode(mRectEditor);
        mRect = nullptr;
        mRectEditor = nullptr;
        event->accept();
//        mRect->setClamp(false);
//        mRect->setWidth(osgEarth::Distance(600, osgEarth::Units::KILOMETERS));
//        mRect->setHeight(osgEarth::Distance(300, osgEarth::Units::KILOMETERS));
//        mRect->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(90.0),osg::Y_AXIS));
//        mRect->setColor(osgEarth::Color::Blue);
    }
    if(event->button() == Qt::MouseButton::MiddleButton && mDrawingState==DrawingState::START)
    {
        mDrawingState = DrawingState::FINISH;
        mMapController->removeNode(mRectEditor);
        mRect = nullptr;
        mRectEditor = nullptr;
        event->accept();
    }
}

void DrawShapes::onEllipseBtnClick(QMouseEvent *event)
{
    osg::Vec3d worldPos;
    mMapController->screenToWorld(event->x(), event->y(), worldPos);
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapController->getMapSRS(), worldPos);

    if(event->button() == Qt::MouseButton::LeftButton && !(mDrawingState==DrawingState::START))
    {
        mDrawingState = DrawingState::START;
        event->accept();
        mEllipse = new Ellipse(true);
        mEllipse->setPosition(osgEarth::GeoPoint(mMapController->getMapSRS(), geoPos.x(), geoPos.y()));
        mMapController->addNode(mEllipse);
        mElpsEditor = new EllipseNodeEditor(mEllipse);
        mElpsEditor->getPositionDragger()->setDefaultDragMode(Dragger::DragMode::DRAGMODE_VERTICAL);
        mMapController->addNode(mElpsEditor);
    }
    if(event->button() == Qt::MouseButton::RightButton && mDrawingState==DrawingState::START)
    {
//        mEllipse->setClamp(false);
//        mEllipse->setColor(osgEarth::Color::Blue);
//        mEllipse->setHeight(250000);
        mDrawingState = DrawingState::DELETE;
        mMapController->removeNode(mEllipse);
        mMapController->removeNode(mElpsEditor);
        mEllipse = nullptr;
        mElpsEditor = nullptr;
    }
    if(event->button() == Qt::MouseButton::MiddleButton && mDrawingState==DrawingState::START)
    {
        mDrawingState = DrawingState::FINISH;
        mEllipse->setHeight(250000);
        mMapController->removeNode(mElpsEditor);
        mEllipse = nullptr;
        mElpsEditor = nullptr;

    }
}

