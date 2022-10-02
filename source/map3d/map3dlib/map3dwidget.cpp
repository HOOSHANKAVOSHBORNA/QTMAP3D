﻿#include "map3dwidget.h"

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
#include <osg/Camera>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>

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


MousePicker::MousePicker(QObject *parent)
    :QObject(parent)
{

}

bool MousePicker::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{

    osgViewer::View *view = dynamic_cast<osgViewer::View *>(&aa);

    switch (ea.getEventType())
    {
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

        QMouseEvent* event = new QMouseEvent(QEvent::Type::MouseButtonPress, QPointF(ea.getX(),ea.getY()), mb, mb, Qt::KeyboardModifier::NoModifier);
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
        mapOptProj.profile() = ProfileOptions("plate-carre");
        mMapNodeProj = new MapNode(new Map(mapOptProj));
        mMapNodeProj->getMap()->addLayer(imlayer);

        mMapRoot = new osg::Group();
        mMapRoot->addChild(mMapNodeGeo);
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
    connect(mCmWidget, &CameraManipulatorWidget::headChanged, [=](double val){
        double degri = osg::RadiansToDegrees(val);
        mCompassWidget->setRotate(-degri);
    } );
    //-------------------------------
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

void Map3dWidget::mouseWorldPos(osg::Vec3d pos)
{
    osgEarth::GeoPoint geoPos;
    geoPos.fromWorld(mMapNodeGeo->getMapSRS(),pos);
    qDebug() << geoPos.x()<<" "<<geoPos.y()<<" "<<geoPos.z();
}
void Map3dWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if(mCmWidget)
        mCmWidget->move(0, this->height()- mCmWidget->height());
    if(mCompassWidget)
        mCompassWidget->move(this->width()- mCompassWidget->width() - 5, this->height()- mCompassWidget->height() - 5);

    //mMapOpenGLWidget->resize(height(), height());
}

void Map3dWidget::onMapPressEvent(QMouseEvent *event)
{
    QApplication::postEvent(this,event);
    qDebug()<<event;
}
