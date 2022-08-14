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

Map3dlib::Map3dlib(QWidget *parent)
    : QMainWindow(parent)
    , mMapOpenGLWidget(new osgQOpenGLWidget(this))
{
    QObject::connect(mMapOpenGLWidget, &osgQOpenGLWidget::initialized, this, &Map3dlib::on_osgQOpenGLWidgetInit);

    setCentralWidget(mMapOpenGLWidget);
    //    QDirIterator it(":", QDirIterator::Subdirectories);
    //    while (it.hasNext()) {
    //        qDebug() << it.next();
    //    }
    QIcon ic(":/res/atlas.ico");
    setWindowIcon(ic);
    //---------------
    auto dockWidget = new QDockWidget(this);
    dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
    dockWidget->setFloating(false);
    dockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
    QSlider* slider = new QSlider(this);
    dockWidget->setWidget(slider);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

void Map3dlib::on_osgQOpenGLWidgetInit()
{
    // set up the camera manipulators.
    mEarthManipulator = new osgEarth::Util::EarthManipulator;
    mMapOpenGLWidget->getOsgViewer()->setCameraManipulator(mEarthManipulator);
    //read map node ------------------------------
    osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(nullptr);
    osgEarth::Config c;
    c.add("elevation_smoothing", false);
    osgEarth::TerrainOptions to(c);
    osgEarth::MapNodeOptions defMNO;
    defMNO.setTerrainOptions(to);
    myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());
    QString  baseMapPath = QStringLiteral("../map3dlib/res/earth_files/projected.earth");
    osg::Node *baseMap = osgDB::readNodeFile(baseMapPath.toStdString(), myReadOptions);
    //-----------------------------------------------
    mMapOpenGLWidget->getOsgViewer()->setSceneData(baseMap);
}
