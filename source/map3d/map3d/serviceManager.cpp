#include "serviceManager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ServiceManager::ServiceManager(MapItem *mapItem, QObject *parent): mMapItem(mapItem), QObject(parent)
{

}

void ServiceManager::initLayers(QJsonDocument *layersJson)
{
    QJsonObject data = layersJson->object();
    for (auto it : data)
        parseLayersFromJson(it.toObject());
}

void ServiceManager::addFlyableModel(QJsonDocument *flyable, int layerId)
{
    QJsonObject data = flyable->object();
    double x = data.value("x").toDouble();
    double y = data.value("y").toDouble();
    double z = data.value("z").toDouble();
    osgEarth::GeoPoint geopos(mMapItem->screenToGeoPoint(600, 300));
    geopos.z() = 0;
    ServiseModel *model = new ServiseModel(data.value("modelUrl2D").toString().toStdString(),data.value("modelUrl3D").toString().toStdString()
                                           , data.value("iconUrl").toString().toStdString(), geopos);
    emit flyableAdded(model, layers[layerId].get());
}

void ServiceManager::parseLayersFromJson(QJsonObject obj, CompositeAnnotationLayer *parent)
{
    if (obj.value("childs").toArray().size() > 0){
        osg::ref_ptr<CompositeAnnotationLayer> comp = new CompositeAnnotationLayer();
        comp->setName(obj.value("text").toString().toStdString());
        mMapItem->getMapObject()->addLayer(comp);
        unsigned int order = obj.value("order").toInt();
        if (parent)
            parent->insertLayer(comp, order);
        layers[obj.value("id").toInt()] = comp;
        for (auto it: obj.value("childs").toArray()) {
            parseLayersFromJson(it.toObject(), comp);
        }
    }
    else {
        osg::ref_ptr<ParenticAnnotationLayer> parentic = new ParenticAnnotationLayer();
        parentic->setName(obj.value("text").toString().toStdString());
        unsigned int order = obj.value("order").toInt();
        if (parent)
            parent->insertLayer(parentic, order);
        layers[obj.value("id").toInt()] = parentic;
        return;
    }
}

