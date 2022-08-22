#ifndef MAP3DWIDGET_H
#define MAP3DWIDGET_H

#include <QHBoxLayout>
#include <QWidget>

#include "cameramanipulatorwidget.h"
#include "compasswidget.h"

class osgQOpenGLWidget;
namespace osgEarth{
namespace  Util{
    class EarthManipulator;
}
}

class Map3dWidget: public QWidget
{
    Q_OBJECT
public:
    Map3dWidget(QWidget *parent = nullptr);
public slots:
    void osgQOpenGLWidgetInitialized();
    void setZoom(double);
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    void initMapOpenGLWidget();
private:
    osgQOpenGLWidget *mMapOpenGLWidget;
    osgEarth::Util::EarthManipulator *mEarthManipulator;
    CameraManipulatorWidget *mCmWidget;
    CompassWidget *mCompassWidget;
    bool mIsGeocentric;
    QHBoxLayout *mHBLayout;
};

#endif // MAP3DWIDGET_H
