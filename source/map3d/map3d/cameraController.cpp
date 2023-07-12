#include "cameraController.h"
#include "mapItem.h"

#include <qdebug.h>

CameraController::CameraController(MapItem *mapItem)
    :mMapItem(mapItem)
{

}

void CameraController::home(double value)
{
    osgEarth::Util::EarthManipulator::home(value);
    auto camSet = getSettings();
    camSet->setMinMaxDistance(0,MAX_CAM_DISTANCE);
    applySettings(camSet);
}

void CameraController::zoom(double delta)
{
    zoom(0, delta, mMapItem->getViewer());
}

void CameraController::zoom(double dx, double dy, osg::View *view)
{
    osgEarth::Util::EarthManipulator::zoom(dx, -dy, view);
}

void CameraController::goToPosition(double lat, double lon, double range, double duration)
{
    osgEarth::GeoPoint  geoPoint(mMapItem->getMapObject()->getSRS()->getGeographicSRS(), lat, lat, 0);
    geoPoint.transformInPlace(mMapItem->getMapObject()->getSRS());

    osgEarth::Viewpoint vp;
    vp.focalPoint() = geoPoint;
    vp.range()= range;
    setViewpoint(vp, duration);
}

void CameraController::goToPosition(double lat, double lon, double range, double pitch, double heading, double duration)
{
    osgEarth::GeoPoint  geoPoint(mMapItem->getMapObject()->getSRS()->getGeographicSRS(), lat, lat, 0);
    geoPoint.transformInPlace(mMapItem->getMapObject()->getSRS());

    osgEarth::Viewpoint vp;
    vp.focalPoint() = geoPoint;
    vp.range()= range;
    vp.pitch() = pitch;
    vp.heading() = heading;
    setViewpoint(vp, duration);
}

void CameraController::goToPosition(osgEarth::GeoPoint mapPoint, double range, double duration)
{
    if(mapPoint.isRelative())
        mapPoint.makeAbsolute(mMapItem->getMapNode()->getTerrain());
    osgEarth::Viewpoint vp;
    vp.focalPoint() = mapPoint;
    vp.range()= range;
    setViewpoint(vp, duration);
}

void CameraController::setHeadingToNorth()
{
    osgEarth::Viewpoint vp = getViewpoint();
    vp.heading() = 0;
    setViewpoint(vp, 0.3);
}

void CameraController::setTrackNode(osg::Node *node, double minDistance)
{
    auto vp = getViewpoint();
    if(vp.getNode() == node)
        return;

    vp.setNode(node);
    setViewpoint(vp);
    auto camSet = getSettings();
    camSet->setMinMaxDistance(minDistance,MAX_CAM_DISTANCE);
    camSet->setTetherMode(osgEarth::Util::EarthManipulator::TetherMode::TETHER_CENTER);
    applySettings(camSet);
}

void CameraController::untrack()
{
    auto vp = getViewpoint();
    //    if(vp.getNode() == nullptr)
    //        return;
    vp.setNode(nullptr);
    setViewpoint(vp);

    auto camSet = getSettings();
    camSet->setMinMaxDistance(0,MAX_CAM_DISTANCE);
    applySettings(camSet);
}

bool CameraController::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
    if(ea.getEventType() == osgGA::GUIEventAdapter::PUSH){
        eventHandled = ea.getHandled();
    }
    return osgEarth::Util::EarthManipulator::handle(ea, us);
}

bool CameraController::getEventHandled() const
{
    return eventHandled;
}
