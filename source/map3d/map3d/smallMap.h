#ifndef SMALLMAP_H
#define SMALLMAP_H

#include <QObject>
#include <osg/Shape>
#include <osgEarthAnnotation/RectangleNode>
#include "osgViewerItem.h"
#include "mapObject.h"
#include <osgEarthUtil/EarthManipulator>
//#include "mapControllerItem.h"
class MapItem;
class SmallMap : public OsgViewerItem
{
    Q_OBJECT
public:
    SmallMap(QQuickItem *parent = nullptr);
    Q_INVOKABLE void setMainMapItem(MapItem *mapItem);
    void initializeOsgEarth();
    void createMapNode(bool geocentric, osgEarth::Map *map);

    void frame() override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
//    Q_INVOKABLE void setMainMap(CameraController *camera , osgViewer::Viewer *viewer);

private:
    MapItem *mMapItem{nullptr};
    osg::ref_ptr<osgEarth::MapNode> mMapNode{nullptr};
    osg::ref_ptr<osg::Group> mMapRoot{nullptr};
    osg::ref_ptr<MapObject> mMapObject;
    osg::ref_ptr<osgEarth::Util::EarthManipulator> mCameraController;
    osgEarth::Annotation::RectangleNode *focalRect;

};

#endif // SMALLMAP_H
