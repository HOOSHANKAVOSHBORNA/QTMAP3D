
#include "map3dwidget.h"

#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
//#include <osgEarth/Map>
//#include <osgEarth/MapNode>
#include <osgEarth/Registry>
//#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/LogarithmicDepthBuffer>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgEarthUtil/Sky>
#include <osg/Camera>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgEarth/Layer>
//#include <osgEarthAnnotation/TrackNode>
//#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>

#include <QDirIterator>
#include<QDebug>
#include <QDockWidget>
#include <QSlider>
#include <QHBoxLayout>
#include <exception>
#include <QObject>
#include <QMouseEvent>
#include <QApplication>
#include <QMessageBox>
//#include <qcoreevent.h>

using namespace osgEarth;
using namespace osgEarth::Drivers;

const double ZOOM_STEP{0.2};
const double UP_DOWN_STEP{0.1};
const double LEFT_RIGHT_STEP{0.1};
const double HEAD_STEP{osg::DegreesToRadians(5.0)};
const double PITCH_STEP{osg::DegreesToRadians(2.0)};

const double MIN_DISTANCE{10.0};
const double MAX_DISTANCE{1000000000.0};
const double MAX_OFSET{5000.0};

const double DURATION{3};


MousePicker::MousePicker(Map3dWidget *map3dWidget)
{
    mMap3dWidget = map3dWidget;
}

bool MousePicker::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{

    osgViewer::View *view = dynamic_cast<osgViewer::View *>(&aa);

    QEvent::Type qEventType;
    switch (ea.getEventType())
    {
    case osgGA::GUIEventAdapter::FRAME:
        mMap3dWidget->frame();
        break;
    case (osgGA::GUIEventAdapter::PUSH):
        qEventType = QEvent::Type::MouseButtonPress;
        if (view) { mouseEvent(view, ea, qEventType); }
        break;
    case (osgGA::GUIEventAdapter::RELEASE):
        qEventType = QEvent::Type::MouseButtonRelease;
        if (view) { mouseEvent(view, ea, qEventType); }
        break;
    case (osgGA::GUIEventAdapter::MOVE):
        qEventType = QEvent::Type::MouseMove;
        if (view) { mouseEvent(view, ea, qEventType); }
        break;
    case (osgGA::GUIEventAdapter::SCROLL):
        qEventType = QEvent::Type::Wheel;
        if (view) { mouseEvent(view, ea, qEventType); }
        break;
    case (osgGA::GUIEventAdapter::DOUBLECLICK):
        qEventType = QEvent::Type::MouseButtonDblClick;
        if (view) { mouseEvent(view, ea, qEventType); }
        break;
    default:
        break;
    }
    return false;
}

void MousePicker::mouseEvent(osgViewer::View *view, const osgGA::GUIEventAdapter &ea, QEvent::Type qEventType)
{
    QMouseEvent* event;
    Qt::MouseButton mb;
    switch (ea.getButtonMask())
    {
    case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
        mb = Qt::MouseButton::LeftButton;
        break;
    case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
        mb = Qt::MouseButton::RightButton;
        break;
    case osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
        mb = Qt::MouseButton::MiddleButton;
        break;
    default:
        mb = Qt::MouseButton::NoButton;
    }

    event = new QMouseEvent(qEventType,
                            QPointF(static_cast<qreal>(ea.getX()),static_cast<qreal>(ea.getY())),
                            mb, mb, Qt::KeyboardModifier::NoModifier);

    osgUtil::LineSegmentIntersector::Intersections intersections;
    if (view->computeIntersections(ea, intersections))
    {
        for (const auto &intersection : intersections)
        {
            //            mCurrentLocalPos    = intersection.getLocalIntersectPoint();
            osg::Vec3d currentWorldPos = intersection.getWorldIntersectPoint();
            mMap3dWidget->mapMouseEvent(event,currentWorldPos);
            return;
        }
    }
}


Map3dWidget::Map3dWidget(bool isGeocentric, QWidget *parent)
    : osgQOpenGLWidget(parent)
    , mSRSwgs84(osgEarth::SpatialReference::get("wgs84"))
{
    mIsGeocentric = isGeocentric;
    //    mLayout = new QHBoxLayout(this);
    //    mLayout->setMargin(0);
    //    mMapOpenGLWidget = new osgQOpenGLWidget(this);
    setMouseTracking(true);

    //    mLayout->addWidget(mMapOpenGLWidget);
    createWidgets();
    // init signal handle-------------------------------------------------------
    QObject::connect(this, &osgQOpenGLWidget::initialized, [=]{
        //create camera ----------------------------------------------
        createManipulator();
        getOsgViewer()->setCameraManipulator(mEarthManipulator);
        //add mouse event handler
        auto mousePicker = new MousePicker(this);
        getOsgViewer()->addEventHandler(mousePicker);
        //create map -----------------------------------------------
        mMapRoot = new osg::Group();
        mSkyNode = osgEarth::Util::SkyNode::create();
        createMapNode(isGeocentric);

        getOsgViewer()->setSceneData(mMapRoot);
        mHomeViewpoint = mEarthManipulator->getViewpoint();

        mCmWidget->setStateMap(isGeocentric);
        //--------------------------------------------------------------
        GDALOptions gdal;
        gdal.url() = "../map3dlib/data/earth_files/world.tif";
        osg::ref_ptr<ImageLayer> imlayer = new ImageLayer("base-world", gdal);
        addLayer(imlayer);

    });


    //setMouseTracking(true);
}

void Map3dWidget::setMap(Map *map)
{
    typeChanged(map->isGeocentric());

    mMapNode->getMap()->clear();
    mMapNode->getMap()->setLayersFromMap(map);

    mCmWidget->setStateMap(map->isGeocentric());

    home();
}

void Map3dWidget::setTrackNode(osg::Node *value)
{
    auto vp = mEarthManipulator->getViewpoint();
    if(vp.getNode() == value)
        return;

    vp.setNode(value);//to track
    //vp.setRange(100);
    mEarthManipulator->setViewpoint(vp);
    auto camSet = mEarthManipulator->getSettings();
    camSet->setTetherMode(osgEarth::Util::EarthManipulator::TetherMode::TETHER_CENTER);
    //    camSet->getBreakTetherActions().push_back(osgEarth::Util::EarthManipulator::ACTION_GOTO );
    mEarthManipulator->applySettings(camSet);

}

void Map3dWidget::unTrackNode()
{
    auto vp = mEarthManipulator->getViewpoint();
    if(vp.getNode() == nullptr)
        return;
    vp.setNode(nullptr);//to track
    mEarthManipulator->setViewpoint(vp);
}

bool Map3dWidget::addNode(osg::Node *node)
{
    osgEarth::Registry::shaderGenerator().run(node);// for textures or lighting
    return mMapNode->addChild(node);
}

bool Map3dWidget::removeNode(osg::Node *node)
{
    return mMapNode->removeChild(node);
}

void Map3dWidget::setViewpoint(const Viewpoint &vp, double duration_s)
{
    mEarthManipulator->setViewpoint(vp,duration_s);
}

Viewpoint Map3dWidget::getViewpoint() const
{
    return mEarthManipulator->getViewpoint();
}

MapNode *Map3dWidget::getMapNode()
{
    return mMapNode;
}

void Map3dWidget::addLayer(osgEarth::Layer *layer)
{
    mMapNode->getMap()->addLayer(layer);
    // Check if the layer is added successfully
    auto added = mMapNode->getMap()->getLayerByName(layer->getName());
    if (added && added->getEnabled())
    {

    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Data loading failed!"));
    }
}

void Map3dWidget::createManipulator()
{
    mEarthManipulator = new osgEarth::Util::EarthManipulator;
    auto  settings = mEarthManipulator->getSettings();
    settings->setSingleAxisRotation(true);
    //qDebug()<<settings->getMaxDistance();
    settings->setMinMaxDistance(MIN_DISTANCE, MAX_DISTANCE);
    settings->setMaxOffset(MAX_OFSET, MAX_OFSET);
    settings->setMinMaxPitch(-90, 90);
    settings->setTerrainAvoidanceEnabled(true);
    settings->setThrowingEnabled(false);

}

void Map3dWidget::createWidgets()
{
    mCmWidget = new CameraManipulatorWidget(this);
    // setting CameraManipulatorWidget
    mCmWidget->setZoomStep(ZOOM_STEP);
    mCmWidget->setUpDownStep(UP_DOWN_STEP);
    mCmWidget->setLeftRightStep(LEFT_RIGHT_STEP);
    mCmWidget->setHeadStep(HEAD_STEP);
    mCmWidget->setPitchStep(PITCH_STEP);
    connect(mCmWidget, &CameraManipulatorWidget::homeClicked, this,  &Map3dWidget::home);
    connect(mCmWidget, &CameraManipulatorWidget::zoomChanged, this,&Map3dWidget::setZoom);
    connect(mCmWidget, &CameraManipulatorWidget::upDownChanged, [=](double val){mEarthManipulator->pan(0,val);} );
    connect(mCmWidget, &CameraManipulatorWidget::leftRightChanged, [=](double val){mEarthManipulator->pan(val,0);} );
    connect(mCmWidget, &CameraManipulatorWidget::headChanged, [=](double val){mEarthManipulator->rotate(val,0);} );
    connect(mCmWidget, &CameraManipulatorWidget::pitchChanged, [=](double val){mEarthManipulator->rotate(0,val);} );
    connect(mCmWidget, &CameraManipulatorWidget::mapChange,this, &Map3dWidget::typeChanged);
    //-------------------------------------
    mCompassWidget = new CompassWidget(this);
    //    connect(mCmWidget, &CameraManipulatorWidget::headChanged, [=](double val){
    //        double degri = osg::RadiansToDegrees(val);
    //        mCompassWidget->setRotate(-degri);
    //    } );
    //-------------------------------
    mLocationWidget = new LocationWidget(this);
    connect(mLocationWidget, &LocationWidget::goPosition,this, &Map3dWidget::goPosition);
    connect(mLocationWidget, &LocationWidget::saveLocation,this, &Map3dWidget::saveCurrentPosition);
    //connect(mLocationWidget, &LocationWidget::clickedPosition,this, &Map3dWidget::onClickedPosition);
    //mLocationWidget->addViewPoint( osgEarth::Viewpoint("hasan",23,444,555,66,77,88));
}

void Map3dWidget::setZoom(double val)
{
    mEarthManipulator->zoom(0, -val, getOsgViewer());
}

void Map3dWidget::home()
{
    mEarthManipulator->home(0);
    //        mEarthManipulator->setViewpoint(mHomeViewpoint);
    mCompassWidget->setPoint(0);
}

void Map3dWidget::typeChanged(bool isGeocentric)
{
    if(mIsGeocentric == isGeocentric)
        return;

    mIsGeocentric = isGeocentric;

    LayerVector layers;
    mMapNode->getMap()->getLayers(layers);

    createMapNode(isGeocentric);
    for(auto layer: layers)
       addLayer(layer);

    Viewpoint vp = mEarthManipulator->getViewpoint();
    //    qDebug()<<"vp.heading():"<<QString::fromUtf8(vp.heading()->asString().c_str());
    //    qDebug()<<"vp.pitch():"<<QString::fromUtf8(vp.pitch()->asString().c_str());
    //    qDebug()<<"vp.range():"<<QString::fromUtf8(vp.range()->asString().c_str());
    createManipulator();
    getOsgViewer()->setCameraManipulator(mEarthManipulator);
    mEarthManipulator->setViewpoint(vp);

    emit mapNodeChanged(mMapNode);
}

const SpatialReference *Map3dWidget::getMapSRS() const
{
    mMapNode->getMapSRS();
}

void Map3dWidget::goPosition(double latitude, double longitude, double range)
{
    osgEarth::GeoPoint  pointLatLong(mSRSwgs84, latitude, longitude, 0);
    osgEarth::GeoPoint  mapPoint;
    pointLatLong.transform(getMapSRS(), mapPoint);

    osgEarth::Viewpoint vp;
    vp.focalPoint() = mapPoint;
    vp.range()= range;
    //vp.heading()->set(50, osgEarth::Units::DEGREES);
    //vp.pitch()->set(-25, osgEarth::Units::DEGREES);
    setViewpoint(vp, DURATION);
}

void Map3dWidget::frame()
{
    this->mCompassWidget->setRotate(-this->getViewpoint().getHeading());
    auto vp = getViewpoint();
    mLocationWidget->setCurrentLocation(vp.focalPoint()->x(),vp.focalPoint()->y());
}

void Map3dWidget::mapMouseEvent(QMouseEvent *event, osg::Vec3d worldPos)
{
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(getMapSRS(), worldPos);

    osgEarth::GeoPoint  geographicPos = geoPos.transform(mSRSwgs84);

    mLocationWidget->setMousePosition(tr("Lat Lon: [%1, %2, %3]")
                                      .arg(geographicPos.x(), 0, 'f', 3)
                                      .arg(geographicPos.y(), 0, 'f', 3)
                                      .arg(geographicPos.z(), 0, 'f', 3));
    emit mouseEvent(event, geoPos);
}

void Map3dWidget::saveCurrentPosition(QString name)
{
    osgEarth::Viewpoint vp = getViewpoint();

    mLocationWidget->addLocation(name,vp.focalPoint()->x(),vp.focalPoint()->y() ,vp.getRange());
}

void Map3dWidget::createMapNode(bool isGeocentric)
{
    mSkyNode->removeChild(mMapNode);
    mMapRoot->removeChild(mSkyNode);
    mMapRoot->removeChild(mMapNode);

    MapOptions mapOpt;
    if(isGeocentric)
    {
        mapOpt.coordSysType() = MapOptions::CSTYPE_GEOCENTRIC;
        mapOpt.profile() = ProfileOptions("global-mercator");

        mMapNode = new MapNode(new Map(mapOpt));
    }
    else
    {
        mapOpt.coordSysType() = MapOptions::CSTYPE_PROJECTED;
        //        mapOptProj.profile() = ProfileOptions("plate-carre");
        mapOpt.profile() = ProfileOptions();
        mMapNode = new MapNode(new Map(mapOpt));
    }
    mSkyNode->addChild(mMapNode);
    mMapRoot->addChild(mSkyNode);
//    mMapRoot->addChild(mMapNode);

}

void Map3dWidget::resizeEvent(QResizeEvent* event)
{
    osgQOpenGLWidget::resizeEvent(event);
    if(mCmWidget)
        mCmWidget->move(0, this->height()- mCmWidget->height());
    if(mCompassWidget)
        mCompassWidget->move(this->width()- mCompassWidget->width() - 5, this->height()- mCompassWidget->height() - 5);
    if(mLocationWidget)
        mLocationWidget->move(mCmWidget->geometry().x() + mCmWidget->width(), this->height() - mLocationWidget->height() - 13);
}
