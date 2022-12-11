#ifndef MapController_H
#define MapController_H

#include <QQuickWindow>
#include <QObject>
#include <QEvent>
#include <QScreen>
#include <GL/gl.h>
#include <osgEarth/MapNode>
#include <osgEarth/Map>
#include <osgEarthUtil/EarthManipulator>

#include "osgrenderer.h"
#include "osgcontroller.h"


class MainEventHandler;


class MapController : public OsgController
{
    friend class OsgQuickWindow;
    friend class MainEventHandler;
    Q_OBJECT

signals:
    void headingAngleChanged(qreal angle);
    void mouseEvent(QMouseEvent* event, osgEarth::GeoPoint geoPos, osg::Vec3d worldPos);
    void mousePointedLocationChanged(QVector3D location);

    void focalPointLatChanged  (qreal focalPointLat);
    void focalPointLongChanged (qreal focalPointLong);
    void focalPointRangeChanged(qreal focalPointRange);
    void focalPointPitchChanged(qreal focalPointPitch);
    void focalPointHeadChanged(qreal focalPointHead);

    void fpsChanged(qreal fps);

public:
    osgViewer::Viewer *getViewer();
    osgEarth::Util::EarthManipulator *getEarthManipulator();
    osgEarth::Viewpoint getViewpoint() const;
    osgEarth::MapNode *getMapNode();
    osg::ref_ptr<osg::Group> getRoot() const;
    const osgEarth::SpatialReference* getMapSRS() const;

public:
    void setMap(osgEarth::Map *map);
    void setTrackNode(osg::Node *node);
    void untrackNode();
    bool addNode(osg::Node *node);
    bool removeNode(osg::Node *node);
    void setViewpoint(const osgEarth::Viewpoint& vp, double duration_s = 0.0);
    void addLayer(osgEarth::Layer* layer);

public slots:
    void setZoom(double);
    void goToHome();
    void goToPosition(double latitude, double longitude, double range);
    void setGeocentric(bool bGeocentric);
    void toggleProjection();
    void frame();

    void panUp();
    void panDown();
    void panLeft();
    void panRight();

    void rotateUp();
    void rotateDown();
    void rotateLeft();
    void rotateRight();

    void zoomIn();
    void zoomOut();

    void travelToViewpoint(qreal latitude,
                           qreal longitude,
                           qreal range,
                           qreal pitch,
                           qreal heading);


private:
    explicit MapController(QQuickWindow *window);
    ~MapController();


    virtual void installEventHandler() override;
    void mapMouseEvent(QMouseEvent *event, const osg::Vec3d& worldPos);
};

#endif // MapController_H
