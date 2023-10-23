#ifndef SMALLMAP_H
#define SMALLMAP_H

#include <QObject>
#include <osg/Shape>
#include <osgEarthAnnotation/RectangleNode>
#include "mapItem.h"
#include "mapObject.h"
//#include "mapControllerItem.h"

class SmallMap : public MapItem
{
    Q_OBJECT
public:
    SmallMap(QQuickItem *parent = nullptr);
    void initializeOsgEarth() override;
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
    Q_INVOKABLE void setMainMap(CameraController *camera , osgViewer::Viewer *viewer);

private:
    osg::ref_ptr<osgEarth::MapNode> mMapNode{nullptr};
    osg::ref_ptr<osg::Group> mMapRoot{nullptr};
    osg::ref_ptr<MapObject> mMapObject;
    osg::ref_ptr<CameraController> mCameraController;
    CameraController *mainMapCamera;
    osgViewer::Viewer*  mainMapView;
    osgEarth::Annotation::RectangleNode *focalRect;

};

#endif // SMALLMAP_H
