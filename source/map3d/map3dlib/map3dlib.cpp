#include "map3dlib.h"

#include <osgQOpenGL/osgQOpenGLWidget>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/SphericalManipulator>

#include <osgDB/FileUtils>
#include <osgViewer/Viewer>

#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarth/Registry>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/LogarithmicDepthBuffer>
#include <osgEarthUtil/ExampleResources>

Map3dlib::Map3dlib(QWidget *parent)
    : QMainWindow(parent)
{
    osgQOpenGLWidget *widget = new osgQOpenGLWidget(this);

    QObject::connect(widget, &osgQOpenGLWidget::initialized,   [widget]
    {
        // set up the camera manipulators.
        auto manipulator = new osgEarth::Util::EarthManipulator;
        widget->getOsgViewer()->setCameraManipulator(manipulator);
        //read map node ------------------------------
        osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(nullptr);
        osgEarth::Config c;
        c.add("elevation_smoothing", false);
        osgEarth::TerrainOptions to(c);
        osgEarth::MapNodeOptions defMNO;
        defMNO.setTerrainOptions(to);

        myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());
        QString  baseMapPath = QStringLiteral("/home/client110/Documents/project/hooshan/QTMAP3D/source/earth-files/projected.earth");
        osg::Node *baseMap = osgDB::readNodeFile(baseMapPath.toStdString(), myReadOptions);
        //-----------------------------------------------
        widget->getOsgViewer()->setSceneData(baseMap);
    });

    setCentralWidget(widget);
}
