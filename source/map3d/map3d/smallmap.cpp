#include "smallmap.h"
#include <osgViewer/config/SphericalDisplay>
#include <osgEarth/GeoData>
#include <osgEarth/MapNode>
#include <osgEarth/MapOptions>
#include <osgEarth/ModelLayer>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osg/BlendFunc>
#include <osg/StateSet>
#include <osgEarthSymbology/Geometry>
#include <osgEarthSymbology/Style>




SmallMap::SmallMap(QQuickItem *parent):
    MapItem(parent)
{

    this->getViewer()->getCamera()->setClearColor(osg::Vec4(0,0,0,0));
    this->getViewer()->getCamera()->setClearMask(GL_DEPTH_BUFFER_BIT);
    this->getViewer()->getCamera()->setProjectionResizePolicy( osg::Camera::FIXED );
    this->getViewer()->getCamera()->setProjectionMatrixAsPerspective(30.0, double(100) / double(100), 1.0, 1000.0);
    initializeOsgEarth();

}

void SmallMap::initializeOsgEarth()
{
    mCameraController = this->getCameraController();
    mMapObject = this->getMapObject();
    mMapRoot = new osg::Group();

    createMapNode(true,mMapObject);
    this->oSGRenderNode()->setSceneData(mMapRoot);

    osgEarth::Drivers::GDALOptions gdal;
    gdal.maxDataLevelOverride() = 700000;
    gdal.interpolation() = osgEarth::ElevationInterpolation::INTERP_CUBICSPLINE;
    gdal.L2CacheSize() = 2048;
    gdal.url() = (QString(EXTERNAL_RESOURCE_DIR) + QString("/world_simple.tif")).toStdString();
    osg::ref_ptr<osgEarth::ImageLayer> imlayer = new osgEarth::ImageLayer("base-world", gdal);
    mMapObject->addLayer(imlayer);
    mCameraController = new CameraController(this);
    focalRect = new osgEarth::Annotation::RectangleNode;
    osgEarth::Symbology::Style rectseStyle;
    rectseStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =  osgEarth::Color("01AED6");
    rectseStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color().a() = 0.7;
    rectseStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    focalRect->setStyle(rectseStyle);
    focalRect->setHeight(1200000);
    focalRect->setWidth(1200000);
    mMapRoot->addChild(focalRect);
    focalRect->setPosition(mCameraController->getViewpoint().focalPoint().get());

    this->oSGRenderNode()->setCameraManipulator(mCameraController);
}


void SmallMap::createMapNode(bool geocentric, osgEarth::Map *map)
{
    mMapRoot->removeChild(mMapNode);
    if(!map)
    {
        osgEarth::MapOptions mapOpt;
        if(geocentric)
        {
            mapOpt.coordSysType() = osgEarth::MapOptions::CSTYPE_GEOCENTRIC;
            mapOpt.profile() = osgEarth::ProfileOptions("global-mercator");
            mMapObject = new MapObject(mapOpt);
            mMapNode = new osgEarth::MapNode(mMapObject);
        }
        else
        {
            mapOpt.coordSysType() = osgEarth::MapOptions::CSTYPE_PROJECTED;
            mapOpt.profile() = osgEarth::ProfileOptions();
            mMapObject = new MapObject(mapOpt);
            mMapNode = new osgEarth::MapNode(mMapObject);
        }
    }
    else
        mMapNode = new osgEarth::MapNode(map);
    mMapRoot->addChild(mMapNode);
}


void SmallMap::frame()
{
    osgEarth::Viewpoint tmpVP;
    tmpVP = mainMapCamera->getViewpoint();
    tmpVP.setRange(22000000);
    tmpVP.setPitch(-90);
    mCameraController->setViewpoint(tmpVP);

//    focalRect->setPosition(mCameraController->getViewpoint().focalPoint().get());
}

void SmallMap::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}

void SmallMap::keyReleaseEvent(QKeyEvent *event)
{
    event->ignore();
}

void SmallMap::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
}

void SmallMap::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
}

void SmallMap::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->ignore();
}

void SmallMap::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
}

void SmallMap::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

void SmallMap::hoverMoveEvent(QHoverEvent *event)
{
    event->ignore();
}

void SmallMap::setMainMap(CameraController *camera, osgViewer::Viewer *viewer)
{
    mainMapCamera = camera;
    mainMapView = viewer;
}




