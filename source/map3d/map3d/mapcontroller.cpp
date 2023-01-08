
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

//class MainEventHandler : public osgGA::GUIEventHandler
//{
//public:
//    MainEventHandler(MapController *pMapController);
//    virtual ~MainEventHandler() override { }

//protected:
//    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;

//private:
//    void mouseEvent(osgViewer::View *view, const osgGA::GUIEventAdapter &ea, QEvent::Type qEventType);

//private:
//    MapController *mpMapController = nullptr;
//};


//bool MainEventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//{

//    osgViewer::View *view = dynamic_cast<osgViewer::View *>(&aa);

//    QEvent::Type qEventType;
//    switch (ea.getEventType())
//    {
//    case osgGA::GUIEventAdapter::FRAME:
//        mpMapController->frame();
//        break;
//    case (osgGA::GUIEventAdapter::PUSH):
//        qEventType = QEvent::Type::MouseButtonPress;
//        if (view) { mouseEvent(view, ea, qEventType); }
//        break;
//    case (osgGA::GUIEventAdapter::RELEASE):
//        qEventType = QEvent::Type::MouseButtonRelease;
//        if (view) { mouseEvent(view, ea, qEventType); }
//        break;
//    case (osgGA::GUIEventAdapter::MOVE):
//        qEventType = QEvent::Type::MouseMove;
//        if (view) { mouseEvent(view, ea, qEventType); }
//        break;
//    case (osgGA::GUIEventAdapter::SCROLL):
//        qEventType = QEvent::Type::Wheel;
//        if (view) { mouseEvent(view, ea, qEventType); }
//        break;
//    case (osgGA::GUIEventAdapter::DOUBLECLICK):
//        qEventType = QEvent::Type::MouseButtonDblClick;
//        if (view) { mouseEvent(view, ea, qEventType); }
//        break;
//    default:
//        break;
//    }
//    return false;

//}

//void MainEventHandler::mouseEvent(osgViewer::View *view, const osgGA::GUIEventAdapter &ea, QEvent::Type qEventType)
//{

//    QMouseEvent* event;
//    Qt::MouseButton mb;
//    switch (ea.getButtonMask())
//    {
//    case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
//        mb = Qt::MouseButton::LeftButton;
//        break;
//    case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
//        mb = Qt::MouseButton::RightButton;
//        break;
//    case osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
//        mb = Qt::MouseButton::MiddleButton;
//        break;
//    default:
//        mb = Qt::MouseButton::NoButton;
//    }

//    event = new QMouseEvent(qEventType,
//                            QPointF(static_cast<qreal>(ea.getX()),static_cast<qreal>(ea.getY())),
//                            mb, mb, Qt::KeyboardModifier::NoModifier);

//    osgUtil::LineSegmentIntersector::Intersections intersections;
//    if (view->computeIntersections(ea, intersections))
//    {
//        for (const auto &intersection : intersections)
//        {
//            //            mCurrentLocalPos    = intersection.getLocalIntersectPoint();
//            osg::Vec3d currentWorldPos = intersection.getWorldIntersectPoint();
//            mpMapController->mapMouseEvent(event,currentWorldPos);
//            return;
//        }
//    }
//}

//MainEventHandler::MainEventHandler(MapController *pMapController) :
//    mpMapController(pMapController)
//{

//}
//--MapController---------------------------------------------------------------------------------------------------------

MapController::MapController(QQuickWindow *window) :
    mWindow(window)
{
}

MapController::~MapController()
{

}

void MapController::installEventHandler()
{
    //getViewer()->addEventHandler(new MainEventHandler(this));
}

void MapController::mapMouseEvent(QMouseEvent *event, const osg::Vec3d &worldPos)
{
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(getMapSRS(), worldPos);
    osgEarth::GeoPoint  geographicPos = geoPos.transform(getMapSRS()->getGeocentricSRS()->getGeographicSRS());

    emit mousePointingLocationChanged(QVector3D(
                                         static_cast<float>(geoPos.x()),
                                         static_cast<float>(geoPos.y()),
                                         static_cast<float>(geoPos.z())));
    emit mousePointingLocationWgs84Changed(QVector3D(
                                         static_cast<float>(geographicPos.x()),
                                         static_cast<float>(geographicPos.y()),
                                         static_cast<float>(geographicPos.z())));

    emit mouseEvent(event, geoPos, worldPos);
}

osgViewer::Viewer *MapController::getViewer()
{
    return dynamic_cast<osgViewer::Viewer*>(mOsgRenderer);
}

osgEarth::Util::EarthManipulator *MapController::getEarthManipulator()
{
    return mEarthManipulator;
}

osgEarth::Viewpoint MapController::getViewpoint() const
{
    if (mEarthManipulator)
        return mEarthManipulator->getViewpoint();
    return osgEarth::Viewpoint();
}

osgEarth::MapNode *MapController::getMapNode()
{
    return mMapNode;
}
osg::ref_ptr<osg::Group> MapController::getRoot() const
{
    return mMapRoot;
}

const osgEarth::SpatialReference *MapController::getMapSRS() const
{
    return mMapNode->getMapSRS();
}

void MapController::setMap(osgEarth::Map *map)
{
    setGeocentric(map->isGeocentric());

    mMapNode->getMap()->clear();
    mMapNode->getMap()->setLayersFromMap(map);

    goToHome();
}

void MapController::setTrackNode(osg::Node *node)
{
    auto vp = getEarthManipulator()->getViewpoint();
    if(vp.getNode() == node)
        return;

    vp.setNode(node);
    getEarthManipulator()->setViewpoint(vp);
    auto camSet = getEarthManipulator()->getSettings();
    camSet->setTetherMode(osgEarth::Util::EarthManipulator::TetherMode::TETHER_CENTER);
    getEarthManipulator()->applySettings(camSet);
}

void MapController::untrackNode()
{
    auto vp = getEarthManipulator()->getViewpoint();
    if(vp.getNode() == nullptr)
        return;
    vp.setNode(nullptr);
    getEarthManipulator()->setViewpoint(vp);
}

bool MapController::addNode(osg::Node *node)
{
    osgEarth::Registry::shaderGenerator().run(node);// for textures or lighting
    return mMapNode->addChild(node);
}

bool MapController::removeNode(osg::Node *node)
{
    return mMapNode->removeChild(node);
}

void MapController::setViewpoint(const osgEarth::Viewpoint &vp, double duration_s)
{
    getEarthManipulator()->setViewpoint(vp,duration_s);
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

void MapController::zoom(double val)
{
    getEarthManipulator()->zoom(0, -val, getViewer());
}

void MapController::goToHome()
{
    getEarthManipulator()->home(0);
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

void MapController::goToPosition(osgEarth::GeoPoint mapPoint, double range)
{
    if(mapPoint.isRelative())
        mapPoint.makeAbsolute(mMapNode->getTerrain());
    osgEarth::Viewpoint vp;
    vp.focalPoint() = mapPoint;
    vp.range()= range;
    setViewpoint(vp, 3.0);
}

void MapController::setMode(bool is3DView)
{
    mIs3DView = is3DView;
    auto  settings = mEarthManipulator->getSettings();
    if(is3DView)
        settings->setMinMaxPitch(-90, 0);
    else
    {
        settings->setMinMaxPitch(-90, -90);
        mEarthManipulator->setRotation(osg::Quat());
    }
    emit modeChanged(is3DView);
}

bool MapController::getMode()
{
    return mIs3DView;
}

void MapController::setGeocentric(bool geocentric)
{
    if (mGeocentric == geocentric)
        return;

    mGeocentric = geocentric;

    osgEarth::LayerVector layers;
    mMapNode->getMap()->getLayers(layers);

    createMapNode(geocentric);
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


    static std::deque<std::chrono::high_resolution_clock::time_point> timepoint_list;
    const auto now_timepoint = std::chrono::high_resolution_clock::now();

    timepoint_list.push_back(now_timepoint);
    while (timepoint_list.size() > 2) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>
                (now_timepoint - timepoint_list.front()).count() > 2000) {
            timepoint_list.pop_front();
        } else {
            break;
        }
    }

    if (timepoint_list.size() >= 2) {
        const qreal duration = qreal(std::chrono::duration_cast<std::chrono::milliseconds>(timepoint_list.back() - timepoint_list.front()).count());
        if (duration > 0) {
            const qreal fps = qreal((timepoint_list.size()-1) * 1000) / duration;
            emit fpsChanged(fps);
        }
    }

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

void MapController::screenToWorld(float x, float y, osg::Vec3d &outWorldPoint) const
{
    float height = static_cast<float>(mOsgRenderer->getCamera()->getViewport()->height());
//    osgUtil::LineSegmentIntersector::Intersections intersections;
//    if (mOsgRenderer->computeIntersections(x, height - y, intersections))
//    {
//        for (const auto &intersection : intersections)
//        {
//            //            mCurrentLocalPos    = intersection.getLocalIntersectPoint();
//            out_coords = intersection.getWorldIntersectPoint();
//            return;
//        }
//    }
    mEarthManipulator->screenToWorld(x, height - y,mOsgRenderer, outWorldPoint);
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

    osg::Vec3d worldPos;
    screenToWorld(event->x(), event->y(), worldPos);
//    osgEarth::GeoPoint geoPos;
//    geoPos.fromWorld(getMapSRS(), worldPos);
//    qDebug()<<"mpos:"<<QString::fromStdString(geoPos.toString());
//    qDebug()<<event->x()<<", "<<event->y();
//    float x;
//    float y;
//    worldToScreen(worldPos, x, y);
//    qDebug()<<x<<", "<<y;
    mapMouseEvent(event, worldPos);
}

void MapController::mouseReleaseEvent(QMouseEvent *event)
{

    if (mOsgRenderer) {
        mOsgRenderer->mouseReleaseEvent(event);
    }
    osg::Vec3d worldPos;
    screenToWorld(event->x(),  event->y(), worldPos);
    mapMouseEvent(event, worldPos);
}

void MapController::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (mOsgRenderer)
        mOsgRenderer->mouseDoubleClickEvent(event);
    osg::Vec3d worldPos;
    screenToWorld(event->x(),  event->y(), worldPos);
    mapMouseEvent(event, worldPos);

}

void MapController::mouseMoveEvent(QMouseEvent *event)
{

    if (mOsgRenderer) {
        mOsgRenderer->mouseMoveEvent(event);
    }
    osg::Vec3d worldPos;
    screenToWorld(event->x(),  event->y(), worldPos);
    mapMouseEvent(event, worldPos);
}

void MapController::wheelEvent(QWheelEvent *event)
{

    if (mOsgRenderer)
        mOsgRenderer->wheelEvent(event);
//    osg::Vec3d worldPos;
//    mEarthManipulator->screenToWorld(event->x(),  event->y(),mOsgRenderer, worldPos);
//    mapMouseEvent(event, worldPos);
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

void MapController::initializeOsgEarth()
{
    installEventHandler();

    mMapRoot = new osg::Group();
    mSkyNode = osgEarth::Util::SkyNode::create(mMapNode);
    createMapNode(true);

    mOsgRenderer->setSceneData(mMapRoot);

    osgEarth::Drivers::GDALOptions gdal;
    gdal.url() = (QString(EXTERNAL_RESOURCE_DIR) + QString("/world.tif")).toStdString();
    osg::ref_ptr<osgEarth::ImageLayer> imlayer = new osgEarth::ImageLayer("base-world", gdal);
    mMapNode->getMap()->addLayer(imlayer);
    //create camera after create map node
    createCameraManipulator();
    mOsgRenderer->setCameraManipulator(mEarthManipulator);
}

void MapController::createMapNode(bool bGeocentric)
{
    mSkyNode->removeChild(mMapNode);
    mMapRoot->removeChild(mMapNode);
    mMapRoot->removeChild(mSkyNode);

    osgEarth::MapOptions mapOpt;
    if(bGeocentric)
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

    mSkyNode->addChild(mMapNode);
    mMapRoot->addChild(mSkyNode);
//    mMapRoot->addChild(mMapNode);
}

void MapController::createCameraManipulator()
{
    mEarthManipulator = new osgEarth::Util::EarthManipulator;
    auto  settings = mEarthManipulator->getSettings();
//    settings->setSingleAxisRotation(true);

    settings->setMinMaxDistance(0.0, 30000000.0);
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

}
