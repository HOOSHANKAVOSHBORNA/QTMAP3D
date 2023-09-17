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
    flyableModel->id = data.value("Id").toInt();
    flyableModel->longitude =  data.value("Longitude").toDouble();
    flyableModel->latitude = data.value("Latitude").toDouble();
    flyableModel->altitude = data.value("Altitude").toDouble();
    flyableModel->name = data.value("Name").toString().toStdString();
    flyableModel->url2D = data.value("Url2d").toString().toStdString();
    flyableModel->url3D = data.value("Url3d").toString().toStdString();
    flyableModel->color = data.value("Color").toString().toStdString();
    flyableModel->speed = data.value("Speed").toInt();
    for (auto i : data.value("LayersId").toArray()){
        int id = i.toInt();
        if(mParenticLayerMap.contains(id))
            flyableModel->layerList.push_back(mParenticLayerMap[id]);
        else
            qDebug()<<"Can not found layer: "<<id;
    }
    if(flyableModel->layerList.size() > 0)
        emit flyableAdded(flyableModel);
}

void ServiceManager::parseLayersFromJson(QJsonObject obj, CompositeAnnotationLayer *parent)
{
    if (obj.value("Children").toArray().size() > 0){
        CompositeAnnotationLayer* comp = new CompositeAnnotationLayer(obj.value("Id").toInt());
        comp->setName(obj.value("Text").toString().toStdString());
        comp->setOrder(obj.value("Order").toInt());
//        qDebug()<<"composite:"<<comp->getName();
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
        int layerId = obj.value("Id").toInt();
        if(!mParenticLayerMap.contains(layerId)){
            ParenticAnnotationLayer* parentic = new ParenticAnnotationLayer(layerId);
            parentic->setName(obj.value("Text").toString().toStdString());
            mParenticLayerMap[layerId] = parentic;
//            qDebug()<<"composite parent: "<<parent->getName();
//            qDebug()<<"parentic: "<<parentic->getName();
        }
        parent->addLayer(mParenticLayerMap[layerId]);

//        emit layerAdded(parentic, obj.value("Id").toInt(), obj.value("ParentId").toInt(), obj.value("Order").toInt());
        return;
    }
}

