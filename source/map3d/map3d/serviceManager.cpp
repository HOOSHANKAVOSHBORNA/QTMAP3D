#include "serviceManager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ServiceManager::ServiceManager(MapItem *mapItem, QObject *parent):
    QObject(parent),
    mMapItem{mapItem}
{

}

void ServiceManager::initLayers(std::string layersStr)
{
    QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(layersStr).toUtf8());

    QJsonObject data = doc.object();
    for (auto it : data)
        parseLayersFromJson(it.toObject());
}

void ServiceManager::addFlyableModel(std::string flyable)
{
    QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(flyable).toUtf8());
    QJsonObject data = doc.object();

    ServiceFlyableModel* flyableModel = new ServiceFlyableModel();
    flyableModel->longitude =  data.value("Longitude").toDouble();
    flyableModel->latitude = data.value("Latitude").toDouble();
    flyableModel->altitude = data.value("Altitude").toDouble();
    flyableModel->name = data.value("Name").toString().toStdString();
    flyableModel->url2D = data.value("Url2d").toString().toStdString();
    flyableModel->url3D = data.value("Url3d").toString().toStdString();
    flyableModel->color = data.value("Color").toString().toStdString();
    flyableModel->speed = data.value("Speed").toInt();
    for (auto i : data.value("LayersId").toArray())
        flyableModel->layersId.push_back(i.toInt());

    emit flyableAdded(flyableModel);
}

void ServiceManager::parseLayersFromJson(QJsonObject obj, CompositeAnnotationLayer *parent)
{
    if (obj.value("Children").toArray().size() > 0){
        CompositeAnnotationLayer* comp = new CompositeAnnotationLayer(obj.value("Id").toInt());
        comp->setName(obj.value("Text").toString().toStdString());
        comp->setOrder(obj.value("Order").toInt());
        if (parent){
            parent->addLayer(comp);
        }
        else
            emit layerAdded(comp);

        for (auto it: obj.value("Children").toArray()) {
            parseLayersFromJson(it.toObject(), comp);
        }
    }
    else {
        ParenticAnnotationLayer* parentic = new ParenticAnnotationLayer(obj.value("Id").toInt());
        parentic->setName(obj.value("Text").toString().toStdString());
        parent->addLayer(parentic);
//        emit layerAdded(parentic, obj.value("Id").toInt(), obj.value("ParentId").toInt(), obj.value("Order").toInt());
        return;
    }
}

