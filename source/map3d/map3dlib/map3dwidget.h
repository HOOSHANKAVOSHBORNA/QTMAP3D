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

class osgQOpenGLWidget;

namespace osgEarth{
class Viewpoint;
//class Map;
//class MapNode;
//namespace  Util{
//    class EarthManipulator;
//}
}
//namespace osg{
//class Group;

//}

class Map3dWidget: public QWidget
{
    Q_OBJECT
public:
    explicit Map3dWidget(bool isGeocentric, QWidget *parent = nullptr);

    void setMap(osgEarth::Map *map);
    void setViewpoint( const osgEarth::Viewpoint& vp, double duration_s =0.0 );
    osgEarth::Viewpoint getViewpoint() const;

    osgEarth::MapNode* getMapNode();
    osg::ref_ptr<osg::Group> mMapRoot;
    osg::ref_ptr<osgEarth::Util::EarthManipulator> mEarthManipulator;
public slots:
    void setZoom(double);
    void home();
    void typeChanged(bool);
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    void createManipulator();
    void createWidgets();
private:

    osg::ref_ptr<osgEarth::MapNode> mMapNodeGeo;
    osg::ref_ptr<osgEarth::MapNode> mMapNodeProj;

    osgEarth::Viewpoint mHomeViewpoint;
private:
    osgQOpenGLWidget* mMapOpenGLWidget;
    CameraManipulatorWidget *mCmWidget{nullptr};
    CompassWidget *mCompassWidget{nullptr};
    QHBoxLayout *mLayout{nullptr};
    bool mIsGeocentric;
};

#endif // MAP3DWIDGET_H
