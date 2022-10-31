
#include "map3dwidget.h"

#include <osgQOpenGL/osgQOpenGLWidget>
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


MousePicker::MousePicker(QObject *parent)
    :QObject(parent)
{

}

bool MousePicker::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{

    osgViewer::View *view = dynamic_cast<osgViewer::View *>(&aa);

    switch (ea.getEventType())
    {
    case osgGA::GUIEventAdapter::FRAME:
        emit frame();
        break;
    case (osgGA::GUIEventAdapter::PUSH):
        if (view) { getPos(view, ea); }
        return false;
    case (osgGA::GUIEventAdapter::MOVE):
        if (view) { getPos(view, ea); }
        return false;
    case (osgGA::GUIEventAdapter::DOUBLECLICK):
    case (osgGA::GUIEventAdapter::KEYDOWN):
        if (view) { getPos(view, ea); }
        return false;
    default:

        return false;
    }
}

void MousePicker::getPos(osgViewer::View *view, const osgGA::GUIEventAdapter &ea)
{
    if(ea.getEventType() == osgGA::GUIEventAdapter::PUSH)
    {
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
        }

        QMouseEvent* event = new QMouseEvent(QEvent::Type::MouseButtonPress,
                                             QPointF(static_cast<qreal>(ea.getX()),static_cast<qreal>(ea.getY())),
                                             mb, mb, Qt::KeyboardModifier::NoModifier);
        emit mousePressEvent(event);
    }

    osgUtil::LineSegmentIntersector::Intersections intersections;
    // Only interact with data nodes and map nodes
    if (view->computeIntersections(ea, intersections))
    {
        for (const auto &intersection : intersections)
        {
            bool  visible = true;
            //                for (const auto node : intersection.nodePath)
            //                {
            //                    // Only count the intersection in main view
            //                    if ((node->getNodeMask() & 0x10000000) == 0)
            //                    {
            //                        visible = false;
            //                        break;
            //                    }
            //                }

            if (visible)
            {
                mCurrentLocalPos    = intersection.getLocalIntersectPoint();
                mCurrentWorldPos    = intersection.getWorldIntersectPoint();
                emit currentWorldPos(mCurrentWorldPos);
                //qDebug() << mCurrentWorldPos.x()<<" "<<mCurrentWorldPos.y()<<" "<<mCurrentWorldPos.z();
                return;
            }
        }
    }
}


Map3dWidget::Map3dWidget(bool isGeocentric, QWidget *parent)
    : QWidget(parent)
    , mSRSwgs84(osgEarth::SpatialReference::get("wgs84"))
{
    mIsGeocentric = isGeocentric;
    mLayout = new QHBoxLayout(this);
    mLayout->setMargin(0);
    mMapOpenGLWidget = new osgQOpenGLWidget(this);
    mMapOpenGLWidget->setMouseTracking(true);

    mLayout->addWidget(mMapOpenGLWidget);
    createWidgets();
    // init signal handle-------------------------------------------------------
    QObject::connect(mMapOpenGLWidget, &osgQOpenGLWidget::initialized, [=]{
        //create camera ----------------------------------------------
        createManipulator();
        mMapOpenGLWidget->getOsgViewer()->setCameraManipulator(mEarthManipulator);
        //add mouse event handler
        auto mousePicker = new MousePicker(mMapOpenGLWidget);
        QObject::connect(mousePicker, &MousePicker::currentWorldPos, this,  &Map3dWidget::mouseWorldPos);
        QObject::connect(mousePicker, &MousePicker::mousePressEvent, this,  &Map3dWidget::onMapPressEvent);
        QObject::connect(mousePicker, &MousePicker::mousePressEvent, mLocationWidget,  &LocationWidget::setClose);
        QObject::connect(mousePicker, &MousePicker::frame, this, &Map3dWidget::onFrame);
        mMapOpenGLWidget->getOsgViewer()->addEventHandler(mousePicker);
        //create map node---------------------------------------------
        GDALOptions gdal;
        gdal.url() = "../map3dlib/data/earth_files/world.tif";
        osg::ref_ptr<ImageLayer> imlayer = new ImageLayer("base-world", gdal);
        MapOptions mapOptGeo;
        mapOptGeo.coordSysType() = MapOptions::CSTYPE_GEOCENTRIC;
        mapOptGeo.profile() = ProfileOptions("global-mercator");
        mMapNodeGeo = new MapNode(new Map(mapOptGeo));
        mMapNodeGeo->getMap()->addLayer(imlayer);

        MapOptions mapOptProj;
        mapOptProj.coordSysType() = MapOptions::CSTYPE_PROJECTED;
//        mapOptProj.profile() = ProfileOptions("plate-carre");
        mapOptProj.profile() = ProfileOptions();
//        qDebug() << "srs proj:"<<QString::fromUtf8(mapOptProj.profile()->srsString()->c_str());
        mMapNodeProj = new MapNode(new Map(mapOptProj));
        mMapNodeProj->getMap()->addLayer(imlayer);

        osg::ref_ptr<osgEarth::Util::SkyNode> skyNodeGeo = osgEarth::Util::SkyNode::create( mMapNodeGeo);
        skyNodeGeo->addChild(mMapNodeGeo);

//        osg::ref_ptr<osgEarth::Util::SkyNode> skyNodeProj = osgEarth::Util::SkyNode::create( mMapNodeProj);
//        skyNodeProj->addChild(mMapNodeProj);

        mMapRoot = new osg::Group();
        mMapRoot->addChild(skyNodeGeo);
        mMapRoot->addChild(mMapNodeProj);

        mMapNodeGeo->setNodeMask(isGeocentric);
        mMapNodeProj->setNodeMask(!isGeocentric);

        mMapOpenGLWidget->getOsgViewer()->setSceneData(mMapRoot);
        mHomeViewpoint = mEarthManipulator->getViewpoint();

        mCmWidget->setStateMap(isGeocentric);

    });


    //setMouseTracking(true);
}

void Map3dWidget::setMap(Map *map)
{
    mMapNodeGeo->getMap()->clear();
    mMapNodeProj->getMap()->clear();

    //    LayerVector layers;
    //    map->getLayers(layers);
    //    foreach (auto layer, layers) {
    //        qDebug()<<layer->getName().c_str();
    //        qDebug()<<layer->getTypeName();
    //        ImageLayer* imLayere = static_cast<ImageLayer*>(layer.get());
    //        auto lay2 = static_cast<ImageLayer*>(imLayere->clone(osg::CopyOp::SHALLOW_COPY));
    //        mMapNodeGeo->getMap()->addLayer(imLayere);
    //        mMapNodeProj->getMap()->addLayer(lay2);
    //    }

    mMapNodeGeo->getMap()->setLayersFromMap(map);
    mMapNodeProj->getMap()->setLayersFromMap(map);

    //    LayerVector layers1;
    //    mMapNodeGeo->getMap()->getLayers(layers1);
    //    foreach (auto layer, layers1) {
    //        qDebug()<<layer->getName().c_str();
    //        qDebug()<<layer->getTypeName();
    //        //mMapNodeGeo->getMap()->addLayer(layer.get());
    //        //mMapNodeProj->getMap()->addLayer(layer.get());
    //    }

    mCmWidget->setStateMap(map->isGeocentric());
    typeChanged(map->isGeocentric());
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
    //osgEarth::Registry::shaderGenerator().run(node);// for textures or lighting
    mMapNodeGeo->addChild(node);
    mMapNodeProj->addChild(node);
    return true;
}

bool Map3dWidget::removeNode(osg::Node *node)
{
    mMapNodeGeo->removeChild(node);
    mMapNodeProj->removeChild(node);
    return true;
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
    if(mIsGeocentric)
        return mMapNodeGeo;
    else
        return mMapNodeProj;
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

    mObjectInfoWidget = new ObjectInfoWidget(this);
    mObjectInfoWidget->setVisible(false);
}

void Map3dWidget::setZoom(double val)
{
    mEarthManipulator->zoom(0, -val, mMapOpenGLWidget->getOsgViewer());
}

void Map3dWidget::home()
{
    mEarthManipulator->home(0);
    //        mEarthManipulator->setViewpoint(mHomeViewpoint);
    mCompassWidget->setPoint(0);
}

void Map3dWidget::typeChanged(bool isGeocentric)
{
    mIsGeocentric = isGeocentric;
    //qDebug()<<"isGeneric:"<<isGeocentric;
    mMapNodeGeo->setNodeMask(isGeocentric);
    mMapNodeProj->setNodeMask(!isGeocentric);
    //    mEarthManipulator->updateProjection()
    //    osg::Camera *c = new osg::Camera() ;
    //    mEarthManipulator->updateCamera(*c);
    //    mEarthManipulator->setInitialVFOV(0);
    //    auto cam = new osgEarth::Util::EarthManipulator;
    //    auto  settings = cam->getSettings();
    //    qDebug()<<settings->getOrthoTracksPerspective();
    //    settings->setOrthoTracksPerspective(isGeocentric);
    //    qDebug()<<settings->getOrthoTracksPerspective();
    //settings->setMinMaxPitch(90, 180);
    //cam->applySettings(settings);
    Viewpoint vp = mEarthManipulator->getViewpoint();
    //    qDebug()<<"vp.heading():"<<QString::fromUtf8(vp.heading()->asString().c_str());
    //    qDebug()<<"vp.pitch():"<<QString::fromUtf8(vp.pitch()->asString().c_str());
    //    qDebug()<<"vp.range():"<<QString::fromUtf8(vp.range()->asString().c_str());
    createManipulator();
    mMapOpenGLWidget->getOsgViewer()->setCameraManipulator(mEarthManipulator);
    mEarthManipulator->setViewpoint(vp);
}

const SpatialReference *Map3dWidget::getMapSRS() const
{
    if(mIsGeocentric)
        return mMapNodeGeo->getMapSRS();
    else
        return mMapNodeProj->getMapSRS();
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

void Map3dWidget::setObjectInfoWidgetVisible(bool bVisible)
{
    mIsObjectInfoWidgetVisible = bVisible;
    mObjectInfoWidget->setVisible(bVisible);
}

void Map3dWidget::setSelectedAirplane(Annotation::ModelNode *airplane)
{
    mSelectedAirplane = airplane;
}

void Map3dWidget::onFrame()
{

    this->mCompassWidget->setRotate(-this->getViewpoint().getHeading());
    if (this->mIsObjectInfoWidgetVisible) {

        osgViewer::Viewer::Views views;
        const osg::Matrixd modelViewMat  = this->mEarthManipulator->getInverseMatrix();
        const osg::Matrixd projectionMat = this->mMapOpenGLWidget->getOsgViewer()->getCamera()->getProjectionMatrix();

        if (this->mSelectedAirplane) {
            const osgEarth::GeoPoint pt = this->mSelectedAirplane->getPosition();
            osg::Vec3d w;
            pt.toWorld(w);

            w = w * modelViewMat;
            w = w * projectionMat;

            const int x = static_cast<int>(((w.x() + 1.0) / 2.0) * double(mMapOpenGLWidget->width()));
            const int y = static_cast<int>((( (w.y()*-1.0) + 1.0) / 2.0) * double(mMapOpenGLWidget->height()));

            mObjectInfoWidget->move(x, y);
        }
    }

}

void Map3dWidget::saveCurrentPosition(QString name)
{
    osgEarth::Viewpoint vp = getViewpoint();

    mLocationWidget->addLocation(name,vp.focalPoint()->x(),vp.focalPoint()->y() ,vp.getRange());
}

void Map3dWidget::mouseWorldPos(osg::Vec3d pos)
{
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(getMapSRS(),pos);
    osgEarth::GeoPoint  latLon;
    geoPos.transform(mSRSwgs84, latLon);
    mLocationWidget->setMousePosition(tr("Lat Lon: [%1, %2, %3]")
                                      .arg(latLon.x(), 0, 'f', 3)
                                      .arg(latLon.y(), 0, 'f', 3)
                                      .arg(latLon.z(), 0, 'f', 3));

    //    qDebug() << latLon.x()<<" "<<latLon.y()<<" "<<latLon.z();
}
void Map3dWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if(mCmWidget)
        mCmWidget->move(0, this->height()- mCmWidget->height());
    if(mCompassWidget)
        mCompassWidget->move(this->width()- mCompassWidget->width() - 5, this->height()- mCompassWidget->height() - 5);
    if(mLocationWidget)
        mLocationWidget->move(mCmWidget->geometry().x() + mCmWidget->width(), this->height() - mLocationWidget->height() - 13);
    if(mObjectInfoWidget)
        mObjectInfoWidget->move(this->width() / 2, this->height() / 2);

    //mMapOpenGLWidget->resize(height(), height());
}

void Map3dWidget::onMapPressEvent(QMouseEvent *event)
{
    QApplication::postEvent(this,event);
    auto vp = getViewpoint();
    mLocationWidget->setCurrentLocation(vp.focalPoint()->x(),vp.focalPoint()->y());
    //qDebug()<<event;
}
