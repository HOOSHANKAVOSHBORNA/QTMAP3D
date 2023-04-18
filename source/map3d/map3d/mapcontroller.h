#ifndef MapController_H
#define MapController_H

#include "osgrenderer.h"
//#include "layersmodel.h"

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

class MainMapCallback;

class MapController : public QObject
{
    Q_OBJECT
public:
    void setMap(osgEarth::Map *map);
    osgViewer::Viewer *getViewer()const;
    const osg::Group *getRoot() const;
    osgEarth::MapNode *getMapNode() const;
    const osgEarth::SpatialReference* getMapSRS() const;
    void addLayer(osgEarth::Layer* layer);
//    LayersModel *getLayersModel() const;
    bool addNode(osg::Node *node);
    bool removeNode(osg::Node *node);

public://camera functions
    osgEarth::Util::EarthManipulator *getEarthManipulator() const;
    void setViewpoint(const osgEarth::Viewpoint& vp, double duration_s = 0.0);
    osgEarth::Viewpoint getViewpoint() const;
    void setTrackNode(osg::Node *node, double minDistance = 400);
    void untrackNode(osg::Node *node);

public:
    void screenToWorld(float x, float y, osg::Vec3d& outWorldPoint ) const;
    osgEarth::GeoPoint screenToGeoPoint(float x, float y) const;
    void worldToScreen(osg::Vec3d worldPoint, float& outX, float& outY) const;

public slots:
    void zoom(double);
    void goToHome();
    void goToPosition(double latitude, double longitude, double range);
    void goToPosition(osgEarth::GeoPoint mapPoint, double range, double duration = 3.0);
    void setMode(bool is3DView);
    bool getMode() const;
    void setGeocentric(bool isGeocentric);
    void toggle3DView();
    void pan(double xVal, double yVal);
    void rotate(double xVal, double yVal);
    void travelToViewpoint(qreal latitude,
                           qreal longitude,
                           qreal range,
                           qreal pitch,
                           qreal heading);

    void orientCameraToNorth();

signals:
    void headingAngleChanged(qreal angle);
    //void mouseEvent(QMouseEvent* event, osgEarth::GeoPoint geoPos, osg::Vec3d worldPos);
    void mousePointingLocationWgs84Changed(QVector3D location);
    void mousePointingLocationChanged(QVector3D location);

    void focalPointLatChanged  (qreal focalPointLat);
    void focalPointLongChanged (qreal focalPointLong);
    void focalPointRangeChanged(qreal focalPointRange);
    void focalPointPitchChanged(qreal focalPointPitch);
    void focalPointHeadChanged(qreal focalPointHead);

    void modeChanged(bool is3DView);
    void layerChanged();
    void mapCleared();
//    void mapSRSChanged();

private:
    explicit MapController(QQuickWindow *window);
    ~MapController();

    void mapMouseLocation(osgEarth::GeoPoint geoPos);

    void initializeOsgEarth();
    void createMapNode(bool geocentric, osgEarth::Map *map = nullptr);
    void createCameraManipulator();

    void layerAdded(osgEarth::Layer* layer, unsigned index);
    void layerRemoved(osgEarth::Layer* layer, unsigned index);
    void frame();

    friend class MainWindow;
    friend class MainMapCallback;
private:
    osg::ref_ptr<osgEarth::MapNode> mMapNode{nullptr};
    osg::ref_ptr<osgEarth::Util::SkyNode> mSkyNode{nullptr};
    osg::ref_ptr<osg::Group> mMapRoot{nullptr};

private:
    bool mIsFirstFrame{true};
    GLuint mRenderTargetId{0};
    osgEarth::Util::EarthManipulator *mEarthManipulator{nullptr};
    bool mIsGeocentric{true};
    bool mIs3DView{true};
//--renderer------------------------------------------------------------------------------------------------------
public:
    void cleanup();
    void initializeGL(int width, int height, QScreen *screen, GLuint renderTargetId);
    void resizeGL(int width, int height, QScreen *screen);
    void paintGL();
    void createOsgRenderer(int width, int height, QScreen *screen);
    //events------------------------------
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
private:
    OSGRenderer *mOsgRenderer{nullptr};
    QQuickWindow *mWindow{nullptr};
};

#endif // MapController_H
