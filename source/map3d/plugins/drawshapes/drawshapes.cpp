#include "drawshapes.h"
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
#include <osgEarthAnnotation/ImageOverlayEditor>
#include "osgEarthAnnotation/AnnotationEditing"

using namespace osgEarth::Annotation;

DrawShapes::DrawShapes(QWidget *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(res);
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
    Q_UNUSED(engine)
    desc->toolboxItemsList.push_back(new ItemDesc{LINE, CATEGORY, "qrc:/res/line.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{SPHERE, CATEGORY, "qrc:/res/sphere.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{CONE, CATEGORY, "qrc:/res/cone.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{CYLINDER, CATEGORY, "qrc:/res/cylinder.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{CAPSULE, CATEGORY, "qrc:/res/capsule.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{BOX, CATEGORY, "qrc:/res/box.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{POLYGON, CATEGORY, "qrc:/res/polygon.png", true});
    //desc->toolboxItemsList.push_back(new ItemDesc{EXTRPOLY, CATEGORY, "qrc:/res/extroPolygon.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{IMAGE_OVERLAY, CATEGORY, "qrc:/res/image.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{CIRCLE, CATEGORY, "qrc:/res/circle.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{ELLIPSE, CATEGORY, "qrc:/res/ellipse.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{RECT, CATEGORY, "qrc:/res/rectangle.png", true});
    return true;
}

void DrawShapes::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
{

    if(CATEGORY == category && name == LINE)
    {
        if(checked)
        {
            mLine = new Line(mMapController);
            shape = Shape::line;
        }
        else
        {
            mIsFinished = true;
            mLine->removePoint();
            shape = Shape::noOne;
        }
    }
    if(CATEGORY == category && name == SPHERE)
    {
        if(checked)
        {
        mSphere = new Sphere(mMapController,mRadius, false);
        QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onSphereBtnClick);
        }
        else
        {
        QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onSphereBtnClick);
        }
}
        if(CATEGORY == category && name == CONE)
        {
            if(checked)
            {
            mCone = new Cone(mMapController,111100, 111100,false);
            QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onConeBtnClick);
            }
            else
            {
            QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onConeBtnClick);
            }
    }
        if(CATEGORY == category && name == CYLINDER)
        {
            if(checked)
            {
            mCylinder = new Cylinder(mMapController,mRadius, 111111,false);
            QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onCylinderBtnClick);
            }
            else
            {
            QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onCylinderBtnClick);
            }
    }
        if(CATEGORY == category && name == CAPSULE)
        {
            if(checked)
            {
            mCapsule = new Capsule(mMapController,mRadius, 111111,false);
            QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onCapsuleBtnClick);
            }
            else
            {
            QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onCapsuleBtnClick);
            }
    }
        if(CATEGORY == category && name == BOX)
        {
            if(checked)
            {
            mBox = new Box(mMapController,50000, 60000, 40000, false);
            QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onBoxBtnClick);
            }
            else
            {
            QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onBoxBtnClick);
            }
    }
    if(CATEGORY == category && name == POLYGON)
    {
        if(checked)
        {
        mCircleGr = new osg::Group;
        mMapController->addNode(mCircleGr);
        mPoly = new Polygone(mMapController, true);
        mPoly->setFillColor(osgEarth::Color::Green);
        QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onPolygoneBtnClick);
        }
        else
        {
        QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onPolygoneBtnClick);
        }
    }
//    if(CATEGORY == category && name == EXTRPOLY)
//    {

//    }
    if(CATEGORY == category && name == IMAGE_OVERLAY)
    {
        if(checked)
        {

        QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onImgOvlyBtnClick);
        }
        else
        {
        QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onImgOvlyBtnClick);
        }
    }
    if(CATEGORY == category && name == CIRCLE)
    {
        if(checked)
        {
        mCircle = new Circle(mMapController,true);
        QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onCircleBtnClick);
        }
        else
        {
        QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onCircleBtnClick);
        }

    }
    if(CATEGORY == category && name == ELLIPSE)
    {
        if(checked)
        {
        mEllipse = new Ellipse(true);
        QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onEllipseBtnClick);
        }
        else
        {
        QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onEllipseBtnClick);
        }
    }
    if(CATEGORY == category && name == RECT)
    {
        if(checked)
        {
        mRect = new Rect(true, 600, 300);
        QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onRectBtnClick);
        }
        else
        {
        QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onRectBtnClick);
        }
    }
}

bool DrawShapes::setup(MapController *mapController,
                              NetworkManager *networkManager,
                              UIHandle *UIHandle)
{
    mMapController = mapController;
    osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());
//    auto abbas = new osgEarth::ModelLayer("abbas", mCircle);
//    mMapController->addLayer(abbas);
//    auto changiz = new osg::Group;
//    changiz->addChild(mCircle);
//    mMapController->addNode(changiz)
    return true;
}

void DrawShapes::mousePressEvent(QMouseEvent *event)
{
    if(shape == Shape::line)
        onLineBtnClick(event);

}

void DrawShapes::mouseMoveEvent(QMouseEvent *event)
{
    if(shape == Shape::line)
        onMouseMove(event);
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

void DrawShapes::onMouseMove(QMouseEvent *event)
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

void DrawShapes::onSphereBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mSphere->model->setPosition(geoPos);
        mMapController->addNode(mSphere);
        mSphere->setClamp(false);
    }
    if(event->button() == Qt::MouseButton::LeftButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mRadius = 200000;
        mSphere->setColor(osg::Vec4(1,0,0,1));
        mSphere->setCenter(osg::Vec3(0,0,0));
        mSphere->setRadius(mRadius);
        mSphere->setClamp(true);
    }
}

void DrawShapes::onConeBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mCone->model->setPosition(geoPos);
        mMapController->addNode(mCone);

    }
}

void DrawShapes::onCylinderBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mCylinder->model->setPosition(geoPos);
        mMapController->addNode(mCylinder);

    }
}

void DrawShapes::onCapsuleBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mCapsule->model->setPosition(geoPos);
        mMapController->addNode(mCapsule);

    }
}

void DrawShapes::onBoxBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mBox->model->setPosition(geoPos);
        mMapController->addNode(mBox);

    }
}

void DrawShapes::onPolygoneBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        osgEarth::Symbology::Style circleStyle;
        circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::Red, 0.5);
        circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
        circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;

        circle = new osgEarth::Annotation::CircleNode;
        circle->set(
                    osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), geoPos.x(), geoPos.y(), 1000, osgEarth::ALTMODE_RELATIVE),
                    osgEarth::Distance(50, osgEarth::Units::KILOMETERS),
                    circleStyle, osgEarth::Angle(0.0, osgEarth::Units::DEGREES), osgEarth::Angle(360.0, osgEarth::Units::DEGREES), true);
        mCircleGr->addChild(circle);
        geoPos.z() = 0;
        mPoly->addPoints(geoPos.vec3d());

    }

    if(event->button() == Qt::MouseButton::MiddleButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        if (mPoly->getSize() >= 3){
        mMapController->addNode(mPoly);
        mMapController->removeNode(mCircleGr);
        mPoly = new Polygone(mMapController,true);
        mCircleGr = new osg::Group;
        mMapController->addNode(mCircleGr);



       }
    }

}


void DrawShapes::onImgOvlyBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        osgEarth::Annotation::ImageOverlay* imageOverlay = nullptr;
        osg::Image* image = osgDB::readImageFile("/home/client111/Downloads/icons8-usa-32.png");
        if (image)
        {
            imageOverlay = new osgEarth::Annotation::ImageOverlay(mMapController->getMapNode(), image);
            //imageOverlay->setBounds(osgEarth::Bounds(-100.0, 35.0, -90.0, 40.0));
            imageOverlay->setCenter(geoPos.x(),geoPos.y());
            mMapController->addNode(imageOverlay);
            osg::Node* editor = new osgEarth::Annotation::ImageOverlayEditor( imageOverlay, false );
            osg::Group* gr = new osg::Group;
            gr->addChild(editor);
            mMapController->getMapNode()->addChild(gr);

        }
    }
}

void DrawShapes::onCircleBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mCircle->setPosition(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), geoPos.x(), geoPos.y()));
        mCircle->setArcStart(0);
        mCircle->setArcEnd(360);
        mMapController->addNode(mCircle);
        osgEarth::Annotation::CircleNodeEditor* editor = new osgEarth::Annotation::CircleNodeEditor(mCircle);
//        osg::Group* gr = new osg::Group;
//        gr->addChild(editor);
        mMapController->addNode(editor);
    }
//    if(event->button() == Qt::MouseButton::LeftButton && event->type() == QEvent::Type::MouseButtonPress)
//    {
//        mCircle->setClamp(false);
//        mCircle->setColor(osgEarth::Color::Blue);
//        mCircle->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(90.0),osg::Y_AXIS));
//        mCircle->setArcStart(-60);
//        mCircle->setArcEnd(60);
//        mCircle->setPosition(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), geoPos.x(), geoPos.y(),geoPos.z()+1000000));
//        mCircle->setCircleHeight(25000);


//    }
}

void DrawShapes::onRectBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mRect->setPosition(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), geoPos.x(), geoPos.y()));
        mMapController->addNode(mRect);
    }
    if(event->button() == Qt::MouseButton::LeftButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mRect->setClamp(false);
        mRect->setWidth(osgEarth::Distance(600, osgEarth::Units::KILOMETERS));
        mRect->setHeight(osgEarth::Distance(300, osgEarth::Units::KILOMETERS));
        mRect->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(90.0),osg::Y_AXIS));
        mRect->setColor(osgEarth::Color::Blue);
    }
}






void DrawShapes::onEllipseBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mEllipse->setPosition(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), geoPos.x(), geoPos.y()));
        mMapController->addNode(mEllipse);
        EllipseNodeEditor* editor = new EllipseNodeEditor(mEllipse);
        osg::Group* gr = new osg::Group;
        gr->addChild(editor);
        mMapController->getMapNode()->addChild(gr);
    }
    if(event->button() == Qt::MouseButton::LeftButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mEllipse->setClamp(false);
        mEllipse->setColor(osgEarth::Color::Blue);
        mEllipse->setHeight(250000);
    }
}

