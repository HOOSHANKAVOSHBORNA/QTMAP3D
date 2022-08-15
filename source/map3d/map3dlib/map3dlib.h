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

class MAP3DLIB_EXPORT Map3dlib: public QMainWindow
{
    Q_OBJECT

public:
    Map3dlib(QWidget *parent = nullptr);
public slots:
    void osgQOpenGLWidgetInitialized();
    void zoomChanged(int val);
private:
    osgQOpenGLWidget *mMapOpenGLWidget;
    osgEarth::Util::EarthManipulator *mEarthManipulator;
};

#endif // MAP3DLIB_H
