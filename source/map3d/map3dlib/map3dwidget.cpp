#include "map3dwidget.h"

//#include <osgQOpenGL/osgQOpenGLWidget>
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

#include <QDirIterator>
#include<QDebug>
#include <QDockWidget>
#include <QSlider>
#include <QHBoxLayout>

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

Map3dWidget::Map3dWidget(QString baseMapPath, QWidget *parent)
    : QWidget(parent)
{
    mLayout = new QHBoxLayout(this);
    mLayout->setMargin(0);
    mMapOpenGLWidget = new osgQOpenGLWidget(this);
    mLayout->addWidget(mMapOpenGLWidget);
    createWidgets();
//    const int min = std::min(mMapOpenGLWidget->width(), mMapOpenGLWidget->height());
//    mMapOpenGLWidget->resize(min, min);
    // init signal handle-------------------------------------------------------
    QObject::connect(mMapOpenGLWidget, &osgQOpenGLWidget::initialized, [=]{
        //create camera ----------------------------------------------
        createManipulator();
        mMapOpenGLWidget->getOsgViewer()->setCameraManipulator(mEarthManipulator);
        //create map node---------------------------------------------
        osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(nullptr);
        osgEarth::Config c;
        c.add("elevation_smoothing", false);
        osgEarth::TerrainOptions to(c);
        osgEarth::MapNodeOptions defMNO;
        defMNO.setTerrainOptions(to);
        myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());
        osg::ref_ptr<osg::Node> baseMap = osgDB::readNodeFile(baseMapPath.toStdString(), myReadOptions);
        osg::ref_ptr<osgEarth::MapNode> mapNode1 = osgEarth::MapNode::get(baseMap);

        mCmWidget->setStateMap(mapNode1->isGeocentric());

        MapOptions mapOpt;
        if(mapNode1->isGeocentric())
        {
            mapOpt.coordSysType() = MapOptions::CSTYPE_PROJECTED;
            mapOpt.profile() = ProfileOptions("plate-carre");
        }
        else
        {
            mapOpt.coordSysType() = MapOptions::CSTYPE_GEOCENTRIC;
            mapOpt.profile() = ProfileOptions("global-mercator");
        }
        // create map from other map
        //qDebug()<<mapNode1->getMap()->getNumLayers();
        osg::ref_ptr<osgEarth::MapNode> mapNode2 = new MapNode(new Map(mapOpt));
        mapNode2->getMap()->setLayersFromMap(mapNode1->getMap());
        //qDebug()<<mapNode1->getMap()->getNumLayers();

        if(mapNode1->isGeocentric())
        {
            mMapNodeGeo = mapNode1.get();
            mMapNodeProj = mapNode2.get();
            mMapNodeProj->setNodeMask(false);
        }
        else
        {
            mMapNodeGeo = mapNode2.get();
            mMapNodeProj = mapNode1.get();
            mMapNodeGeo->setNodeMask(false);
        }
        mMapRoot = new osg::Group();
        mMapRoot->addChild(mMapNodeGeo);
        mMapRoot->addChild(mMapNodeProj);
        mMapOpenGLWidget->getOsgViewer()->setSceneData(mMapRoot);
        mHomeViewpoint = mEarthManipulator->getViewpoint();

    });
}

Map3dWidget::Map3dWidget(bool isGeocentric, QWidget *parent)
    : QWidget(parent)
{
    mLayout = new QHBoxLayout(this);
    mLayout->setMargin(0);
    mMapOpenGLWidget = new osgQOpenGLWidget(this);

    mLayout->addWidget(mMapOpenGLWidget);
    createWidgets();
    // init signal handle-------------------------------------------------------
    QObject::connect(mMapOpenGLWidget, &osgQOpenGLWidget::initialized, [=]{
        //create camera ----------------------------------------------
        createManipulator();
        mMapOpenGLWidget->getOsgViewer()->setCameraManipulator(mEarthManipulator);
        //create map node---------------------------------------------
        GDALOptions gdal;
        gdal.url() = "../map3dlib/data/earth_files/world.tif";

        MapOptions mapOptGeo;
        mapOptGeo.coordSysType() = MapOptions::CSTYPE_GEOCENTRIC;
        mapOptGeo.profile() = ProfileOptions("global-mercator");
        mMapNodeGeo = new MapNode(new Map(mapOptGeo));
        mMapNodeGeo->getMap()->addLayer(new ImageLayer("base-world", gdal));

        MapOptions mapOptProj;
        mapOptProj.coordSysType() = MapOptions::CSTYPE_PROJECTED;
        mapOptProj.profile() = ProfileOptions("plate-carre");
        mMapNodeProj = new MapNode(new Map(mapOptProj));
        mMapNodeProj->getMap()->addLayer(new ImageLayer("base-world", gdal));

        mMapRoot = new osg::Group();
        mMapRoot->addChild(mMapNodeGeo);
        mMapRoot->addChild(mMapNodeProj);

        mMapNodeGeo->setNodeMask(isGeocentric);
        mMapNodeProj->setNodeMask(!isGeocentric);

        mMapOpenGLWidget->getOsgViewer()->setSceneData(mMapRoot);
        mHomeViewpoint = mEarthManipulator->getViewpoint();

        mCmWidget->setStateMap(isGeocentric);

    });
}

void Map3dWidget::createManipulator()
{
    mEarthManipulator = new osgEarth::Util::EarthManipulator;
    auto  settings = mEarthManipulator->getSettings();
    settings->setSingleAxisRotation(true);
    //qDebug()<<settings->getMaxDistance();
    settings->setMinMaxDistance(MIN_DISTANCE, MAX_DISTANCE);
    settings->setMaxOffset(MAX_OFSET, MAX_OFSET);
    //settings->setMinMaxPitch(-90, 90);
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
    connect(mCmWidget, &CameraManipulatorWidget::homeClicked, [=]{
        mEarthManipulator->home(0);
//        mEarthManipulator->setViewpoint(mHomeViewpoint);
        mCompassWidget->setPoint(0);
    } );
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

void Map3dWidget::typeChanged(bool isGeocentric)
{
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
void Map3dWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if(mCmWidget)
        mCmWidget->move(0, this->height()- mCmWidget->height());
    if(mCompassWidget)
        mCompassWidget->move(this->width()- mCompassWidget->width() - 5, this->height()- mCompassWidget->height() - 5);
}

