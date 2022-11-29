#include "drawshapes.h"

#include "map3dwidget.h"
#include "toolbarwidget.h"
#include <osgEarthSymbology/Style>
#include <osgEarth/ModelLayer>
#include <osgEarthDrivers/arcgis/ArcGISOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/feature_wfs/WFSFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthAnnotation/FeatureNode>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <osgEarth/GLUtils>
#include <osgViewer/Viewer>
#include <osgEarth/GLUtils>
#include <osg/ShapeDrawable>
#include <osg/ClipNode>
#include <osg/ClipPlane>
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


DrawShapes::DrawShapes(QWidget *parent)
    : PluginInterface(parent)
{

}
void DrawShapes::setUpUI()
{

    ToolBarWidget::Category cat = ToolBarWidget::Category::Draw;
    QString nameLine = "Line";
    mToolBar->addItem(cat, nameLine, "qrc:/res/line.png", true);

    QString nameSphere = "Sphere";
    mToolBar->addItem(cat, nameSphere, "qrc:/res/sphere.png", true);

    QString namePolygone = "Polygone";
    mToolBar->addItem(cat, namePolygone, "qrc:/res/polygon.png", true);

    QString nameExtrPoly = "ExtrPoly";
    mToolBar->addItem(cat, nameExtrPoly, "qrc:/res/extroPolygon.png", true);

    QString nameImageOverlay = "ImageOverlay";
    mToolBar->addItem(cat, nameImageOverlay, "", false);

    QString nameCircle = "Circle";
    mToolBar->addItem(cat, nameCircle, "", false);

    QString nameEllipse = "Ellipse";
    mToolBar->addItem(cat, nameEllipse, "", false);

    QString nameRect = "Rect";
    mToolBar->addItem(cat, nameRect, "", true);

    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name, bool isCheck){
        if(cat == category && name == nameLine)
        {
            if (isCheck){
            QObject::connect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onLineBtnClick);

            }
            else{
             QObject::disconnect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onLineBtnClick);
             QObject::disconnect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onMouseMove);
            }
        }

        if(cat == category && name == nameSphere)
        {
            if(isCheck)
            {
                QObject::connect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onSphereBtnClick);
            }
            else
            {
                QObject::disconnect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onSphereBtnClick);
            }
        }

        if(cat == category && name == namePolygone)
        {
            if(isCheck)
            {
                QObject::connect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onPolygoneBtnClick);
            }
            else
            {
                QObject::disconnect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onPolygoneBtnClick);
            }
        }

        if(cat == category && name == nameExtrPoly)
        {
            if(isCheck)
            {
                QObject::connect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onExtrPolyBtnClick);
            }
            else
            {
                QObject::disconnect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onExtrPolyBtnClick);
            }
        }

        if(cat == category && name == nameRect)
        {
            if(isCheck)
            {
                QObject::connect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onRectBtnClick);
            }
            else
            {
                QObject::disconnect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onRectBtnClick);
            }
        }

        if(cat == category && name == nameCircle)
        {
            if(isCheck)
            {
                QObject::connect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onCircleBtnClick);
            }
            else
            {
                QObject::disconnect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onCircleBtnClick);
            }
        }
//        if(cat == category && name == nameSphere)
//        {
//            drawSphere();
//        }
//        if(cat == category && name == namePolygone)
//        {
//            drawPolygone();
//        }
//        if(cat == category && name == nameExtrPoly)
//        {
//            drawExtrPoly();
//        }
//        if(cat == category && name == nameImageOverlay)
//        {
//            drawImageOverlay();
//        }
//        if(cat == category && name == nameCircle)
//        {
//            drawCircle();
//        }
//        if(cat == category && name == nameEllipse)
//        {
//            drawEllipse();
//        }
//        if(cat == category && name == nameRect)
//        {
//            drawRect();
//        }

    });
}

void DrawShapes::drawLine()
{
    osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(linePath, osgEarth::SpatialReference::get("wgs84"));
    pathFeature->geoInterp() = osgEarth::GEOINTERP_RHUMB_LINE;
    osgEarth::Symbology::Style pathStyle;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->color() = osgEarth::Color::Red;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->stroke()->width() = 4.0f;
    pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()->tessellationSize() = 75000;
    //pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->size() = 5;
    //pathStyle.getOrCreate<osgEarth::Symbology::PointSymbol>()->fill()->color() = osgEarth::Color::Red;
    pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;

    //OE_INFO << "Path extent = " << pathFeature->getExtent().toString() << std::endl;

   pathNode = new osgEarth::Annotation::FeatureNode(pathFeature, pathStyle);
   mMap3dWidget->addNode(pathNode);
}


osgEarth::Annotation::ModelNode* DrawShapes::mDrawSphere(float radius)
{
    osgEarth::GLUtils::setGlobalDefaults(mMap3dWidget->getOsgViewer()->getCamera()->getOrCreateStateSet());

    osg::Vec3 eye(0.3f,0.2f,1.0f);
    osg::ref_ptr<osg::Sphere> pSphereShape = new osg::Sphere(eye, radius);
    osg::ref_ptr<osg::ShapeDrawable> pShapeDrawable = new osg::ShapeDrawable(pSphereShape.get());

    pShapeDrawable->setColor(osg::Vec4(0.0, 0.5f, 1.0, 0.3f));

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(1.0), osg::StateAttribute::ON);

    geode->addDrawable(pShapeDrawable.get());



    // osg::ref_ptr<osg::Node> mDrownode = makeShape(osg::Vec3(position.x(),position.y(),position.z()),20000.0f);
    osgEarth::Symbology::Style style;

    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_ABSOLUTE;
    style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(geode);

    osg::ref_ptr<osgEarth::Annotation::ModelNode> model;
    model = new osgEarth::Annotation::ModelNode(mMap3dWidget->getMapNode(), style);

    return model.release();
}

void DrawShapes::onLineBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::LeftButton && event->type() ==  QEvent::Type::MouseButtonPress)
    {
        cancelBtn = false;
        QObject::connect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onMouseMove);
        osgEarth::GLUtils::setGlobalDefaults(mMap3dWidget->getOsgViewer()->getCamera()->getOrCreateStateSet());

        linePath->push_back(geoPos.vec3d());
        drawLine();
    }
    if(event->button() == Qt::MouseButton::RightButton && event->type() ==  QEvent::Type::MouseButtonPress){
        linePath->pop_back();
        linePath->clear();
        cancelBtn = true;
        mMap3dWidget->removeNode(pathNode);

    }
}

void DrawShapes::onSphereBtnClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {

        auto node = mDrawSphere(20000.0f);

        osg::ClipNode* clipnode = new osg::ClipNode;
        osg::ClipPlane* clipplane = new osg::ClipPlane();
        clipplane->setClipPlane(0, 1, 0, 1);

        clipnode->addClipPlane(clipplane);
        clipnode->addChild(node);
        //osgEarth::GeoPoint point(osgEarth::SpatialReference::get("wgs84"), 52.859, 35.461);
        // geoPos.z() += 20000;
        node->setPosition(geoPos);
        mMap3dWidget->addNode(clipnode);
        mMap3dWidget->goPosition(geoPos.x(), geoPos.y(), 100000);

    }
}

void DrawShapes::onPolygoneBtnClick(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
        osgEarth::GLUtils::setGlobalDefaults(mMap3dWidget->getOsgViewer()->getCamera()->getOrCreateStateSet());
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
        osgEarth::Annotation::FeatureNode* pathNode = nullptr;
        mMap3dWidget->addNode(pathNode);
        mMap3dWidget->addNode(fnode);

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
    mMap3dWidget->addNode(featureNode);
    }
}

void DrawShapes::onCircleBtnClick(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::RightButton && event->type() == QEvent::Type::MouseButtonPress)
    {
    osgEarth::Symbology::Style ellipseStyle;
    ellipseStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::Orange, 0.75);
    ellipseStyle.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>()->height() = 250000.0; // meters MSL
    osgEarth::Annotation::EllipseNode* ellipse = new osgEarth::Annotation::EllipseNode;
    ellipse->set(
                osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),-80.28, 25.82, 0.0, osgEarth::ALTMODE_RELATIVE),
                osgEarth::Distance(250, osgEarth::Units::MILES),
                osgEarth::Distance(250, osgEarth::Units::MILES),
                osgEarth::Angle(0, osgEarth::Units::DEGREES),
                ellipseStyle,
                osgEarth::Angle(360.0, osgEarth::Units::DEGREES),
                //Angle(360.0 - 45.0, Units::DEGREES),
                true);
    }
}

void DrawShapes::onRectBtnClick(QMouseEvent *event)
{
    osgEarth::Symbology::Style rectStyle;
    rectStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Color(osgEarth::Color::Red, 0.5);
    rectStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    rectStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    osgEarth::Annotation::RectangleNode* rect = new osgEarth::Annotation::RectangleNode(
                osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), -117.172, 32.721),
                osgEarth::Distance(300, osgEarth::Units::KILOMETERS),
                osgEarth::Distance(600, osgEarth::Units::KILOMETERS),
                rectStyle);
    mMap3dWidget->addNode(rect);

}
void DrawShapes::onMouseMove(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if (event->type() ==  QEvent::Type::MouseMove){
        if (!cancelBtn){
        if (linePath->size()>=2){
            linePath->pop_back();
             mMap3dWidget->removeNode(pathNode);
        }

        linePath->push_back(geoPos.vec3d());
        drawLine();

}
    }

}
