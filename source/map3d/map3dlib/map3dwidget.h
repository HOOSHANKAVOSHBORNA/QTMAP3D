#ifndef MAP3DWIDGET_H
#define MAP3DWIDGET_H

#include <QHBoxLayout>
#include <QWidget>

#include <osgQOpenGL/osgQOpenGLWidget>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osgEarth/Viewpoint>
#include <osgEarth/MapNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/Sky>

#include "cameramanipulatorwidget.h"
#include "compasswidget.h"
#include "locationwidget.h"
#include "objectinfowidget.h"

namespace osgEarth{
class Viewpoint;
class SpatialReference;
class Layer;
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
class Map3dWidget;

class  MousePicker: public osgGA::GUIEventHandler
{
public:
    MousePicker(Map3dWidget *map3dWidget);
    virtual ~MousePicker()override{}
protected:
    bool  handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private:
    void  mouseEvent(osgViewer::View *view, const osgGA::GUIEventAdapter &ea, QEvent::Type qEventType);

private:
   Map3dWidget* mMap3dWidget;
};

class Map3dWidget: public osgQOpenGLWidget
{
    Q_OBJECT
public:
    explicit Map3dWidget(bool isGeocentric, QWidget *parent = nullptr);

    void setMap(osgEarth::Map *map);
    void setTrackNode(osg::Node* value);
    void unTrackNode();
    bool addNode( osg::Node *node);
    bool removeNode( osg::Node *node);
    void setViewpoint( const osgEarth::Viewpoint& vp, double duration_s =0.0 );
    osgEarth::Viewpoint getViewpoint() const;
    //void mousePressEvent (QMouseEvent *event) override;
    osgEarth::MapNode *getMapNode();
    void addLayer(osgEarth::Layer* layer);
    osg::ref_ptr<osg::Group> mMapRoot;
    osg::ref_ptr<osgEarth::Util::EarthManipulator> mEarthManipulator;

public slots:
    void setZoom(double);
    void home();
    void typeChanged(bool);
    const osgEarth::SpatialReference* getMapSRS() const;
    void goPosition(double latitude ,double longitude ,double range);

signals :
    void mouseEvent(QMouseEvent* event, osgEarth::GeoPoint geoPos);
    void mapNodeChanged(osgEarth::MapNode* newMapNode);
protected:
    void resizeEvent(QResizeEvent* event) override;
private slots:
    void saveCurrentPosition(QString name);
    //void mouseWorldPos(osg::Vec3d pos);
    //void onMapPressEvent(QMouseEvent *event);
private:
    void createMapNode(bool isGeocentric);
    void createManipulator();
    void createWidgets();
    void frame();
    void mapMouseEvent(QMouseEvent* event, osg::Vec3d worldPos);
private:
//    osg::ref_ptr<osgEarth::MapNode> mMapNodeGeo;
//    osg::ref_ptr<osgEarth::MapNode> mMapNodeProj;
    osg::ref_ptr<osgEarth::MapNode> mMapNode;
    osg::ref_ptr<osgEarth::Util::SkyNode> mSkyNode;
    osg::ref_ptr<const osgEarth::SpatialReference>  mSRSwgs84;
    osgEarth::Viewpoint mHomeViewpoint;
private:
    friend class MousePicker;
//    osgQOpenGLWidget* mMapOpenGLWidget;
    CameraManipulatorWidget *mCmWidget{nullptr};
    CompassWidget *mCompassWidget{nullptr};
    LocationWidget* mLocationWidget{nullptr};
    QHBoxLayout *mLayout{nullptr};
    bool mIsGeocentric;
};

#endif // MAP3DWIDGET_H
