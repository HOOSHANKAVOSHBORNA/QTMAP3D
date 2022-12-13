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

#include <line.h>
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

bool DrawShapes::initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc)
{
    Q_UNUSED(engine)
    desc->toolboxItemsList.push_back(new ItemDesc{LINE, CATEGORY, "qrc:/res/line.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{SPHERE, CATEGORY, "qrc:/res/sphere.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{POLYGON, CATEGORY, "qrc:/res/polygon.png", true});
    desc->toolboxItemsList.push_back(new ItemDesc{EXTRPOLY, CATEGORY, "qrc:/res/extroPolygon.png", true});
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
            mLine = new Line();
            QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onLineBtnClick);
            QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onMouseMove);
        }
        else
        {
            QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onLineBtnClick);
            QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onMouseMove);
        }
    }
    if(CATEGORY == category && name == SPHERE)
    {
        if(checked)
        {
        mSphere = new Sphere(mMapController,mRadius);        
        QObject::connect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onSphereBtnClick);
        }
        else
        {
        QObject::disconnect(mMapController,&MapController::mouseEvent, this, &DrawShapes::onSphereBtnClick);
        }
        
    }
    if(CATEGORY == category && name == POLYGON)
    {

    }
    if(CATEGORY == category && name == EXTRPOLY)
    {

    }
    if(CATEGORY == category && name == IMAGE_OVERLAY)
    {

    }
    if(CATEGORY == category && name == CIRCLE)
    {

    }
    if(CATEGORY == category && name == ELLIPSE)
    {

    }
    if(CATEGORY == category && name == RECT)
    {

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





void DrawShapes::onLineBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::LeftButton && event->type() ==  QEvent::Type::MouseButtonPress)
    {
        mIsFinished = false;

        mLine->addPoint(geoPos.vec3d());
        mLine->setLineStyle(osgEarth::Color::Purple,7,false);
        mMapController->addNode(mLine->getNode());
    }
    if(event->button() == Qt::MouseButton::RightButton && event->type() ==  QEvent::Type::MouseButtonPress)
    {
        //mLine->setPointStyle(osgEarth::Color::Purple,20,true);

        mLine->removePoint();
        mIsFinished = true;
        mLine = new Line();
    }

}

void DrawShapes::onMouseMove(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if (event->type() ==  QEvent::Type::MouseMove)
    {
         if (!mIsFinished)
         {
             if (mLine->getSize() >= 2)
             {
                 mLine->removePoint();
            }

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
    }
    if(event->button() == Qt::MouseButton::LeftButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        mRadius = 200000;
        mSphere->setProp(osg::Vec4(1,0,0,1),osg::Vec3(0,0,0),mRadius);
    }
}

void DrawShapes::onPolygoneBtnClick(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());
        osgEarth::Features::Geometry* geom = new osgEarth::Features::Polygon();
        geom->push_back(osg::Vec3d(0, 40, 0));
        geom->push_back(osg::Vec3d(-60, 40, 0));
        geom->push_back(osg::Vec3d(-60, 60, 0));
        geom->push_back(osg::Vec3d(0, 60, 0));

        osgEarth::Features::Feature* feature = new osgEarth::Features::Feature(geom, osgEarth::SpatialReference::get("wgs84"));
        feature->geoInterp() = osgEarth::GEOINTERP_RHUMB_LINE;

        osgEarth::Symbology::Style geomStyle;
        geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::White;
        geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 5.0f;
        geomStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
        geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
        geomStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_GPU;

        osgEarth::Annotation::FeatureNode* fnode = new osgEarth::Annotation::FeatureNode( feature, geomStyle);
        //osgEarth::Annotation::FeatureNode* pathNode = nullptr;
        mMapController->addNode(mPathNode);
        mMapController->addNode(fnode);

    }
}

void DrawShapes::onExtrPolyBtnClick(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        osgEarth::Features::Geometry* utah = new osgEarth::Features::Polygon();
        utah->push_back(-114.052, 37.0);
        utah->push_back(-109.054, 37.0);
        utah->push_back(-109.054, 41.0);
        utah->push_back(-111.040, 41.0);
        //utah->push_back(-111.080, 42.059);
        //utah->push_back(-114.080, 42.024);

        osgEarth::Symbology::Style utahStyle;
        utahStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 250000.0; // meters MSL
        utahStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::White, 0.8f);

        osgEarth::Annotation::Feature*     utahFeature = new osgEarth::Annotation::Feature(utah, osgEarth::SpatialReference::get("wgs84"));
        osgEarth::Annotation::FeatureNode* featureNode = new osgEarth::Annotation::FeatureNode(utahFeature, utahStyle);
        mMapController->addNode(featureNode);
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
        }
    }
}

void DrawShapes::onCircleBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        osgEarth::Symbology::Style circleStyle;
        circleStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::Cyan, 0.5);
        circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
        circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;

        osgEarth::Annotation::CircleNode* circle = new osgEarth::Annotation::CircleNode;
        circle->set(
                    osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), geoPos.x(), geoPos.y(), 1000., osgEarth::ALTMODE_RELATIVE),
                    osgEarth::Distance(300, osgEarth::Units::KILOMETERS),
                    circleStyle, osgEarth::Angle(-45.0, osgEarth::Units::DEGREES), osgEarth::Angle(45.0, osgEarth::Units::DEGREES), true);
        mMapController->addNode(circle);
    }
}

void DrawShapes::onRectBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        osgEarth::Symbology::Style rectStyle;
        rectStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::Red, 0.5);
        rectStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
        rectStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
        osgEarth::Annotation::RectangleNode* rect = new osgEarth::Annotation::RectangleNode(
                    osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), geoPos.x(), geoPos.y()),
                    osgEarth::Distance(300, osgEarth::Units::KILOMETERS),
                    osgEarth::Distance(600, osgEarth::Units::KILOMETERS),
                    rectStyle);

        mMapController->addNode(rect);
    }
}

void DrawShapes::onEllipseBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        osgEarth::GLUtils::setGlobalDefaults(mMapController->getViewer()->getCamera()->getOrCreateStateSet());

        osgEarth::Symbology::Style ellipseStyle;
        ellipseStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::Orange, 0.75);
        ellipseStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 250000.0; // meters MSL
        osgEarth::Annotation::EllipseNode* ellipse = new osgEarth::Annotation::EllipseNode;
        ellipse->set(
                    osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),geoPos.x(), geoPos.y(), 0.0, osgEarth::ALTMODE_RELATIVE),
                    osgEarth::Distance(250, osgEarth::Units::MILES),
                    osgEarth::Distance(250, osgEarth::Units::MILES),
                    osgEarth::Angle(0, osgEarth::Units::DEGREES),
                    ellipseStyle,
                    osgEarth::Angle(360.0, osgEarth::Units::DEGREES),
                    //Angle(360.0 - 45.0, Units::DEGREES),
                    true);
        mMapController->addNode(ellipse);

    }
}

