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

    Q_PROPERTY(qreal headingAngle READ headingAngle NOTIFY headingAngleChanged)

    Q_PROPERTY(bool zoomInButtonPressed       WRITE setZoomInButtonPressed      )
    Q_PROPERTY(bool zoomOutButtonPressed      WRITE setZoomOutButtonPressed     )
    Q_PROPERTY(bool upButtonPressed           WRITE setUpButtonPressed          )
    Q_PROPERTY(bool downButtonPressed         WRITE setdownButtonPressed        )
    Q_PROPERTY(bool leftButtonPressed         WRITE setleftButtonPressed        )
    Q_PROPERTY(bool rightButtonPressed        WRITE setrightButtonPressed       )
    Q_PROPERTY(bool rotateUpButtonPressed     WRITE setrotateUpButtonPressed    )
    Q_PROPERTY(bool rotateDownButtonPressed   WRITE setrotateDownButtonPressed  )
    Q_PROPERTY(bool rotateLeftButtonPressed   WRITE setrotateLeftButtonPressed  )
    Q_PROPERTY(bool rotateRightButtonPressed  WRITE setrotateRightButtonPressed )

    Q_PROPERTY(QVector3D mapMouseGeoLocation READ mapMouseGeoLocation NOTIFY mouseLocationChanged)
    Q_PROPERTY(QVector3D mapMouseLocation READ mapMouseLocation NOTIFY mouseLocationChanged)

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
    QVector3D mapMouseGeoLocation() const;
    QVector3D mapMouseLocation() const;
//
public slots:
    void setZoomInButtonPressed(bool pressed);
    void setZoomOutButtonPressed(bool pressed);
    void setUpButtonPressed(bool pressed);
    void setdownButtonPressed(bool pressed);
    void setleftButtonPressed(bool pressed);
    void setrightButtonPressed(bool pressed);
    void setrotateUpButtonPressed(bool pressed);
    void setrotateDownButtonPressed(bool pressed);
    void setrotateLeftButtonPressed(bool pressed);
    void setrotateRightButtonPressed(bool pressed);
signals:
    void headingAngleChanged();
    //void mouseEvent(QMouseEvent* event, osgEarth::GeoPoint geoPos, osg::Vec3d worldPos);
    void mouseLocationChanged();
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
    void tickNavigation(double deltaTime);

//    void mapMouseLocation(osgEarth::GeoPoint geoPos);

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
    osgEarth::GeoPoint mCurrentMouseGeoPoint;

private:
    bool mZoomInButtonPressed{false};
    bool mZoomOutButtonPressed{false};
    bool mUpButtonPressed{false};
    bool mDownButtonPressed{false};
    bool mLeftButtonPressed{false};
    bool mRightButtonPressed{false};
    bool mRotateUpButtonPressed{false};
    bool mRotateDownButtonPressed{false};
    bool mRotateLeftButtonPressed{false};
    bool mRotateRightButtonPressed{false};
//--renderer------------------------------------------------------------------------------------------------------
public:
//    void cleanup();
//    void initializeGL(int width, int height, QScreen *screen, GLuint renderTargetId);
//    void resizeGL(int width, int height, QScreen *screen);
//    void paintGL();
//    void createOsgRenderer();
    void frame();
    //events------------------------------
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
private:
    OSGRenderNode *mOSGRenderNode{nullptr};
//    QQuickWindow *mWindow{nullptr};
};

#endif // MapController_H
