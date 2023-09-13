#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "compositeAnnotationLayer.h"
#include "mapItem.h"
#include <QJsonDocument>
#include <osgEarth/Layer>

struct ServiseFixedModel: public osg::Referenced
{
    int id;
    std::string name;
    std::string url2D;
    std::string url3D;
    std::string color;
    double latitude;
    double longitude;
    double altitude;
    std::vector<int> layersId;
    ServiseFixedModel()
        : osg::Referenced() {}
};

struct ServiceMovableModel: public ServiseFixedModel
{
    ServiceMovableModel():
        ServiseFixedModel(){}
};

struct  ServiceFlyableModel: public ServiceMovableModel
{
    ServiceFlyableModel()
        : ServiceMovableModel(){}

    double speed;
};

struct ServiceLayer {
    int id;
    int parentId;
    std::string text;
    int order;
    bool parentic;
    ServiceLayer(int id, int parent, std::string text, int order, bool parentic):id{id}, parentId{parent}, text{text}, order{order}, parentic{parentic} {}

};

class ServiceManager: public QObject
{
    Q_OBJECT
public:
    ServiceManager(MapItem *mapItem, QObject *parent = nullptr);

    void initLayers(std::string layersStr);
    void addFlyableModel(std::string flyable);
    void addMovableModel(QJsonDocument *movable);
    void addModel(QJsonDocument *model);

    void addPolygon(QJsonDocument *polygon);
    void addSphere(QJsonDocument *sphere);
    void addCircle(QJsonDocument *circle);
    void addPolyline(QJsonDocument *polyline);

signals:
    void layerAdded(CompositeAnnotationLayer *layer);
    void flyableAdded(ServiceFlyableModel *model);
private:
    void parseLayersFromJson(QJsonObject obj, CompositeAnnotationLayer *parent = nullptr);

private:
    MapItem *mMapItem{nullptr};
};

#endif // DATAMANAGER_H
