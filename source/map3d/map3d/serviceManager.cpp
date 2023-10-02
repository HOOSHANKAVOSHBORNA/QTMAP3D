#include "serviceManager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ServiceManager::ServiceManager(MapItem *mapItem, QObject *parent):
    QObject(parent),
    mMapItem{mapItem}
{

}

void ServiceManager::layersData(QJsonObject jsonObject)
{
    for (auto it : jsonObject)
        parseLayersFromJson(it.toObject());
}

void ServiceManager::flyableNodeData(QJsonObject jsonObject)
{
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
        auto layer = findParenticLayer(id);
        if(layer)
            flyableNodeData->layers.push_back(layer);
    }
    if(flyableNodeData->layers.size() > 0)
        emit flyableNodeDataReceived(flyableNodeData);
}

void ServiceManager::statusNodeData(QJsonObject jsonObject)
{
    StatusNodeData *statusNodeData = new StatusNodeData;

    statusNodeData->id = jsonObject.value("Id").toInt();
    statusNodeData->name = jsonObject.value("Name").toString().toStdString();
    statusNodeData->longitude =  jsonObject.value("Longitude").toDouble();
    statusNodeData->latitude = jsonObject.value("Latitude").toDouble();
    statusNodeData->altitude = jsonObject.value("Altitude").toDouble();
    int layerId = jsonObject.value("LayerId").toInt();
    auto layer = findParenticLayer(layerId);
    if(layer)
        statusNodeData->layer = layer;

    for(auto& key:jsonObject.keys())
        statusNodeData->data.push_back(NodeFieldData{key, jsonObject.value(key)});

    if(statusNodeData->layer)
        emit statusNodeDataReceived(statusNodeData);
}

void ServiceManager::messageData(QString jsonData)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject obj;
    if (!doc.isNull()) {
        if (doc.isObject()){
            obj = doc.object();
            QString type = obj.value("Type").toString();
            if      (type == "Layer")
                layersData(obj.value("Data").toObject());
            else if (type == "Flyable")
                flyableNodeData(obj.value("Data").toObject());
            else if (type == "Status")
                statusNodeData(obj.value("Data").toObject());
            else if (type == "Line")
                polylineData(obj.value("Data").toObject());
            else if(type == "Node")
                nodeData(obj.value("Data").toObject());
            else
                qDebug() << "type of data is unknown";
        }
    }
}

void ServiceManager::polylineData(QJsonObject polyline)
{
    QJsonArray points = polyline.value("Points").toArray();
    LineNodeData *lineNodeData = new LineNodeData;
    for (auto i : points) {
        QVector3D point;
        point.setX(i.toObject().value("Longitude").toDouble());
        point.setY(i.toObject().value("Latitude").toDouble());
        point.setZ(i.toObject().value("Altitude").toDouble());
        lineNodeData->points.push_back(point);
    }
    int layerId = polyline.value("LayerId").toInt();
    auto layer = findParenticLayer(layerId);
    if (layer){
        lineNodeData->layer = layer;
        lineNodeData->name = polyline.value("name").toString().toStdString();
        lineNodeData->id = polyline.value("Id").toInt();
        emit lineNodeDataReceived(lineNodeData);
    }
}

void ServiceManager::nodeData(QJsonObject jsonObject)
{
    NodeData* nodeData = new NodeData();
    nodeData->id = jsonObject.value("Id").toInt();
    nodeData->longitude =  jsonObject.value("Longitude").toDouble();
    nodeData->latitude = jsonObject.value("Latitude").toDouble();
    nodeData->altitude = jsonObject.value("Altitude").toDouble();
    nodeData->name = jsonObject.value("Name").toString().toStdString();
    nodeData->url2D = jsonObject.value("Url2d").toString().toStdString();
    nodeData->url3D = jsonObject.value("Url3d").toString().toStdString();
    nodeData->color = jsonObject.value("Color").toString().toStdString();
    for (auto i : jsonObject.value("LayersId").toArray()){
        int id = i.toInt();
        auto layer = findParenticLayer(id);
        if(layer)
            nodeData->layers.push_back(layer);
    }
    if(nodeData->layers.size() > 0)
        emit nodeDataReceived(nodeData);
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

ParenticAnnotationLayer *ServiceManager::findParenticLayer(int id)
{
    if (mParenticLayerMap.contains(id))
        return mParenticLayerMap[id];
    qDebug()<<"Can not found layer: "<<id;
    return nullptr;
}

