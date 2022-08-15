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
    //---------------
    auto dockWidget = new QDockWidget(this);
    dockWidget->setFloating(false);
    dockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);

    CameraManipulatorWidget *cmWidget = new CameraManipulatorWidget(this);
    dockWidget->setWidget(cmWidget);
    dockWidget->setMinimumSize(QSize(60,190));
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    // setting CameraManipulatorWidget
    cmWidget->setZoomStep(1);
    cmWidget->setUpDownStep(1);
    connect(cmWidget, &CameraManipulatorWidget::homeClicked, [=]{mEarthManipulator->home(0);} );
    connect(cmWidget, &CameraManipulatorWidget::zoomChanged, [=](float val){mEarthManipulator->zoom(0, val, mMapOpenGLWidget->getOsgViewer());} );
    connect(cmWidget, &CameraManipulatorWidget::upDownChanged, [=](float val){mEarthManipulator->pan(0,val);} );
}

void Map3dlib::osgQOpenGLWidgetInitialized()
{
    // set up the camera manipulators.
    mEarthManipulator = new osgEarth::Util::EarthManipulator;
    mMapOpenGLWidget->getOsgViewer()->setCameraManipulator(mEarthManipulator);
    auto  settings = mEarthManipulator->getSettings();
    settings->setSingleAxisRotation(true);
    qDebug()<<settings->getMaxDistance();
    settings->setMinMaxDistance(10.0, 1000000000);
    settings->setMaxOffset(5000.0, 5000.0);
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
    QString  baseMapPath = QStringLiteral("../map3dlib/res/earth_files/projected.earth");
//    QString  baseMapPath = QStringLiteral(":/res/geocentric.earth");
    osg::Node *baseMap = osgDB::readNodeFile(baseMapPath.toStdString(), myReadOptions);
    //-----------------------------------------------
    mMapOpenGLWidget->getOsgViewer()->setSceneData(baseMap);
}

void Map3dlib::zoomChanged(int val)
{
//    qDebug()<<val;
//    auto distance = mEarthManipulator->getDistance();
//    qDebug()<<distance;
//    mEarthManipulator->setDistance(distance - 500000);
    //mEarthManipulator->pan(0,0.1);
    //mEarthManipulator->rotate( 0, 3.14/50 );
    //mEarthManipulator->zoom(0,-0.5,mMapOpenGLWidget->getOsgViewer());
//    mEarthManipulator->home(0);
}
