#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgEarthUtil/Sky>
#include <osgEarth/Registry>
#include <QMessageBox>
#include <QTimer>
#include <QQuickOpenGLUtils>
#include <osgGA/TrackballManipulator>
#include <osgEarthDrivers/xyz/XYZOptions>
#include "mapItem.h"


//--MapItem---------------------------------------------------------------------------------------------------------
MapItem::MapItem(QQuickItem *parent) :
    OsgViewerItem(parent)
{
    getViewer()->getCamera()->setClearColor(osg::Vec4(0.15f, 0.15f, 0.15f, 1.0f));
}

MapItem::~MapItem()
{
    // mMapNode->unref();
    // mSkyNode->unref();
    // mMapRoot->unref();
    // delete mMapObject;
    qDebug() << "~Mapitem";
}

void MapItem::setMap(osgEarth::Map *map)
{
    mIsGeocentric = map->isGeocentric();
    createMapNode(mIsGeocentric, map);

    createCameraManipulator();
    getViewer()->setCameraManipulator(mCameraController);
    emit mapCleared();
    mCameraController->home(0);
}

const osg::Group *MapItem::getRoot() const
{
    return mMapRoot;
}

osgEarth::MapNode *MapItem::getMapNode() const
{
    return mMapNode;
}

MapObject *MapItem::getMapObject()
{
    return mMapObject;
}

const MapObject *MapItem::getMapObject() const
{
    return mMapObject;
}

CameraController *MapItem::getCameraController()
{
    return mCameraController;
}

const CameraController *MapItem::getCameraController() const
{
    return mCameraController;
}

const osgEarth::SpatialReference *MapItem::getMapSRS() const
{
    return mMapNode->getMapSRS();
}

bool MapItem::addNode(osg::Node *node)
{
    //osgEarth::Registry::shaderGenerator().run(node);// for textures or lighting
    return mMapNode->addChild(node);
}

bool MapItem::removeNode(osg::Node *node)
{
    return mMapNode->removeChild(node);
}

void MapItem::addCaptureCallback(CaptureCallback *captureCallback)
{
    getViewer()->getCamera()->addFinalDrawCallback(captureCallback);
}

void MapItem::removeCaptureCallback(CaptureCallback *captureCallback)
{
    getViewer()->getCamera()->removeFinalDrawCallback(captureCallback);
}

void MapItem::screenToWorld(float x, float y, osg::Vec3d &outWorldPoint) const
{
    //    float height = static_cast<float>(mOSGRenderNode->getCamera()->getViewport()->height());
    float height = 0;
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (getViewer()->computeIntersections(x, /*height - */y, intersections))
    {
        for (const auto &intersection : intersections)
        {
            //            mCurrentLocalPos    = intersection.getLocalIntersectPoint();
            outWorldPoint = intersection.getWorldIntersectPoint();
            return;
        }
    }
    else
        mCameraController->screenToWorld(x, y,getViewer(), outWorldPoint);
}

osgEarth::GeoPoint MapItem::screenToGeoPoint(float x, float y) const
{
    osg::Vec3d worldPoint;
    screenToWorld(x, y, worldPoint);
    osgEarth::GeoPoint geoPint;
    geoPint.fromWorld(getMapSRS(), worldPoint);
    return geoPint;
}

void MapItem::worldToScreen(osg::Vec3d worldPoint, float &outX, float &outY) const
{
    float height = static_cast<float>(getViewer()->getCamera()->getViewport()->height());
    float width = static_cast<float>(getViewer()->getCamera()->getViewport()->width());

    const osg::Matrixd pMatrix = getViewer()->getCamera()->getProjectionMatrix();
    const osg::Matrixd vMatrix = getViewer()->getCamera()->getViewMatrix();
    osg::Vec3f result =   (worldPoint * vMatrix) * pMatrix;
    outX = result.x() * (width/2.0f) + width/2.0f;
    outY = result.y() * (height/2.0f) + height/2.0f;
    outY = height - outY;

}

void MapItem::worldToOSGScreen(osg::Vec3d worldPoint, float &outX, float &outY) const
{
    float height = static_cast<float>(getViewer()->getCamera()->getViewport()->height());
    float width = static_cast<float>(getViewer()->getCamera()->getViewport()->width());

    const osg::Matrixd pMatrix = getViewer()->getCamera()->getProjectionMatrix();
    const osg::Matrixd vMatrix = getViewer()->getCamera()->getViewMatrix();
    osg::Vec3f result =   (worldPoint * vMatrix) * pMatrix;
    outX = result.x() * (width/2.0f) + width/2.0f;
    outY = result.y() * (height/2.0f) + height/2.0f;
    QPointF point = mapToScene(QPoint(outX, outY));
    outX = point.x();
    outY = point.y();
}

void MapItem::addBaselayers()
{
    osgEarth::Drivers::GDALOptions gdal;
    gdal.maxDataLevelOverride() = 700000;
    gdal.interpolation() = osgEarth::ElevationInterpolation::INTERP_CUBICSPLINE;
    gdal.L2CacheSize() = 2048;
    gdal.url() = (QString(EXTERNAL_RESOURCE_DIR) + QString("/world.tif")).toStdString();
    osg::ref_ptr<osgEarth::ImageLayer> imlayer = new osgEarth::ImageLayer("base-world", gdal);
    mMapObject->addLayer(imlayer);

    osgEarth::Drivers::GDALOptions  opt;
    opt.url() = "../../../../QTMAP3D-DATA/dataosgearth/Tehranelevation/tehran1.tif";
    osg::ref_ptr<osgEarth::ElevationLayer>  layer = new osgEarth::ElevationLayer(osgEarth::ElevationLayerOptions("Terrain", opt));
    getMapObject()->addLayer(layer);

    osgEarth::Drivers::XYZOptions optxyz;
    optxyz.url() = "https://mt1.google.com/vt/lyrs=s&x={x}&y={y}&z={z}";
    optxyz.profile() = { "spherical-mercator" };
    auto imageLayerOptions = osgEarth::ImageLayerOptions("Google S", optxyz);
    osg::ref_ptr<osgEarth::ImageLayer> layerImage2 = new osgEarth::ImageLayer(imageLayerOptions);
    layerImage2->setName("Google S");
    getMapObject()->addLayer(layerImage2);
}

void MapItem::changeMode()
{
    mIs3DView = !mIs3DView;
    auto  settings = mCameraController->getSettings();
    if(mIs3DView) {
        settings->setMinMaxPitch(-90, 0);
    }
    else
    {
        settings->setMinMaxPitch(-90, -90);
        mCameraController->setRotation(osg::Quat());
    }
    emit modeChanged(mIs3DView);
}

bool MapItem::getMode() const
{
    return mIs3DView;
}

void MapItem::setGeocentric(bool isGeocentric)
{
    if (mIsGeocentric == isGeocentric)
        return;

    mIsGeocentric = isGeocentric;
    osgEarth::LayerVector layers;
    mMapNode->getMap()->getLayers(layers);

    createMapNode(mIsGeocentric);
    emit mapCleared();
    for(const auto &layer: layers)
        mMapObject->addLayer(layer);

    osgEarth::Viewpoint vp = mCameraController->getViewpoint();

    createCameraManipulator();
    getViewer()->setCameraManipulator(mCameraController);
    mCameraController->setViewpoint(vp);
}

void MapItem::initializeOsgEarth()
{
    mMapRoot = new osg::Group();
    osgEarth::Util::SkyOptions sopts;
    mSkyNode = osgEarth::Util::SkyNode::create(sopts);
    createMapNode(true);
    getViewer()->setSceneData(mMapRoot);

    //create camera after create map node
    createCameraManipulator();
    getViewer()->setCameraManipulator(mCameraController);
}

void MapItem::createMapNode(bool geocentric, osgEarth::Map *map)
{
    mSkyNode->removeChild(mMapNode);
    mMapRoot->removeChild(mMapNode);
    mMapRoot->removeChild(mSkyNode);

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
            mapOpt.profile() = osgEarth::ProfileOptions("plate-carre");
            mMapObject = new MapObject(mapOpt);
            mMapNode = new osgEarth::MapNode(mMapObject);
        }
    }
    else
        mMapNode = new osgEarth::MapNode(map);
    mSkyNode->addChild(mMapNode);
    mMapRoot->addChild(mSkyNode);
}

void MapItem::createCameraManipulator()
{
    mCameraController = new CameraController(this);
    auto  settings = mCameraController->getSettings();
    //    settings->setSingleAxisRotation(true);

    settings->setMinMaxDistance(0.0, MAX_CAM_DISTANCE);
    if(mIs3DView)
        settings->setMinMaxPitch(-90, 0);
    else
        settings->setMinMaxPitch(-90, -90);

    //    // set home to tehran
    osgEarth::GeoPoint  geoPoint(getMapSRS()->getGeographicSRS(), 51.3347, 35.7219,0);
    geoPoint.transformInPlace(getMapSRS());
    osgEarth::Viewpoint vp;
    vp.focalPoint() = geoPoint;
    vp.range()= 5000000;
    vp.pitch() = -90;
    vp.heading() = 0;
    mCameraController->setHomeViewpoint(vp, 0);

    settings->setTerrainAvoidanceEnabled(true);
    settings->setTerrainAvoidanceMinimumDistance(20);
}

void MapItem::frame()
{
    // Set SkyNode sun light rotate with camera
    //
    auto sunLight = mSkyNode->getSunLight();

    sunLight->setAmbient(osg::Vec4(0.04f, 0.04f, 0.04f, 1));
    sunLight->setDiffuse(osg::Vec4(0.35f, 0.35f, 0.35f, 1));
    sunLight->setSpecular(osg::Vec4(0.5f, 0.5f, 0.5f, 1));

    float splen = sunLight->getPosition().length();
    osg::Vec3 spos = mCameraController->getMatrix().getTrans();
    spos.normalize();
    spos *= splen;

    mSkyNode->setSunVisible(false);
    mSkyNode->setAtmosphereVisible(false);
    sunLight->setPosition(osg::Vec4(spos, 0));
    spos.normalize();
    spos *= -1.0f;
    sunLight->setDirection(spos);
}
