#include "smallmap.h"
#include <osgViewer/config/SphericalDisplay>
#include <osgEarth/MapOptions>
#include <osgEarth/ModelLayer>
#include <osgEarth/VirtualProgram>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osg/BlendFunc>
#include <osg/StateSet>

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

    createCameraManipulator();

//    if (mMapNode)
//    {
//        const char* modelTransparencyFrag = R"(
//    #version 330
//    uniform float modelTransparencyValue;
//    void modelTransparencyFunc (inout vec4 color) {
//         color = vec4(color.r, color.g, color.b, 0.7);
//    })";

//        osgEarth::VirtualProgram* vp = osgEarth::VirtualProgram::getOrCreate(mMapNode->getOrCreateStateSet());
//        vp->setFunction("modelTransparencyFunc", modelTransparencyFrag, osgEarth::ShaderComp::LOCATION_FRAGMENT_COLORING);

//        osg::Uniform* transparentUniform = new osg::Uniform("modelTransparencyValue", 0.5f);
//        mMapNode->getOrCreateStateSet()->addUniform(transparentUniform);
//    }


    this->oSGRenderNode()->setCameraManipulator(mCameraController);

}

void SmallMap::setLocation(const osgEarth::GeoPoint &geoPos)
{

    this->getCameraController()->goToPosition(geoPos,30000,3);
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

void SmallMap::createCameraManipulator()
{
    mCameraController = new CameraController(this);
    auto  settings = mCameraController->getSettings();
    settings->setMinMaxDistance(0.0, MAX_CAM_DISTANCE);
    settings->setMinMaxPitch(-90, 0);

    osgEarth::Viewpoint vp;
    vp.range()= 32000000;
    vp.pitch() = -90;
    vp.heading() = 0;
    mCameraController->setHomeViewpoint(vp, 0);
    settings->setMinMaxPitch(-90, 0);




}

void SmallMap::frame()
{

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




