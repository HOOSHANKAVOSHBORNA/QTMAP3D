#ifndef MapController_H
#define MapController_H

#include "osgRenderNode.h"
//#include "layersmodel.h"
#include <QQuickItem>
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

class MapItem : public QQuickItem
{
    Q_OBJECT

public:
    explicit MapItem(QQuickItem *parent = nullptr);
    ~MapItem();
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
	void setTrackNode(osg::Node *node, double minDistance);
//    void untrackNode(osg::Node *node);
	void untrack();
    qreal headingAngle() const;
public:
    void screenToWorld(float x, float y, osg::Vec3d& outWorldPoint ) const;
    osgEarth::GeoPoint screenToGeoPoint(float x, float y) const;
    void worldToScreen(osg::Vec3d worldPoint, float& outX, float& outY) const;
    bool addNodeToLayer(osg::Node *node, std::string layerName);
    bool removeNodeFromLayer(osg::Node *node, std::string layerName);

    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;
public slots:
    void zoom(double);
    void goToHome();
    void goToPosition(double latitude, double longitude, double range);
    void goToPosition(osgEarth::GeoPoint mapPoint, double range, double duration = 3.0);
    void changeMode();
    bool getMode() const;
    void setGeocentric(bool isGeocentric);
//    void toggle3DView();
    void pan(double xVal, double yVal);
    void rotate(double xVal, double yVal);
    void travelToViewpoint(qreal latitude,
                           qreal longitude,
                           qreal range,
                           qreal pitch,
                           qreal heading);

    void orientCameraToNorth();

signals:
    void focalPointLatChanged  (qreal focalPointLat);
    void focalPointLongChanged (qreal focalPointLong);
    void focalPointRangeChanged(qreal focalPointRange);
    void focalPointPitchChanged(qreal focalPointPitch);
    void focalPointHeadChanged(qreal focalPointHead);

    void modeChanged(bool is3DView);
    void layerChanged();
    void mapCleared();
    void frameChanged();
//    void mapSRSChanged();

private:
    void initializeOsgEarth();
    void createMapNode(bool geocentric, osgEarth::Map *map = nullptr);
    void createCameraManipulator();

    void layerAdded(osgEarth::Layer* layer, unsigned index);
    void layerRemoved(osgEarth::Layer* layer, unsigned index);

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
//    void cleanup();
//    void initializeGL(int width, int height, QScreen *screen, GLuint renderTargetId);
//    void resizeGL(int width, int height, QScreen *screen);
//    void paintGL();
//    void createOsgRenderer();
    virtual void frame();
    //events------------------------------
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

private:
    OSGRenderNode *mOSGRenderNode{nullptr};
//    QQuickWindow *mWindow{nullptr};
};

#endif // MapController_H
