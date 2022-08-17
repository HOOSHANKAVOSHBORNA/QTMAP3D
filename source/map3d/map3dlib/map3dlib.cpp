#include "map3dlib.h"

#include <osgQOpenGL/osgQOpenGLWidget>

#include <osgDB/FileUtils>
#include <osgViewer/Viewer>

#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarth/Registry>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/LogarithmicDepthBuffer>
#include <osgEarthUtil/ExampleResources>
#include <QDirIterator>
#include<QDebug>
#include <QDockWidget>
#include <QSlider>

#include "cameramanipulatorwidget.h"
#include "compasswidget.h"

const double ZOOM_STEP{0.2};
const double UP_DOWN_STEP{0.1};
const double LEFT_RIGHT_STEP{0.1};
const double HEAD_STEP{5*osg::PI/180};
const double PITCH_STEP{5*osg::PI/180};

const double MIN_DISTANCE{10.0};
const double MAX_DISTANCE{1000000000.0};
const double MAX_OFSET{5000.0};

Map3dlib::Map3dlib(QWidget *parent)
    : QMainWindow(parent)
    , mIsGeocentric(true)
{
    //    QDirIterator it(":", QDirIterator::Subdirectories);
    //    while (it.hasNext()) {
    //        qDebug() << it.next();
    //    }
    QIcon ic(":/res/atlas.ico");
    setWindowIcon(ic);

    initMapOpenGLWidget();

    mCmWidget = new CameraManipulatorWidget(this);
    // setting CameraManipulatorWidget
    mCmWidget->setZoomStep(ZOOM_STEP);
    mCmWidget->setUpDownStep(UP_DOWN_STEP);
    mCmWidget->setLeftRightStep(LEFT_RIGHT_STEP);
    mCmWidget->setHeadStep(HEAD_STEP);
    mCmWidget->setPitchStep(PITCH_STEP);
    connect(mCmWidget, &CameraManipulatorWidget::homeClicked, [=]{
        mEarthManipulator->home(0);
        mCompassWidget->setPoint(0);
    } );
    connect(mCmWidget, &CameraManipulatorWidget::zoomChanged, this,&Map3dlib::zoomChanged);
    connect(mCmWidget, &CameraManipulatorWidget::upDownChanged, [=](double val){mEarthManipulator->pan(0,val);} );
    connect(mCmWidget, &CameraManipulatorWidget::leftRightChanged, [=](double val){mEarthManipulator->pan(val,0);} );
    connect(mCmWidget, &CameraManipulatorWidget::headChanged, [=](double val){mEarthManipulator->rotate(val,0);} );
    connect(mCmWidget, &CameraManipulatorWidget::pitchChanged, [=](double val){mEarthManipulator->rotate(0,val);} );
    connect(mCmWidget, &CameraManipulatorWidget::mapChange, [=](bool val){
        mIsGeocentric = !val;
        initMapOpenGLWidget();
        mMapOpenGLWidget->stackUnder(mCmWidget);
        mMapOpenGLWidget->stackUnder(mCmWidget);
    });
    //-------------------------------------
    mCompassWidget = new CompassWidget(this);
    connect(mCmWidget, &CameraManipulatorWidget::headChanged, [=](double val){
        double degri = val * 180/osg::PI;
        mCompassWidget->setRotate(-degri);
    } );
    //-------------------------------
}
void Map3dlib::initMapOpenGLWidget()
{
    if(mMapOpenGLWidget != nullptr)
    {
        delete mMapOpenGLWidget;
        mMapOpenGLWidget = nullptr;
    }
    mMapOpenGLWidget = new osgQOpenGLWidget(this);
    QObject::connect(mMapOpenGLWidget, &osgQOpenGLWidget::initialized, this, &Map3dlib::osgQOpenGLWidgetInitialized);
    setCentralWidget(mMapOpenGLWidget);
}

void Map3dlib::osgQOpenGLWidgetInitialized()
{
    // set up the camera manipulators.
    auto earthManipulator = new osgEarth::Util::EarthManipulator;
    auto  settings = earthManipulator->getSettings();
    settings->setSingleAxisRotation(true);
    //qDebug()<<settings->getMaxDistance();
    settings->setMinMaxDistance(MIN_DISTANCE, MAX_DISTANCE);
    settings->setMaxOffset(MAX_OFSET, MAX_OFSET);
    //settings->setMinMaxPitch(-90, 90);
    settings->setTerrainAvoidanceEnabled(true);
    settings->setThrowingEnabled(false);
    mMapOpenGLWidget->getOsgViewer()->setCameraManipulator(earthManipulator);
    //read map node ------------------------------
    osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(nullptr);
    osgEarth::Config c;
    c.add("elevation_smoothing", false);
    osgEarth::TerrainOptions to(c);
    osgEarth::MapNodeOptions defMNO;
    defMNO.setTerrainOptions(to);
    myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());
    QString  baseMapPath;
    if(mIsGeocentric)
        baseMapPath = QStringLiteral("../map3dlib/res/earth_files/geocentric.earth");
    else
        baseMapPath = QStringLiteral("../map3dlib/res/earth_files/projected.earth");
    //    QString  baseMapPath = QStringLiteral(":/res/geocentric.earth");
    osg::Node *baseMap = osgDB::readNodeFile(baseMapPath.toStdString(), myReadOptions);
    //-----------------------------------------------
    mMapOpenGLWidget->getOsgViewer()->setSceneData(baseMap);
    //auto mapNode = osgEarth::MapNode::get(baseMap);
    //mapNode->getMap();
    mEarthManipulator = earthManipulator;
}

void Map3dlib::zoomChanged(double val)
{
    mEarthManipulator->zoom(0, -val, mMapOpenGLWidget->getOsgViewer());
}
void Map3dlib::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    mCmWidget->move(0, this->height()- mCmWidget->height());
    mCompassWidget->move(this->width()- mCompassWidget->width() - 5, this->height()- mCompassWidget->height() - 5);
}

