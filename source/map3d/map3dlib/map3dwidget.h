#ifndef MAP3DWIDGET_H
#define MAP3DWIDGET_H

#include <QHBoxLayout>
#include <QWidget>


#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osgEarth/Viewpoint>
#include <osgEarth/MapNode>
//#include <osgQOpenGL/osgQOpenGLWidget>
#include <osgEarthUtil/EarthManipulator>

#include "cameramanipulatorwidget.h"
#include "compasswidget.h"
#include "locationwidget.h"

class osgQOpenGLWidget;

namespace osgEarth{
class Viewpoint;
class SpatialReference;
//class Map;
//class MapNode;
//namespace  Util{
//    class EarthManipulator;
//}
}
//namespace osg{
//class Group;

//}
namespace osgViewer
{
class View;
}

class  MousePicker:public QObject, public osgGA::GUIEventHandler
{
    Q_OBJECT

public:
    MousePicker(QObject *parent = nullptr);
    virtual ~MousePicker()override{}
signals:
    void  currentWorldPos(osg::Vec3d pos);
    void mousePressEvent(QMouseEvent *event);
protected:
    // Public main entrance for GUIEventHandler
    bool  handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private:
    // Intersect with the scene and update info about the intersected point
    void  getPos(osgViewer::View *view, const osgGA::GUIEventAdapter &ea);

private:
    osg::Vec3d mCurrentLocalPos;
    osg::Vec3d mCurrentWorldPos;
};

class Map3dWidget: public QWidget
{
    Q_OBJECT
public:
    explicit Map3dWidget(bool isGeocentric, QWidget *parent = nullptr);

    void setMap(osgEarth::Map *map);
    void setViewpoint( const osgEarth::Viewpoint& vp, double duration_s =0.0 );
    osgEarth::Viewpoint getViewpoint() const;
    //void mousePressEvent (QMouseEvent *event) override;
    osgEarth::MapNode* getMapNode();
    osg::ref_ptr<osg::Group> mMapRoot;
    osg::ref_ptr<osgEarth::Util::EarthManipulator> mEarthManipulator;

public slots:
    void setZoom(double);
    void home();
    void typeChanged(bool);
    const osgEarth::SpatialReference* getMapSRS() const;
    void onGoPosition(float latitude ,float longitude ,float range);
    void onSavePosition(QString name);
    void onClickedPosition(osgEarth::Viewpoint* point);
private slots:
    void mouseWorldPos(osg::Vec3d pos);
    void onMapPressEvent(QMouseEvent *event);
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    void createManipulator();
    void createWidgets();
private:

    osg::ref_ptr<osgEarth::MapNode> mMapNodeGeo;
    osg::ref_ptr<osgEarth::MapNode> mMapNodeProj;
    osg::ref_ptr<const osgEarth::SpatialReference>  mSRSwgs84;
    osgEarth::Viewpoint mHomeViewpoint;
private:
    osgQOpenGLWidget* mMapOpenGLWidget;
    CameraManipulatorWidget *mCmWidget{nullptr};
    CompassWidget *mCompassWidget{nullptr};
    LocationWidget* mLocationWidget{nullptr};
    QHBoxLayout *mLayout{nullptr};
    bool mIsGeocentric;
};

#endif // MAP3DWIDGET_H
