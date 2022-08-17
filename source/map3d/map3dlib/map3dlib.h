#ifndef MAP3DLIB_H
#define MAP3DLIB_H

#include "map3dlib_global.h"
#include <QMainWindow>

class osgQOpenGLWidget;
namespace osgEarth{
namespace  Util{
    class EarthManipulator;
}
}
class CameraManipulatorWidget;
class CompassWidget;

class MAP3DLIB_EXPORT Map3dlib: public QMainWindow
{
    Q_OBJECT
public:
    Map3dlib(QWidget *parent = nullptr);
public slots:
    void osgQOpenGLWidgetInitialized();
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    osgQOpenGLWidget *mMapOpenGLWidget;
    osgEarth::Util::EarthManipulator *mEarthManipulator;
    QDockWidget *dockWidget;
    CameraManipulatorWidget *mCmWidget;
    CompassWidget *mCompassWidget;
};

#endif // MAP3DLIB_H
