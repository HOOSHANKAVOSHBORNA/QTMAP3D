#include "serviceManager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ServiceManager::ServiceManager(MapItem *mapItem, QObject *parent):
    QObject(parent),
    mMapItem{mapItem}
{

}

void ServiceManager::layersData(std::string jsonData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(jsonData).toUtf8());

    QJsonObject jsonObject = jsonDoc.object();
    for (auto it : jsonObject)
        parseLayersFromJson(it.toObject());
}

void ServiceManager::flyableNodeData(std::string jsonData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(jsonData).toUtf8());
    QJsonObject jsonObject = jsonDoc.object();

    NodeData* flyableNodeData = new NodeData();
    flyableNodeData->id = jsonObject.value("Id").toInt();
    flyableNodeData->longitude =  jsonObject.value("Longitude").toDouble();
    flyableNodeData->latitude = jsonObject.value("Latitude").toDouble();
    flyableNodeData->altitude = jsonObject.value("Altitude").toDouble();
    flyableNodeData->name = jsonObject.value("Name").toString().toStdString();
    flyableNodeData->url2D = jsonObject.value("Url2d").toString().toStdString();
    flyableNodeData->url3D = jsonObject.value("Url3d").toString().toStdString();
    flyableNodeData->color = jsonObject.value("Color").toString().toStdString();
    flyableNodeData->speed = jsonObject.value("Speed").toInt();
    for (auto i : jsonObject.value("LayersId").toArray()){
        int id = i.toInt();
        if(mParenticLayerMap.contains(id))
            flyableNodeData->layers.push_back(mParenticLayerMap[id]);
        else
            qDebug()<<"Can not found layer: "<<id;
    }
    if(flyableNodeData->layers.size() > 0)
        emit flyableNodeDataReceived(flyableNodeData);
}

void ServiceManager::statusNodeData(std::string jsonData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(jsonData).toUtf8());
    QJsonObject jsonObject = jsonDoc.object();
    StatusNodeData *statusNodeData = new StatusNodeData;

    statusNodeData->id = jsonObject.value("Id").toInt();
    statusNodeData->name = jsonObject.value("Name").toString().toStdString();
    statusNodeData->longitude =  jsonObject.value("Longitude").toDouble();
    statusNodeData->latitude = jsonObject.value("Latitude").toDouble();
    statusNodeData->altitude = jsonObject.value("Altitude").toDouble();
    int layerId = jsonObject.value("LayerId").toInt();
    if(mParenticLayerMap.contains(layerId))
        statusNodeData->layer = mParenticLayerMap[layerId];
    else
        qDebug()<<"Can not found layer: "<<layerId;

    for(auto& key:jsonObject.keys())
        statusNodeData->data.push_back(NodeFieldData{key, jsonObject.value(key)});

    if(statusNodeData->layer)
        emit statusNodeDataReceived(statusNodeData);
}

void ServiceManager::parseLayersFromJson(QJsonObject jsonObject, CompositeAnnotationLayer *parent)
{
    if (jsonObject.value("Children").toArray().size() > 0){
        CompositeAnnotationLayer* comp = new CompositeAnnotationLayer(jsonObject.value("Id").toInt());
        comp->setName(jsonObject.value("Text").toString().toStdString());
        comp->setOrder(jsonObject.value("Order").toInt());
//        qDebug()<<"composite:"<<comp->getName();
        if (parent){
            parent->addLayer(comp);
        }
        else
            emit layerDataReceived(comp);

        for (auto it: jsonObject.value("Children").toArray()) {
            parseLayersFromJson(it.toObject(), comp);
        }
    }
    else {
        int layerId = jsonObject.value("Id").toInt();
        if(!mParenticLayerMap.contains(layerId)){
            ParenticAnnotationLayer* parentic = new ParenticAnnotationLayer(layerId);
            parentic->setName(jsonObject.value("Text").toString().toStdString());
            mParenticLayerMap[layerId] = parentic;
//            qDebug()<<"composite parent: "<<parent->getName();
//            qDebug()<<"parentic: "<<parentic->getName();
        }
        parent->addLayer(mParenticLayerMap[layerId]);

//        emit layerAdded(parentic, obj.value("Id").toInt(), obj.value("ParentId").toInt(), obj.value("Order").toInt());
        return;
    }
}

