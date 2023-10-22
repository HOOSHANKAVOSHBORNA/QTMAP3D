#ifndef SMALLMAP_H
#define SMALLMAP_H

#include <QObject>
#include <osg/Shape>
#include <osgEarthAnnotation/RectangleNode>
#include "mapItem.h"
#include "mapObject.h"

class SmallMap : public MapItem
{
    Q_OBJECT
public:
    SmallMap(QQuickItem *parent = nullptr);
    void initializeOsgEarth() override;
    void setLocation(const osgEarth::GeoPoint &geoPos);
    void createMapNode(bool geocentric, osgEarth::Map *map);
    void createCameraManipulator();
    void frame() override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
private:
    osg::ref_ptr<osgEarth::Annotation::RectangleNode> mLocation;
    osg::ref_ptr<osgEarth::MapNode> mMapNode{nullptr};
    osg::ref_ptr<osgEarth::Util::SkyNode> mSkyNode{nullptr};
    osg::ref_ptr<osg::Group> mMapRoot{nullptr};
    osg::ref_ptr<MapObject> mMapObject;
    osg::ref_ptr<CameraController> mCameraController;

};

#endif // SMALLMAP_H
