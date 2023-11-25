#include "mapItem.h"
#include "smallMap.h"
#include <osgViewer/config/SphericalDisplay>
#include <osgEarth/GeoData>
#include <osgEarth/ImageLayer>
#include <osgEarth/MapNode>
#include <osgEarth/MapOptions>
#include <osgEarth/ModelLayer>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osg/BlendFunc>
#include <osg/StateSet>
#include <osgEarthSymbology/Geometry>
#include <osgEarthSymbology/Style>




SmallMap::SmallMap(QQuickItem *parent):
    OsgViewerItem(parent)
{
    getViewer()->getCamera()->setClearColor(osg::Vec4(0,0,0,0));
    getViewer()->getCamera()->setClearMask(GL_DEPTH_BUFFER_BIT);
//    getViewer()->getCamera()->setProjectionResizePolicy( osg::Camera::FIXED );
//    getViewer()->getCamera()->setProjectionMatrixAsPerspective(30.0, double(100) / double(100), 1.0, 1000.0);

    mCameraManipulator = new osgEarth::Util::EarthManipulator;
    getViewer()->setCameraManipulator(mCameraManipulator);
    mMapRoot = new osg::Group();
    getViewer()->setSceneData(mMapRoot);

    compile(true);
}

void SmallMap::setMainMapItem(MapItem *mapItem)
{
    mMapItem = mapItem;
}

void SmallMap::compile(bool geocentric)
{


    createMapNode(geocentric);

    osgEarth::Drivers::GDALOptions gdal;
//    gdal.maxDataLevelOverride() = 7000000;
//    gdal.interpolation() = osgEarth::ElevationInterpolation::INTERP_AVERAGE;
//    gdal.L2CacheSize() = 2048;
    gdal.url() = (QString(EXTERNAL_RESOURCE_DIR) + QString("/world_simple.tif")).toStdString();
    osg::ref_ptr<osgEarth::ImageLayer> imlayer = new osgEarth::ImageLayer("base-world", gdal);
    mMap->addLayer(imlayer);
}


void SmallMap::createMapNode(bool geocentric)
{
    mMapRoot->removeChild(mMapNode);

    osgEarth::MapNodeOptions mapNodeOptions;
    mapNodeOptions.enableLighting() = false;

    osgEarth::MapOptions mapOpt;
    if(geocentric)
    {
        mapOpt.coordSysType() = osgEarth::MapOptions::CSTYPE_GEOCENTRIC;
        mapOpt.profile() = osgEarth::ProfileOptions("global-mercator");
        mMap = new osgEarth::Map(mapOpt);
        mMapNode = new osgEarth::MapNode(mMap, mapNodeOptions);
    }
    else
    {
        mapOpt.coordSysType() = osgEarth::MapOptions::CSTYPE_PROJECTED;
        mapOpt.profile() = osgEarth::ProfileOptions("plate-carre");
        mMap = new osgEarth::Map(mapOpt);
        mMapNode = new osgEarth::MapNode(mMap, mapNodeOptions);
    }

    mMapRoot->addChild(mMapNode);
}


void SmallMap::frame()
{
    if(mMapItem){
        tmpVP = mCameraManipulator->getViewpoint();
        tmpVP.focalPoint() = mMapItem->getCameraController()->getViewpoint().focalPoint();
        mCameraManipulator->setViewpoint(tmpVP);
    }
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




