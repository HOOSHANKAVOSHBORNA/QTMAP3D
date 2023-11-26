#ifndef SMALLMAP_H
#define SMALLMAP_H

#include <QObject>
#include <osg/Shape>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/RectangleNode>
#include "osgViewerItem.h"
#include "osgEarth/Map"
#include <osgEarthUtil/EarthManipulator>

class MapItem;
class SmallMap : public OsgViewerItem
{
    Q_OBJECT
public:
    SmallMap(QQuickItem *parent = nullptr);
    Q_INVOKABLE void setMainMapItem(MapItem *mapItem);

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
    void compile(bool geocentric);
    void createMapNode(bool geocentric);

private:
    MapItem *mMapItem{nullptr};
    osg::ref_ptr<osgEarth::MapNode> mMapNode{nullptr};
    osg::ref_ptr<osg::Group> mMapRoot{nullptr};
    osg::ref_ptr<osgEarth::Map> mMap;
    osg::ref_ptr<osgEarth::Util::EarthManipulator> mCameraManipulator;
    osgEarth::Viewpoint tmpVP;

};

#endif // SMALLMAP_H
