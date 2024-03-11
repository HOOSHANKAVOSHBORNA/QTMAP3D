#ifndef MapController_H
#define MapController_H

#include "osgViewerItem.h"
#include "mapObject.h"
#include "cameraController.h"

#include <osgEarth/MapNode>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgEarthUtil/Sky>
#include <osgEarthAnnotation/AnnotationLayer>

class MapItem : public OsgViewerItem
{
    Q_OBJECT

public:
    explicit MapItem(QQuickItem *parent = nullptr);
    ~MapItem();
    void setMap(osgEarth::Map *map);
    const osg::Group *getRoot() const;
    osgEarth::MapNode *getMapNode() const;

    MapObject *getMapObject();
    const MapObject* getMapObject() const;

    Q_INVOKABLE CameraController *getCameraController();
    const CameraController *getCameraController() const;

    const osgEarth::SpatialReference* getMapSRS() const;

    bool addNode(osg::Node *node);
    bool removeNode(osg::Node *node);

public:
    void screenToWorld(float x, float y, osg::Vec3d& outWorldPoint ) const;
    osgEarth::GeoPoint screenToGeoPoint(float x, float y) const;
    void worldToScreen(osg::Vec3d worldPoint, float& outX, float& outY) const;
    void worldToOSGScreen(osg::Vec3d worldPoint, float& outX, float& outY) const;
    void addBaselayers();
    bool getMode() const;
public slots:
    void changeMode();

    void setGeocentric(bool isGeocentric);

signals:
    void modeChanged(bool is3DView);
    void mapCleared();
protected:
    virtual void initializeOsgEarth();
private:
    void createMapNode(bool geocentric, osgEarth::Map *map = nullptr);
    void createCameraManipulator();


    friend class MainWindow;
    friend class MainMapCallback;
private:
    osg::ref_ptr<osgEarth::MapNode> mMapNode{nullptr};
    osg::ref_ptr<osgEarth::Util::SkyNode> mSkyNode{nullptr};
    osg::ref_ptr<osg::Group> mMapRoot{nullptr};
    osg::ref_ptr<MapObject> mMapObject;

private:
    osg::ref_ptr<CameraController> mCameraController{nullptr};
    bool mIsGeocentric{true};
    bool mIs3DView{true};
public:
    virtual void frame() override;
};

#endif // MapController_H
