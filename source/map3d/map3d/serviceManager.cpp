#include "serviceManager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ServiceManager::ServiceManager(QObject *parent): QObject(parent)
{

}

void ServiceManager::initLayers(QString layersStr)
{
    QJsonDocument doc = QJsonDocument::fromJson(layersStr.toUtf8());

    QJsonObject data = doc.object();
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
    if (obj.value("Children").toArray().size() > 0){
        CompositeAnnotationLayer* comp = new CompositeAnnotationLayer(nullptr, obj.value("Id").toInt());
        comp->setName(obj.value("Text").toString().toStdString());
        comp->setOrder(obj.value("Order").toInt());
        if (parent){
            parent->addLayer(comp);
        }
        else
            emit layerAdded(comp, obj.value("Id").toInt());

        for (auto it: obj.value("Children").toArray()) {
            parseLayersFromJson(it.toObject(), comp);
        }
    }
    else {
        ParenticAnnotationLayer* parentic = new ParenticAnnotationLayer(nullptr, obj.value("Id").toInt());
        parentic->setName(obj.value("Text").toString().toStdString());
        parent->addLayer(parentic);
//        emit layerAdded(parentic, obj.value("Id").toInt(), obj.value("ParentId").toInt(), obj.value("Order").toInt());
        return;
    }
}

