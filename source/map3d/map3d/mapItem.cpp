
#include <iostream>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgEarthUtil/Sky>
#include <osgEarth/Registry>
#include <QMessageBox>
#include <QTimer>
#include <deque>
#include <chrono>
#include <QQuickOpenGLUtils>
#include <osgGA/TrackballManipulator>
#include "mapItem.h"

class MainMapCallback : public osgEarth::MapCallback
{
public:
    MainMapCallback(MapItem *MapItem) :
        mMapItem(MapItem)
    {

    }

    void onLayerAdded  (osgEarth::Layer* layer, unsigned index) override
    {
        if(mMapItem)
            mMapItem->layerAdded(layer, index);
    }
    void onLayerRemoved(osgEarth::Layer* layer, unsigned index) override
    {
        if(mMapItem)
            mMapItem->layerRemoved(layer, index);
    }

private:
    MapItem *mMapItem{nullptr};
};

//--MapItem---------------------------------------------------------------------------------------------------------
const double MAX_CAM_DISTANCE = 30000000.0;
MapItem::MapItem(QQuickItem *parent) :
    QQuickItem(parent)
{
    setAcceptHoverEvents(true);
    setFlag(ItemHasContents);
    setAcceptedMouseButtons(Qt::MouseButton::AllButtons);
    mOSGRenderNode = new OSGRenderNode(this);
    mOSGRenderNode->getCamera()->setClearColor(osg::Vec4(0.15f, 0.15f, 0.15f, 1.0f));
    //    createOsgRenderer();

    initializeOsgEarth();
}

MapItem::~MapItem()
{

}

void MapItem::setMap(osgEarth::Map *map)
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

osgViewer::Viewer *MapItem::getViewer() const
{
    return dynamic_cast<osgViewer::Viewer*>(mOSGRenderNode);
}

const osg::Group *MapItem::getRoot() const
{
    return mMapRoot;
}

osgEarth::MapNode *MapItem::getMapNode() const
{
    return mMapNode;
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

osgEarth::Util::EarthManipulator *MapItem::getEarthManipulator() const
{
    return mEarthManipulator;
}

void MapItem::setViewpoint(const osgEarth::Viewpoint &vp, double duration_s)
{
    getEarthManipulator()->setViewpoint(vp,duration_s);
}

osgEarth::Viewpoint MapItem::getViewpoint() const
{
    if (mEarthManipulator)
        return mEarthManipulator->getViewpoint();
    return osgEarth::Viewpoint();
}

void MapItem::setTrackNode(osg::Node *node, double minDistance)
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

//void MapController::untrackNode(osg::Node *node)
//{
//    auto vp = getEarthManipulator()->getViewpoint();
//    if(vp.getNode() == nullptr)
//        return;
//    if(vp.getNode() != node)
//        return;
//    vp.setNode(nullptr);
//    getEarthManipulator()->setViewpoint(vp);
//    auto camSet = getEarthManipulator()->getSettings();
//    camSet->setMinMaxDistance(0,MAX_CAM_DISTANCE);
//	getEarthManipulator()->applySettings(camSet);
//}

void MapItem::untrack()
{
    auto vp = getEarthManipulator()->getViewpoint();
    //    if(vp.getNode() == nullptr)
    //        return;
    vp.setNode(nullptr);
    getEarthManipulator()->setViewpoint(vp);

    auto camSet = getEarthManipulator()->getSettings();
    camSet->setMinMaxDistance(0,MAX_CAM_DISTANCE);
    getEarthManipulator()->applySettings(camSet);

}

qreal MapItem::headingAngle() const
{
    return -getViewpoint().getHeading();
}

void MapItem::screenToWorld(float x, float y, osg::Vec3d &outWorldPoint) const
{
    //    float height = static_cast<float>(mOSGRenderNode->getCamera()->getViewport()->height());
    float height = 0;
    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (mOSGRenderNode->computeIntersections(x, /*height - */y, intersections))
    {
        for (const auto &intersection : intersections)
        {
            //            mCurrentLocalPos    = intersection.getLocalIntersectPoint();
            outWorldPoint = intersection.getWorldIntersectPoint();
            return;
        }
    }
    else
        mEarthManipulator->screenToWorld(x, height - y,mOSGRenderNode, outWorldPoint);
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
    float height = static_cast<float>(mOSGRenderNode->getCamera()->getViewport()->height());
    float width = static_cast<float>(mOSGRenderNode->getCamera()->getViewport()->width());

    const osg::Matrixd pMatrix = mOSGRenderNode->getCamera()->getProjectionMatrix();
    const osg::Matrixd vMatrix = mOSGRenderNode->getCamera()->getViewMatrix();
    osg::Vec3f result =   (worldPoint * vMatrix) * pMatrix;
    outX = result.x() * (width/2.0f) + width/2.0f;
    outY = result.y() * (height/2.0f) + height/2.0f;
    outY = height - outY;

}

void MapItem::addLayer(osgEarth::Layer *layer)
{
    layer->setUserValue("parent", false);
    auto node = layer->getNode();
    if(node)
        node->setName(layer->getName());
    //    else
    //        qDebug()<<layer->getName();
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

void MapItem::removeLayer(osgEarth::Layer *layer)
{
    mMapNode->getMap()->removeLayer(layer);
}

bool MapItem::addNodeToLayer(osg::Node *node, std::string layerName)
{
    auto layer = getMapNode()->getMap()->getLayerByName(layerName);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
            emit layerChanged();
            return true;
        }
    }

    return false;
}

bool MapItem::removeNodeFromLayer(osg::Node *node, std::string layerName)
{
    auto layer = getMapNode()->getMap()->getLayerByName(layerName);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);

            emit layerChanged();
            return true;
        }
    }

    return false;
}

bool MapItem::addLayerToLayer(osgEarth::Layer *layer, std::string layerName)
{
    auto destinationLayer = getMapNode()->getMap()->getLayerByName(layerName);
    //    auto annotationLayer = dynamic_cast<osgEarth::Annotation::AnnotationLayer*>(layer);
    if(destinationLayer){
        auto dataContainer = destinationLayer->getOrCreateUserDataContainer();
        dataContainer->addUserObject(layer);
        //        osg::Group *group = dynamic_cast<osg::Group*>(destinationLayer->getNode());
        //        if(group){
        //            if(!group->containsNode(layer->getNode())){
        //group->addChild(layer->getNode());
        addLayer(layer);
        layer->setUserValue("parent", destinationLayer->getName());
        //                qDebug()<<"num parent"<<layer->getNode()->getNumParents();
        //                qDebug()<<"parent 0"<<layer->getNode()->getParent(0)->getName();
        //                qDebug()<<"parent 1"<<layer->getNode()->getParent(1)->getName();
        emit layerChanged();
        //            }
        return true;
    }
    return false;
}

bool MapItem::removeLayerFromLayer(osgEarth::Layer *layer, std::string layerName)
{
    auto destinationLayer = getMapNode()->getMap()->getLayerByName(layerName);
    //    auto annotationLayer = dynamic_cast<osgEarth::Annotation::AnnotationLayer*>(layer);
    if(destinationLayer){
        auto dataContainer = destinationLayer->getUserDataContainer();
        if(!dataContainer){
            unsigned int index = dataContainer->getUserObjectIndex(layer);
            dataContainer->removeUserObject(index);
            removeLayer(layer);
            emit layerChanged();
            return true;
        }

//        osg::Group *group = dynamic_cast<osg::Group*>(destinationLayer->getNode());
//        if(group){
//            group->removeChild(layer->getNode());
//            removeLayer(layer);
//            layer->setUserValue("parent", false);
//            emit layerChanged();
//            return true;
//        }
    }
    return false;
}

QSGNode *MapItem::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
    QSGRenderNode *n = static_cast<QSGRenderNode *>(node);
    //    qDebug() << "999999999999";
    QSGRendererInterface *ri = window()->rendererInterface();
    if (!ri)
        return nullptr;

    if (!n)
    {
        //        mOSGRenderNode = new OSGRenderNode(this);
        //        createOsgRenderer();
        ////        mOSGRenderNode->setupOSG(x(), y(), width(), height(), 1);

        ////        setNode(mSource);
        //        initializeOsgEarth();

        mOSGRenderNode->setupOSG(x(), y(), width(), height(), 1);

        n = mOSGRenderNode;
    }
    static_cast<OSGRenderNode *>(n)->sync(this);

    if (!n)
        qWarning("QSGRendererInterface reports unknown graphics API %d", ri->graphicsApi());

    return n;
}

void MapItem::zoom(double val)
{
    getEarthManipulator()->zoom(0, -val, getViewer());
}

void MapItem::goToHome()
{
    getEarthManipulator()->home(0);
    auto camSet = getEarthManipulator()->getSettings();
    camSet->setMinMaxDistance(0,MAX_CAM_DISTANCE);
    getEarthManipulator()->applySettings(camSet);
}

void MapItem::goToPosition(double latitude, double longitude, double range)
{
    osgEarth::GeoPoint  geoPoint(getMapSRS()->getGeographicSRS(), latitude, longitude, 0);
    geoPoint.transformInPlace(getMapSRS());

    osgEarth::Viewpoint vp;
    vp.focalPoint() = geoPoint;
    vp.range()= range;
    setViewpoint(vp, 3.0);
}

void MapItem::goToPosition(osgEarth::GeoPoint mapPoint, double range, double duration)
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

void MapItem::changeMode()
{
    mIs3DView = !mIs3DView;
    auto  settings = mEarthManipulator->getSettings();
    if(mIs3DView) {
        settings->setMinMaxPitch(-90, 0);
    }
    else
    {
        settings->setMinMaxPitch(-90, -90);
        mEarthManipulator->setRotation(osg::Quat());
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
        addLayer(layer);

    osgEarth::Viewpoint vp = getEarthManipulator()->getViewpoint();

    createCameraManipulator();
    getViewer()->setCameraManipulator(getEarthManipulator());
    getEarthManipulator()->setViewpoint(vp);
}

//void MapItem::toggle3DView()
//{
//    //    setGeocentric(!mGeocentric);
//    setMode(!mIs3DView);
//}

void MapItem::pan(double xVal, double yVal)
{
    getEarthManipulator()->pan(xVal, yVal);
}

void MapItem::rotate(double xVal, double yVal)
{
    getEarthManipulator()->rotate(xVal, yVal);
}

void MapItem::travelToViewpoint(qreal latitude,
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

void MapItem::orientCameraToNorth()
{
    osgEarth::Viewpoint vp = mEarthManipulator->getViewpoint();
    vp.heading() = 0;
    mEarthManipulator->setViewpoint(vp, 0.3);
}

void MapItem::initializeOsgEarth()
{
    mMapRoot = new osg::Group();
    osgEarth::Util::SkyOptions sopts;
    mSkyNode = osgEarth::Util::SkyNode::create(sopts);
    createMapNode(true);

    mOSGRenderNode->setSceneData(mMapRoot);

    osgEarth::Drivers::GDALOptions gdal;
    gdal.maxDataLevelOverride() = 700000;
    gdal.interpolation() = osgEarth::ElevationInterpolation::INTERP_CUBICSPLINE;
    gdal.L2CacheSize() = 2048;
    gdal.url() = (QString(EXTERNAL_RESOURCE_DIR) + QString("/world.tif")).toStdString();
    osg::ref_ptr<osgEarth::ImageLayer> imlayer = new osgEarth::ImageLayer("base-world", gdal);
    addLayer(imlayer);
    //create camera after create map node
    createCameraManipulator();
    mOSGRenderNode->setCameraManipulator(mEarthManipulator);
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
    //    osg::Node* globe = osgDB::readNodeFile("/home/client112/Desktop/QTMAP3D/source/map3d/data/earth_files/geocentric.earth");
    //    mMapNode = osgEarth::MapNode::get( globe );
    mSkyNode->addChild(mMapNode);
    mMapRoot->addChild(mSkyNode);
    //    mMapRoot->addChild(mMapNode);

    mMapNode->getMap()->addMapCallback(new MainMapCallback(this));
}

void MapItem::createCameraManipulator()
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

void MapItem::layerAdded(osgEarth::Layer */*layer*/, unsigned /*index*/)
{
    emit layerChanged();
}

void MapItem::layerRemoved(osgEarth::Layer */*layer*/, unsigned /*index*/)
{
    emit layerChanged();
}

void MapItem::frame()
{
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
//void MapItem::cleanup()
//{
//    if (mOsgRenderer) {
//        mOsgRenderer->deleteLater();
//        mOsgRenderer = nullptr;
//    }
//}

//void MapItem::initializeGL(int width, int height, QScreen *screen, GLuint renderTargetId)
//{
//    mRenderTargetId = renderTargetId;

//    createOsgRenderer(width, height, screen);

//    if(mWindow) {
//        QObject::connect(mOsgRenderer, &OSGRenderer::needsUpdate,
//                         [this](){
//                             QQuickOpenGLUtils::resetOpenGLState();
//            qDebug() << "<<<<<<<<";
//                         });
//    }
//}

//void MapItem::resizeGL(int width, int height, QScreen *screen)
//{
//    if (mOsgRenderer) {
//        const float pixelRatio = static_cast<float>(screen->devicePixelRatio());
//        mOsgRenderer->resize(0, 0, width, height, pixelRatio);
//    }
//}

//void MapItem::paintGL()
//{
//    qDebug() << "------------------";
//    if (mOsgRenderer) {
//        if (mIsFirstFrame) {
//            mIsFirstFrame = false;
//            mOsgRenderer->getCamera()->getGraphicsContext()->setDefaultFboId(mRenderTargetId);
//        }
//        frame();
//        mOs->frame();
//    }
//}

//void MapItem::createOsgRenderer()
//{
////    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
////    ds->setNvOptimusEnablement(1);
////    ds->setStereo(false);


//    mOSGRenderNode = new OSGRenderNode(this);
////    mOSGRenderNode->setupOSG(x(), y(), width(), height(), 1);

//    mOSGRenderNode->getCamera()->setClearColor(osg::Vec4(0.15f, 0.15f, 0.15f, 1.0f));

//}

void MapItem::keyPressEvent(QKeyEvent *event)
{
    if (mOSGRenderNode)
        mOSGRenderNode->keyPressEvent(event);
}

void MapItem::keyReleaseEvent(QKeyEvent *event)
{
    if (mOSGRenderNode)
        mOSGRenderNode->keyReleaseEvent(event);
}

void MapItem::mousePressEvent(QMouseEvent *event)
{
    if (mOSGRenderNode) {
        mOSGRenderNode->mousePressEvent(event);
    }
}

void MapItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (mOSGRenderNode) {
        mOSGRenderNode->mouseReleaseEvent(event);
    }
}

void MapItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (mOSGRenderNode)
        mOSGRenderNode->mouseDoubleClickEvent(event);
}

void MapItem::mouseMoveEvent(QMouseEvent *event)
{
    if (mOSGRenderNode) {
        mOSGRenderNode->mouseMoveEvent(event);
    }
    mCurrentMouseGeoPoint = screenToGeoPoint(event->position().x(), event->position().y());
}

void MapItem::wheelEvent(QWheelEvent *event)
{
    if (mOSGRenderNode)
        mOSGRenderNode->wheelEvent(event);
}

void MapItem::hoverMoveEvent(QHoverEvent *event)
{
    if (mOSGRenderNode) {
        mOSGRenderNode->hoverMoveEvent(event);
    }
    mCurrentMouseGeoPoint = screenToGeoPoint(event->position().x(), event->position().y());
}
