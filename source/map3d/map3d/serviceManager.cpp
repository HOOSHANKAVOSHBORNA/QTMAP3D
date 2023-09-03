#include "serviceManager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ServiceManager::ServiceManager(QObject *parent): QObject(parent)
{

}

void ServiceManager::initLayers(QJsonDocument *layersJson)
{
    QJsonObject data = layersJson->object();
    for (auto it : data)
        parseLayersFromJson(it.toObject());
}

void ServiceManager::addFlyableModel(QJsonDocument *flyable)
{
    QJsonObject data = flyable->object();
    double x = data.value("x").toDouble();
    double y = data.value("y").toDouble();
    double z = data.value("z").toDouble();
//    osgEarth::GeoPoint geopos(mMapItem->getMapSRS(), x, y, z);
//    ServiceFlyableModel *model = new ServiceFlyableModel(data.value("Id").toInt()
//                                    , data.value("Name").toString().toStdString(), data.value("Url2d").toString().toStdString()
//                                    ,  data.value("Url3d").toString().toStdString(), data.value("Color").toString().toStdString()
//                                    , geopos, data.value("Speed").toDouble());
//    emit flyableAdded(model, layers[layerId].get());
}

void ServiceManager::parseLayersFromJson(QJsonObject obj, CompositeAnnotationLayer *parent)
{
//    if (obj.value("childs").toArray().size() > 0){
//        osg::ref_ptr<CompositeAnnotationLayer> comp = new CompositeAnnotationLayer();
//        comp->setName(obj.value("text").toString().toStdString());
//        mMapItem->getMapObject()->addLayer(comp);
//        unsigned int order = obj.value("order").toInt();
//        if (parent)
//            parent->insertLayer(comp, order);
//        layers[obj.value("id").toInt()] = comp;
//        for (auto it: obj.value("childs").toArray()) {
//            parseLayersFromJson(it.toObject(), comp);
//        }
//    }
//    else {
//        osg::ref_ptr<ParenticAnnotationLayer> parentic = new ParenticAnnotationLayer();
//        parentic->setName(obj.value("text").toString().toStdString());
//        unsigned int order = obj.value("order").toInt();
//        if (parent)
//            parent->insertLayer(parentic, order);
//        layers[obj.value("id").toInt()] = parentic;
//        return;
//    }
}

