#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "compositeAnnotationLayer.h"
#include <QJsonDocument>
#include <osgEarth/Layer>

struct ServiseFixedModel: public osg::Referenced
{
    int id;
    std::string name;
    std::string modelUrl2D;
    std::string modelUrl3D;
    std::string color;
    osgEarth::GeoPoint position;
    ServiseFixedModel(int id, std::string name, std::string modelUrl2D, std::string modelUrl3D, std::string color, osgEarth::GeoPoint position)
        : osg::Referenced(), id{id}, name{name}, modelUrl2D{modelUrl2D}, modelUrl3D{modelUrl3D}, color{color}, position{position} {}
    ServiseFixedModel(): osg::Referenced(){}
};

struct ServiceMovableModel: public ServiseFixedModel
{
    ServiceMovableModel(int id, std::string name, std::string modelUrl2D, std::string modelUrl3D, std::string color, osgEarth::GeoPoint position):
        ServiseFixedModel(id, name, modelUrl2D, modelUrl3D, color, position){}
};

struct  ServiceFlyableModel: public ServiceMovableModel
{
    ServiceFlyableModel(int id, std::string name, std::string modelUrl2D, std::string modelUrl3D, std::string color, osgEarth::GeoPoint position, double speed)
        : ServiceMovableModel(id, name, modelUrl2D, modelUrl3D, color, position){}

    double speed;
};

struct ServiceLayer {

};

class ServiceManager: public QObject
{
    Q_OBJECT
public:
    ServiceManager(QObject *parent = nullptr);

    void initLayers(QJsonDocument *layersJson);
    void addFlyableModel(QJsonDocument *flyable);
    void addMovableModel(QJsonDocument *movable);
    void addModel(QJsonDocument *model);

    void addPolygon(QJsonDocument *polygon);
    void addSphere(QJsonDocument *sphere);
    void addCircle(QJsonDocument *circle);
    void addPolyline(QJsonDocument *polyline);

signals:
//    void flyableAdded(ServiseModel *model, ParenticAnnotationLayer* parentic);
private:
    void parseLayersFromJson(QJsonObject obj, CompositeAnnotationLayer *parent=nullptr);
};

#endif // DATAMANAGER_H
