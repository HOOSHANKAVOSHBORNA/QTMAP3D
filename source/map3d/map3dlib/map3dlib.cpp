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
    dockWidget = new QDockWidget(this);
    dockWidget->setFloating(false);
    dockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //dockWidget->setStyleSheet("background-color:#607D8B;");
    CameraManipulatorWidget *cmWidget = new CameraManipulatorWidget(this);
    //cmWidget->resize(200,200);
    dockWidget->setWidget(cmWidget);
    dockWidget->setMinimumSize(cmWidget->minimumSize()+ QSize(0,25));
    dockWidget->setMaximumSize(cmWidget->minimumSize()+ QSize(0,25));
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    // setting CameraManipulatorWidget
    cmWidget->setZoomStep(ZOOM_STEP);
    cmWidget->setUpDownStep(UP_DOWN_STEP);
    cmWidget->setLeftRightStep(LEFT_RIGHT_STEP);
    cmWidget->setHeadStep(HEAD_STEP);
    cmWidget->setPitchStep(PITCH_STEP);

    connect(cmWidget, &CameraManipulatorWidget::homeClicked, [=]{mEarthManipulator->home(0);} );
    connect(cmWidget, &CameraManipulatorWidget::zoomChanged, [=](double val){mEarthManipulator->zoom(0, -val, mMapOpenGLWidget->getOsgViewer());} );
    connect(cmWidget, &CameraManipulatorWidget::upDownChanged, [=](double val){mEarthManipulator->pan(0,val);} );
    connect(cmWidget, &CameraManipulatorWidget::leftRightChanged, [=](double val){mEarthManipulator->pan(val,0);} );
    connect(cmWidget, &CameraManipulatorWidget::headChanged, [=](double val){mEarthManipulator->rotate(val,0);} );
    connect(cmWidget, &CameraManipulatorWidget::pitchChanged, [=](double val){mEarthManipulator->rotate(0,val);} );
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
   qDebug()<<"main:"<<geometry();
   qDebug()<<"addDockWidget:"<<dockWidget->geometry();
}
