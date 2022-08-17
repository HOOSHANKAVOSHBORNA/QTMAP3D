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
#include "campasswidget.h"

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
    , mMapOpenGLWidget(new osgQOpenGLWidget(this))
{
    QObject::connect(mMapOpenGLWidget, &osgQOpenGLWidget::initialized, this, &Map3dlib::osgQOpenGLWidgetInitialized);

    setCentralWidget(mMapOpenGLWidget);
    //    QDirIterator it(":", QDirIterator::Subdirectories);
    //    while (it.hasNext()) {
    //        qDebug() << it.next();
    //    }
    QIcon ic(":/res/atlas.ico");
    setWindowIcon(ic);
    //setStyleSheet("background-color:#607D8B;");
    setStyleSheet("background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 #09203f, stop:1 #537895);");

    //---------------
    //dockWidget = new QDockWidget(this);
    //dockWidget->setFloating(false);
    //dockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
    //dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //dockWidget->setStyleSheet("background-color:transparent;");
    mCmWidget = new CameraManipulatorWidget(this);
    //dockWidget->setWidget(cmWidget);
    //dockWidget->setFixedSize(cmWidget->size()+ QSize(0,25));
    //addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    // setting CameraManipulatorWidget
    mCmWidget->setZoomStep(ZOOM_STEP);
    mCmWidget->setUpDownStep(UP_DOWN_STEP);
    mCmWidget->setLeftRightStep(LEFT_RIGHT_STEP);
    mCmWidget->setHeadStep(HEAD_STEP);
    mCmWidget->setPitchStep(PITCH_STEP);
    connect(mCmWidget, &CameraManipulatorWidget::homeClicked, [=]{
        mEarthManipulator->home(0);
        mCompassWidget->setRotate(0);
    } );
    connect(mCmWidget, &CameraManipulatorWidget::zoomChanged, [=](double val){mEarthManipulator->zoom(0, -val, mMapOpenGLWidget->getOsgViewer());} );
    connect(mCmWidget, &CameraManipulatorWidget::upDownChanged, [=](double val){mEarthManipulator->pan(0,val);} );
    connect(mCmWidget, &CameraManipulatorWidget::leftRightChanged, [=](double val){mEarthManipulator->pan(val,0);} );
    connect(mCmWidget, &CameraManipulatorWidget::headChanged, [=](double val){mEarthManipulator->rotate(val,0);} );
    connect(mCmWidget, &CameraManipulatorWidget::pitchChanged, [=](double val){mEarthManipulator->rotate(0,val);} );
    //-------------------------------------
    mCompassWidget = new CampassWidget(this);
    connect(mCmWidget, &CameraManipulatorWidget::headChanged, [=](double val){
        double degri = val * 180/osg::PI;
        mCompassWidget->setRotate(-degri);
    } );

}

void Map3dlib::osgQOpenGLWidgetInitialized()
{
    // set up the camera manipulators.
    mEarthManipulator = new osgEarth::Util::EarthManipulator;
    mMapOpenGLWidget->getOsgViewer()->setCameraManipulator(mEarthManipulator);
    auto  settings = mEarthManipulator->getSettings();
    settings->setSingleAxisRotation(true);
    //qDebug()<<settings->getMaxDistance();
    settings->setMinMaxDistance(MIN_DISTANCE, MAX_DISTANCE);
    settings->setMaxOffset(MAX_OFSET, MAX_OFSET);
    //settings->setMinMaxPitch(-90, 90);
    settings->setTerrainAvoidanceEnabled(true);
    settings->setThrowingEnabled(false);
    //read map node ------------------------------
    osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(nullptr);
    osgEarth::Config c;
    c.add("elevation_smoothing", false);
    osgEarth::TerrainOptions to(c);
    osgEarth::MapNodeOptions defMNO;
    defMNO.setTerrainOptions(to);
    myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());
    QString  baseMapPath = QStringLiteral("../map3dlib/res/earth_files/geocentric.earth");
//    QString  baseMapPath = QStringLiteral(":/res/geocentric.earth");
    osg::Node *baseMap = osgDB::readNodeFile(baseMapPath.toStdString(), myReadOptions);
    //-----------------------------------------------
    mMapOpenGLWidget->getOsgViewer()->setSceneData(baseMap);
}
void Map3dlib::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   // Your code here.
   //dockWidget->setFloating(false);
   //qDebug()<<"main:"<<geometry();
   //qDebug()<<"addDockWidget:"<<dockWidget->geometry();
   //dockWidget->setParent(this);
   //dockWidget->move(0,0);
//   mCmWidget->move()
   mCmWidget->move(this->width()- mCmWidget->width(), 0);
}
