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
    void mouseEvent(QMouseEvent* event, const osgEarth::GeoPoint& geoPos);

public:
    osgViewer::Viewer *getViewer();
    osgEarth::Util::EarthManipulator *getEarthManipulator();
    osgEarth::Viewpoint getViewpoint() const;
    osgEarth::MapNode *getMapNode();
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


private:
    explicit MapController(QQuickWindow *window);
    ~MapController();


    virtual void installEventHandler() override;
    void mapMouseEvent(QMouseEvent *event, const osg::Vec3d& worldPos);
};

#endif // MapController_H
