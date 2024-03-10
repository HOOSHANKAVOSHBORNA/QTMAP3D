#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <osgEarthUtil/EarthManipulator>

#define MAX_CAM_DISTANCE 30000000.0
class MapItem;
class CameraController: public osgEarth::Util::EarthManipulator
{
public:
    CameraController(MapItem* mapItem);
    ~CameraController();

    virtual void home(double /*unused*/) override;

    void zoom(double delta);
    virtual void zoom( double dx, double dy, osg::View* view ) override;

    void goToPosition(double lat,
                      double lon,
                      double range,
                      double duration = 0);

    void goToPosition(double lat,
                      double lon,
                      double range,
                      double pitch,
                      double heading,
                      double duration = 0);

    void goToPosition(osgEarth::GeoPoint mapPoint,
                      double range,
                      double duration = 3.0);

    void setHeadingToNorth();

    void setTrackNode(osg::Node *node, double minDistance);
    void untrack();
//    void goToHome();
//    void goToPosition(double latitude, double longitude, double range);
//    void goToPosition(osgEarth::GeoPoint mapPoint, double range, double duration = 3.0);
//    void pan(double xVal, double yVal);
//    void rotate(double xVal, double yVal);
//    void travelToViewpoint(double latitude,
//                           double longitude,
//                           double range,
//                           double pitch,
//                           double heading);

//    void orientCameraToNorth();

//    void setTrackNode(osg::Node *node, double minDistance);
//    //    void untrackNode(osg::Node *node);
//    void untrack();
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us) override;
    bool getEventHandled() const;

private:
    MapItem* mMapItem{nullptr};
    bool eventHandled{false};
};

#endif // CAMERACONTROLLER_H
