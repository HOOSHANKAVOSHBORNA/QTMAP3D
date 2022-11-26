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
    mToolBar->addItem(cat, nameSphere, "", false);

    QString namePolygone = "Polygone";
    mToolBar->addItem(cat, namePolygone, "", false);

    QString nameExtrPoly = "ExtrPoly";
    mToolBar->addItem(cat, nameExtrPoly, "", false);

    QString nameImageOverlay = "ImageOverlay";
    mToolBar->addItem(cat, nameImageOverlay, "", false);

    QString nameCircle = "Circle";
    mToolBar->addItem(cat, nameCircle, "", false);

    QString nameEllipse = "Ellipse";
    mToolBar->addItem(cat, nameEllipse, "", false);

    QString nameRect = "Rect";
    mToolBar->addItem(cat, nameRect, "", false);

    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name, bool isCheck){
        if(cat == category && name == nameLine)
        {
            QObject::connect(mMap3dWidget,&Map3dWidget::mouseEvent, this, &DrawShapes::onMouseClick);
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

   osgEarth::Annotation::FeatureNode* pathNode = new osgEarth::Annotation::FeatureNode(pathFeature, pathStyle);
   mMap3dWidget->addNode(pathNode);
}

void DrawShapes::onMouseClick(QMouseEvent *event, osgEarth::GeoPoint geoPos)
{
    if(event->button() == Qt::MouseButton::LeftButton && event->type() ==  QEvent::Type::MouseButtonPress)
    {
        osgEarth::GLUtils::setGlobalDefaults(mMap3dWidget->getOsgViewer()->getCamera()->getOrCreateStateSet());

        linePath->push_back(geoPos.vec3d());
        if (linePath->size()>=2){
            drawLine();
        }
    }
    if(event->button() == Qt::MouseButton::RightButton && event->type() ==  QEvent::Type::MouseButtonPress){
        linePath->clear();
    }


}
