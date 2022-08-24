#ifndef MAP3DWIDGET_H
#define MAP3DWIDGET_H

#include <QHBoxLayout>
#include <QWidget>


#include <osg/ref_ptr>

#include "cameramanipulatorwidget.h"
#include "compasswidget.h"

class osgQOpenGLWidget;
namespace osgEarth{
class Map;
class MapNode;
namespace  Util{
    class EarthManipulator;
}
}
namespace osg{
class Group;

}

class Map3dWidget: public QWidget
{
    Q_OBJECT
public:
    Map3dWidget(QString baseMapPath, QWidget *parent = nullptr);
    Map3dWidget(bool isGeocentric, QWidget *parent = nullptr);
public slots:
    void setZoom(double);
    void typeChanged(bool);
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    void createManipulator();
    void createWidgets();
private:
    osgQOpenGLWidget* mMapOpenGLWidget;
    osgEarth::Util::EarthManipulator* mEarthManipulator;
    osgEarth::MapNode* mMapNodeGeo;
    osgEarth::MapNode* mMapNodeProj;
    osg::Group* mMapRoot;
private:
    CameraManipulatorWidget *mCmWidget{nullptr};
    CompassWidget *mCompassWidget{nullptr};
    QHBoxLayout *mLayout{nullptr};
};

#endif // MAP3DWIDGET_H
