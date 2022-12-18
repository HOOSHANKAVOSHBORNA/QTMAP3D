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
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgEarthUtil/Sky>

#include "osgrenderer.h"


class MainEventHandler;


class MapController : public QObject
{
    friend class MainWindow;
    friend class MainEventHandler;
    Q_OBJECT

signals:
    void headingAngleChanged(qreal angle);
    void mouseEvent(QMouseEvent* event, osgEarth::GeoPoint geoPos, osg::Vec3d worldPos);
    void mousePointingLocationWgs84Changed(QVector3D location);
    void mousePointingLocationChanged(QVector3D location);

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
    void goToPosition(osgEarth::GeoPoint mapPoint, double range);
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


    void installEventHandler();
    void mapMouseEvent(QMouseEvent *event, const osg::Vec3d& worldPos);


public:

    void cleanup();

    void initializeGL(int width, int height, QScreen *screen, GLuint renderTargetId);
    void resizeGL(int width, int height, QScreen *screen);
    void paintGL();


    void keyPressEvent(QKeyEvent* event); void keyReleaseEvent(QKeyEvent* event);  void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    void createOsgRenderer(int width, int height, QScreen *screen);
    void initializeOsgEarth();
    void createMapNode(bool bGeocentric);
    void createCameraManipulator();

protected:
    osg::ref_ptr<osgEarth::MapNode> mMapNode;
    osg::ref_ptr<osgEarth::Util::SkyNode> mSkyNode;
    osg::ref_ptr<osg::Group> mMapRoot;

protected:
    OSGRenderer *mpOsgRenderer = nullptr;
    bool misFirstFrame = true;
    GLuint mrenderTargetId = 0;
    QQuickWindow *mpWindow = nullptr;

    osgEarth::Util::EarthManipulator *mpEarthManipulator = nullptr;

    bool mbGeocentric = true;


};

#endif // MapController_H
