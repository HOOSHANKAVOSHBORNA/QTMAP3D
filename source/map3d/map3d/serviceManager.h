#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "compositeAnnotationLayer.h"
#include "mapItem.h"
#include <QJsonDocument>
#include <osgEarth/Layer>

struct ServiseModel: public osg::Referenced {
    std::string name;
    std::string modelUrl2D;
    std::string modelUrl3D;
    std::string iconUrl;
    osgEarth::GeoPoint position;
    ServiseModel(std::string modelUrl2D, std::string modelUrl3D, std::string iconUrl, osgEarth::GeoPoint position)
        : modelUrl2D{modelUrl2D}, modelUrl3D{modelUrl3D}, iconUrl{iconUrl}, position{position} {}
};

class ServiceManager: public QObject
{
    Q_OBJECT
public:
    ServiceManager(MapItem *mapItem, QObject *parent = nullptr);

    void initLayers(QJsonDocument *layersJson);
    void addFlyableModel(QJsonDocument *flyable, int layerId);
    void addMovableModel(QJsonDocument *movable);
    void addModel(QJsonDocument *model);

    void addPolygon(QJsonDocument *polygon);
    void addSphere(QJsonDocument *sphere);
    void addCircle(QJsonDocument *circle);
    void addPolyline(QJsonDocument *polyline);

signals:
    void flyableAdded(ServiseModel *model, ParenticAnnotationLayer* parentic);
private:
    void parseLayersFromJson(QJsonObject obj, CompositeAnnotationLayer *parent=nullptr);
private:
    QMap<int, osg::ref_ptr<ParenticAnnotationLayer>> layers;
    MapItem *mMapItem{nullptr};
};

#endif // DATAMANAGER_H
