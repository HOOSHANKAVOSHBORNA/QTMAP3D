
#include <iostream>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgEarthUtil/Sky>
#include <osgEarth/Registry>
#include <QMessageBox>
#include <QTimer>
#include <deque>
#include <chrono>

#include "mapcontroller.h"

class MainMapCallback : public osgEarth::MapCallback
{
public:
    MainMapCallback(MapController *mapController) :
        mMapController(mapController)
    {

    }

    void onLayerAdded  (osgEarth::Layer* layer, unsigned index) override
    {
        if(mMapController)
            mMapController->layerAdded(layer, index);
    }
    void onLayerRemoved(osgEarth::Layer* layer, unsigned index) override
    {
        if(mMapController)
            mMapController->layerRemoved(layer, index);
    }

private:
    MapController *mMapController{nullptr};
};

//--MapController---------------------------------------------------------------------------------------------------------
const double MAX_CAM_DISTANCE = 30000000.0;
MapController::MapController(QQuickWindow *window) :
    mWindow(window)
{
}

MapController::~MapController()
{

}

void MapController::setMap(osgEarth::Map *map)
{
    mIsGeocentric = map->isGeocentric();
    createMapNode(mIsGeocentric, map);

    createCameraManipulator();
    getViewer()->setCameraManipulator(getEarthManipulator());
//    mMapNode->getMap()->clear();
    emit mapCleared();
//    mMapNode->getMap()->setLayersFromMap(map);
    goToHome();
}

osgViewer::Viewer *MapController::getViewer() const
{
    return dynamic_cast<osgViewer::Viewer*>(mOsgRenderer);
}

const osg::Group *MapController::getRoot() const
{
    return mMapRoot;
}

osgEarth::MapNode *MapController::getMapNode() const
{
    return mMapNode;
}

const osgEarth::SpatialReference *MapController::getMapSRS() const
{
    return mMapNode->getMapSRS();
}

void MapController::addLayer(osgEarth::Layer *layer)
{

    mMapNode->getMap()->addLayer(layer);
    // Check if the layer is added successfully
    auto added = mMapNode->getMap()->getLayerByName(layer->getName());
    if (added && added->getEnabled())
    {

    }
    else
    {
        QMessageBox::warning(nullptr, tr("Error"), tr("Data loading failed!"));
    }
}

bool MapController::addNode(osg::Node *node)
{
    //osgEarth::Registry::shaderGenerator().run(node);// for textures or lighting
    return mMapNode->addChild(node);
}

bool MapController::removeNode(osg::Node *node)
{
    return mMapNode->removeChild(node);
}

osgEarth::Util::EarthManipulator *MapController::getEarthManipulator() const
{
    return mEarthManipulator;
}

void MapController::setViewpoint(const osgEarth::Viewpoint &vp, double duration_s)
{
    getEarthManipulator()->setViewpoint(vp,duration_s);
}

osgEarth::Viewpoint MapController::getViewpoint() const
{
    if (mEarthManipulator)
        return mEarthManipulator->getViewpoint();
    return osgEarth::Viewpoint();
}

void MapController::setTrackNode(osg::Node *node, double minDistance)
{
    auto vp = getEarthManipulator()->getViewpoint();
    if(vp.getNode() == node)
        return;

    vp.setNode(node);
    getEarthManipulator()->setViewpoint(vp);
    auto camSet = getEarthManipulator()->getSettings();
    camSet->setMinMaxDistance(minDistance,MAX_CAM_DISTANCE);
    camSet->setTetherMode(osgEarth::Util::EarthManipulator::TetherMode::TETHER_CENTER);
    getEarthManipulator()->applySettings(camSet);
}

void MapController::untrackNode(osg::Node *node)
{
    auto vp = getEarthManipulator()->getViewpoint();
    if(vp.getNode() == nullptr)
        return;
    if(vp.getNode() != node)
        return;
    vp.setNode(nullptr);
    getEarthManipulator()->setViewpoint(vp);
    auto camSet = getEarthManipulator()->getSettings();
    camSet->setMinMaxDistance(0,MAX_CAM_DISTANCE);
    getEarthManipulator()->applySettings(camSet);
}

void MapController::screenToWorld(float x, float y, osg::Vec3d &outWorldPoint) const
{
    float height = static_cast<float>(mOsgRenderer->getCamera()->getViewport()->height());
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (mOsgRenderer->computeIntersections(x, height - y, intersections))
    {
        for (const auto &intersection : intersections)
        {
            //            mCurrentLocalPos    = intersection.getLocalIntersectPoint();
            outWorldPoint = intersection.getWorldIntersectPoint();
            return;
        }
    }
//    mEarthManipulator->screenToWorld(x, height - y,mOsgRenderer, outWorldPoint);
}

osgEarth::GeoPoint MapController::screenToGeoPoint(float x, float y) const
{
    osg::Vec3d worldPoint;
    screenToWorld(x, y, worldPoint);
    osgEarth::GeoPoint geoPint;
    geoPint.fromWorld(getMapSRS(), worldPoint);
    return geoPint;
}

void MapController::worldToScreen(osg::Vec3d worldPoint, float &outX, float &outY) const
{
    float height = static_cast<float>(mOsgRenderer->getCamera()->getViewport()->height());
    float width = static_cast<float>(mOsgRenderer->getCamera()->getViewport()->width());

    const osg::Matrixd pMatrix = mOsgRenderer->getCamera()->getProjectionMatrix();
    const osg::Matrixd vMatrix = mOsgRenderer->getCamera()->getViewMatrix();
    osg::Vec3f result =   (worldPoint * vMatrix) * pMatrix;
    outX = result.x() * (width/2.0f) + width/2.0f;
    outY = result.y() * (height/2.0f) + height/2.0f;
    outY = height - outY;

}

bool MapController::addNodeToLayer(osg::Node *node, std::string layerName)
{
    auto layer = getMapNode()->getMap()->getLayerByName(layerName);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
    return true;
}

bool MapController::removeNodeFromLayer(osg::Node *node, std::string layerName)
{
    auto layer = getMapNode()->getMap()->getLayerByName(layerName);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
    return true;
}

void MapController::zoom(double val)
{
    getEarthManipulator()->zoom(0, -val, getViewer());
}

void MapController::goToHome()
{
    getEarthManipulator()->home(0);
    auto camSet = getEarthManipulator()->getSettings();
    camSet->setMinMaxDistance(0,MAX_CAM_DISTANCE);
    getEarthManipulator()->applySettings(camSet);
}

void MapController::goToPosition(double latitude, double longitude, double range)
{
    osgEarth::GeoPoint  geoPoint(getMapSRS()->getGeographicSRS(), latitude, longitude, 0);
    geoPoint.transformInPlace(getMapSRS());

    osgEarth::Viewpoint vp;
    vp.focalPoint() = geoPoint;
    vp.range()= range;
    setViewpoint(vp, 3.0);
}

void MapController::goToPosition(osgEarth::GeoPoint mapPoint, double range, double duration)
{
    if(mapPoint.isRelative())
        mapPoint.makeAbsolute(mMapNode->getTerrain());
    osgEarth::Viewpoint vp;
    vp.focalPoint() = mapPoint;
    vp.range()= range;
    setViewpoint(vp, duration);

    //    auto minDistance = range/2;
    //    getEarthManipulator()->setViewpoint(vp);
    //    auto camSet = getEarthManipulator()->getSettings();
    //    camSet->setMinMaxDistance(minDistance,MAX_CAM_DISTANCE);
    //    getEarthManipulator()->applySettings(camSet);
}

void MapController::setMode(bool is3DView)
{
    mIs3DView = is3DView;
    auto  settings = mEarthManipulator->getSettings();
    if(is3DView) {
        settings->setMinMaxPitch(-90, 0);
    }
    else
    {
        settings->setMinMaxPitch(-90, -90);
        mEarthManipulator->setRotation(osg::Quat());
    }
    emit modeChanged(is3DView);
}

bool MapController::getMode() const
{
    return mIs3DView;
}

void MapController::setGeocentric(bool isGeocentric)
{
    if (mIsGeocentric == isGeocentric)
        return;

    mIsGeocentric = isGeocentric;
    osgEarth::LayerVector layers;
    mMapNode->getMap()->getLayers(layers);

    createMapNode(mIsGeocentric);
    emit mapCleared();
    for(auto layer: layers)
        addLayer(layer);

    osgEarth::Viewpoint vp = getEarthManipulator()->getViewpoint();

    createCameraManipulator();
    getViewer()->setCameraManipulator(getEarthManipulator());
    getEarthManipulator()->setViewpoint(vp);
}

void MapController::toggle3DView()
{
    //    setGeocentric(!mGeocentric);
    setMode(!mIs3DView);
}

void MapController::pan(double xVal, double yVal)
{
    getEarthManipulator()->pan(xVal, yVal);
}

void MapController::rotate(double xVal, double yVal)
{
    getEarthManipulator()->rotate(xVal, yVal);
}

void MapController::travelToViewpoint(qreal latitude,
                                      qreal longitude,
                                      qreal range,
                                      qreal pitch,
                                      qreal heading)
{

    osgEarth::GeoPoint  pt(getMapSRS()->getGeographicSRS(), latitude, longitude, 0);
    pt.transformInPlace(getMapSRS());
    osgEarth::Viewpoint vp;
    vp.focalPoint() = pt;
    vp.range() = range;
    vp.pitch() = pitch;
    vp.heading() = heading;
    mEarthManipulator->setViewpoint(vp, 3.0);
}

void MapController::orientCameraToNorth()
{
    osgEarth::Viewpoint vp = mEarthManipulator->getViewpoint();
    vp.heading() = 0;
    mEarthManipulator->setViewpoint(vp, 0.3);
}

void MapController::mapMouseLocation(osgEarth::GeoPoint geoPos)
{
    //    osgEarth::GeoPoint geoPos;
    //    geoPos.fromWorld(getMapSRS(), worldPos);
    //    osgEarth::GeoPoint  geographicPos = geoPos.transform(getMapSRS()->getGeocentricSRS()->getGeographicSRS());

    emit mousePointingLocationChanged(QVector3D(
                                          static_cast<float>(geoPos.x()),
                                          static_cast<float>(geoPos.y()),
                                          static_cast<float>(geoPos.z())));

    geoPos.makeGeographic();
    emit mousePointingLocationWgs84Changed(QVector3D(
                                               static_cast<float>(geoPos.x()),
                                               static_cast<float>(geoPos.y()),
                                               static_cast<float>(geoPos.z())));
}

void MapController::initializeOsgEarth()
{
    mMapRoot = new osg::Group();
    osgEarth::Util::SkyOptions sopts;
    mSkyNode = osgEarth::Util::SkyNode::create(sopts);
    createMapNode(true);

    mOsgRenderer->setSceneData(mMapRoot);

    osgEarth::Drivers::GDALOptions gdal;
    gdal.url() = (QString(EXTERNAL_RESOURCE_DIR) + QString("/world.tif")).toStdString();
    osg::ref_ptr<osgEarth::ImageLayer> imlayer = new osgEarth::ImageLayer("base-world", gdal);
    addLayer(imlayer);
    //create camera after create map node
    createCameraManipulator();
    mOsgRenderer->setCameraManipulator(mEarthManipulator);
}

void MapController::createMapNode(bool geocentric, osgEarth::Map *map)
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
            mMapNode = new osgEarth::MapNode(new osgEarth::Map(mapOpt));
        }
        else
        {
            mapOpt.coordSysType() = osgEarth::MapOptions::CSTYPE_PROJECTED;
            mapOpt.profile() = osgEarth::ProfileOptions();
            mMapNode = new osgEarth::MapNode(new osgEarth::Map(mapOpt));
        }
    }
    else
        mMapNode = new osgEarth::MapNode(map);

    mSkyNode->addChild(mMapNode);
    mMapRoot->addChild(mSkyNode);
    //mMapRoot->addChild(mMapNode);

    mMapNode->getMap()->addMapCallback(new MainMapCallback(this));
}

void MapController::createCameraManipulator()
{
    mEarthManipulator = new osgEarth::Util::EarthManipulator;
    auto  settings = mEarthManipulator->getSettings();
    //    settings->setSingleAxisRotation(true);

    settings->setMinMaxDistance(0.0, MAX_CAM_DISTANCE);
    if(mIs3DView)
        settings->setMinMaxPitch(-90, 0);
    else
        settings->setMinMaxPitch(-90, -90);
    //    settings->setMaxOffset(5000.0, 5000.0);
    //    settings->setMinMaxPitch(-90, 90);
    //    settings->setTerrainAvoidanceEnabled(true);
    //    settings->setThrowingEnabled(false);

    //    // set home to tehran
    osgEarth::GeoPoint  geoPoint(getMapSRS()->getGeographicSRS(), 51.3347, 35.7219,0);
    geoPoint.transformInPlace(getMapSRS());
    osgEarth::Viewpoint vp;
    vp.focalPoint() = geoPoint;
    vp.range()= 5000000;
    vp.pitch() = -90;
    vp.heading() = 0;
    getEarthManipulator()->setHomeViewpoint(vp, 0);

    settings->setMinMaxPitch(-90, 0);
}

void MapController::layerAdded(osgEarth::Layer */*layer*/, unsigned /*index*/)
{
    emit layerChanged();
}

void MapController::layerRemoved(osgEarth::Layer */*layer*/, unsigned /*index*/)
{
    emit layerChanged();
}

void MapController::frame()
{
    emit headingAngleChanged(-getViewpoint().getHeading());

    const auto vp = mEarthManipulator->getViewpoint();
    const auto fp = vp.focalPoint();
    if (fp.isSet()) {
        const osgEarth::GeoPoint mapPoint = fp.get();
        //        mapPoint.makeGeographic();
        osgEarth::GeoPoint  pointLatLong;
        mapPoint.transform(getMapSRS()->getGeographicSRS(), pointLatLong);


        emit focalPointLatChanged(pointLatLong.x());
        emit focalPointLongChanged(pointLatLong.y());
        emit focalPointRangeChanged(vp.range().get().getValue());
        emit focalPointPitchChanged(vp.pitch().get().getValue());
        emit focalPointHeadChanged(vp.heading().get().getValue());

    }




    //
    // Set SkyNode sun light rotate with camera
    //
    auto sunLight = mSkyNode->getSunLight();
    sunLight->setAmbient(osg::Vec4(0.01f, 0.01f, 0.01f, 1));
    sunLight->setDiffuse(osg::Vec4(0.5f, 0.5f, 0.5f, 1));
    sunLight->setSpecular(osg::Vec4(0.5f, 0.5f, 0.5f, 1));

    float splen = sunLight->getPosition().length();
    osg::Vec3 spos = mEarthManipulator->getMatrix().getTrans();
    spos.normalize();
    spos *= splen;

    mSkyNode->setSunVisible(false);
    mSkyNode->setAtmosphereVisible(false);
    sunLight->setPosition(osg::Vec4(spos, 0));
    spos.normalize();
    spos *= -1.0f;
    sunLight->setDirection(spos);


}

//--renderer---------------------------------------------------------------------------------------------------
void MapController::cleanup()
{
    if (mOsgRenderer) {
        mOsgRenderer->deleteLater();
        mOsgRenderer = nullptr;
    }
}

void MapController::initializeGL(int width, int height, QScreen *screen, GLuint renderTargetId)
{
    mRenderTargetId = renderTargetId;

    createOsgRenderer(width, height, screen);

    if(mWindow) {
        QObject::connect(mOsgRenderer, &OSGRenderer::needsUpdate,
                         mWindow, &QQuickWindow::update,
                         Qt::DirectConnection);
    }
}

void MapController::resizeGL(int width, int height, QScreen *screen)
{
    if (mOsgRenderer) {
        const float pixelRatio = static_cast<float>(screen->devicePixelRatio());
        mOsgRenderer->resize(0, 0, width, height, pixelRatio);
    }
}

void MapController::paintGL()
{
    if (mOsgRenderer) {
        if (mIsFirstFrame) {
            mIsFirstFrame = false;
            mOsgRenderer->getCamera()->getGraphicsContext()->setDefaultFboId(mRenderTargetId);
        }
        frame();
        mOsgRenderer->frame();
    }
}

void MapController::createOsgRenderer(int width, int height, QScreen *screen)
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    ds->setNvOptimusEnablement(1);
    ds->setStereo(false);


    mOsgRenderer = new OSGRenderer(this);
    const float pixelRatio = static_cast<float>(screen->devicePixelRatio());
    mOsgRenderer->setupOSG(0 , 0, width, height, pixelRatio);

    mOsgRenderer->getCamera()->setClearColor(osg::Vec4(0.15f, 0.15f, 0.15f, 1.0f));

}

void MapController::keyPressEvent(QKeyEvent *event)
{
    if (mOsgRenderer)
        mOsgRenderer->keyPressEvent(event);
}

void MapController::keyReleaseEvent(QKeyEvent *event)
{
    if (mOsgRenderer)
        mOsgRenderer->keyReleaseEvent(event);
}

void MapController::mousePressEvent(QMouseEvent *event)
{
    if (mOsgRenderer) {
        mOsgRenderer->mousePressEvent(event);
    }
}

void MapController::mouseReleaseEvent(QMouseEvent *event)
{
    if (mOsgRenderer) {
        mOsgRenderer->mouseReleaseEvent(event);
    }
}

void MapController::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (mOsgRenderer)
        mOsgRenderer->mouseDoubleClickEvent(event);
}

void MapController::mouseMoveEvent(QMouseEvent *event)
{
    if (mOsgRenderer) {
        mOsgRenderer->mouseMoveEvent(event);
    }
    osgEarth::GeoPoint geoPoint = screenToGeoPoint(event->x(), event->y());
    mapMouseLocation(geoPoint);
}

void MapController::wheelEvent(QWheelEvent *event)
{
    if (mOsgRenderer)
        mOsgRenderer->wheelEvent(event);
}


