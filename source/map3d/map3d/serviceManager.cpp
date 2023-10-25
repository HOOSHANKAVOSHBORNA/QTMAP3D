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
    flyableNodeData->id = jsonObject.value("Id").isObject() ? jsonObject.value("Id").toObject().value("value").toInt() : jsonObject.value("Id").toInt();
    flyableNodeData->longitude = jsonObject.value("Longitude").isObject() ? jsonObject.value("Longitude").toObject().value("value").toDouble() : jsonObject.value("Longitude").toDouble();
    flyableNodeData->latitude = jsonObject.value("Latitude").isObject() ? jsonObject.value("Latitude").toObject().value("value").toDouble() : jsonObject.value("Latitude").toDouble();
    flyableNodeData->altitude = jsonObject.value("Altitude").isObject() ? jsonObject.value("Altitude").toObject().value("value").toDouble() : jsonObject.value("Altitude").toDouble();
    flyableNodeData->name = jsonObject.value("Name").isObject() ? jsonObject.value("Name").toObject().value("value").toString().toStdString() : jsonObject.value("Name").toString().toStdString();
    flyableNodeData->url2D = jsonObject.value("Url2d").isObject() ? jsonObject.value("Url2d").toObject().value("value").toString().toStdString() : jsonObject.value("Url2d").toString().toStdString();
    flyableNodeData->url3D = jsonObject.value("Url3d").isObject() ? jsonObject.value("Url3d").toObject().value("value").toString().toStdString() : jsonObject.value("Url3d").toString().toStdString();
    flyableNodeData->imgSrc = jsonObject.value("ImgSrc").isObject() ? jsonObject.value("ImgSrc").toObject().value("value").toString().toStdString() : jsonObject.value("ImgSrc").toString().toStdString();
    flyableNodeData->iconSrc = jsonObject.value("IconSrc").isObject() ? jsonObject.value("IconSrc").toObject().value("value").toString().toStdString() : jsonObject.value("IconSrc").toString().toStdString();
    flyableNodeData->color = jsonObject.value("Color").isObject() ? jsonObject.value("Color").toObject().value("value").toString().toStdString() : jsonObject.value("Color").toString().toStdString();
    flyableNodeData->speed = jsonObject.value("Speed").isObject() ? jsonObject.value("Speed").toObject().value("value").toInt() : jsonObject.value("Speed").toInt();
    for (auto i : jsonObject.value("LayersId").toArray()){
        int id = i.toInt();
        auto layer = findParenticLayer(id);
        if(layer)
            flyableNodeData->layers.push_back(layer);
    }


    for (auto &key:jsonObject.keys()){
        if (jsonObject.value(key).isObject()) {
            auto obj = jsonObject.value(key).toObject();
            flyableNodeData->fieldData.push_back(NodeFieldData{key, obj.value("value").toVariant(), obj.value("category").toString()});
        }
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
    auto jsonObjectFieldData = jsonObject.value("FieldData").toObject();
    for(auto& key:jsonObjectFieldData.keys()){
        if(jsonObjectFieldData.value(key).type() == QJsonValue::Double)
            statusNodeData->fieldData.push_back(NodeFieldData{key, /*QObject::tr("%1").arg(*/jsonObjectFieldData.value(key).toDouble()/*,0,'f',4)*/});
        else
            statusNodeData->fieldData.push_back(NodeFieldData{key, jsonObjectFieldData.value(key).toString()});
    }

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
            else if (type == "Movable")
                movableNodeData(obj.value("Data").toObject());
            else if(type == "Node")
                nodeData(obj.value("Data").toObject());
            else if(type == "Circle")
                circleData(obj.value("Data").toObject());
            else if(type == "Polygon")
                polygonData(obj.value("Data").toObject());
            else
                qDebug() << "type of data is unknown";
        }
    }
}

void ServiceManager::polylineData(QJsonObject polyline)
{
    QJsonArray points = polyline.value("Points").toArray();
    PolyLineData *lineNodeData = new PolyLineData;
    for (auto i : points) {
        osg::Vec3d point;
        point.x() = (i.toObject().value("Longitude").toDouble());
        point.y() = (i.toObject().value("Latitude").toDouble());
        point.z() = (i.toObject().value("Altitude").toDouble());
        lineNodeData->points.push_back(point);
    }
    int layerId = polyline.value("LayerId").toInt();
    auto layer = findParenticLayer(layerId);
    if (layer){
        lineNodeData->layer = layer;
        lineNodeData->name = polyline.value("name").toString().toStdString();
        lineNodeData->id = polyline.value("Id").toInt();
        lineNodeData->color = polyline.value("Color").toString().toStdString();
        lineNodeData->width = polyline.value("Width").toInt();
        emit lineNodeDataReceived(lineNodeData);
    }
}

void ServiceManager::movableNodeData(QJsonObject jsonObject)
{
    NodeData* movableNodeData = new NodeData();
    movableNodeData->id = jsonObject.value("Id").isObject() ? jsonObject.value("Id").toObject().value("value").toInt() : jsonObject.value("Id").toInt();
    movableNodeData->longitude = jsonObject.value("Longitude").isObject() ? jsonObject.value("Longitude").toObject().value("value").toDouble() : jsonObject.value("Longitude").toDouble();
    movableNodeData->latitude = jsonObject.value("Latitude").isObject() ? jsonObject.value("Latitude").toObject().value("value").toDouble() : jsonObject.value("Latitude").toDouble();
    movableNodeData->altitude = jsonObject.value("Altitude").isObject() ? jsonObject.value("Altitude").toObject().value("value").toDouble() : jsonObject.value("Altitude").toDouble();
    movableNodeData->name = jsonObject.value("Name").isObject() ? jsonObject.value("Name").toObject().value("value").toString().toStdString() : jsonObject.value("Name").toString().toStdString();
    movableNodeData->url2D = jsonObject.value("Url2d").isObject() ? jsonObject.value("Url2d").toObject().value("value").toString().toStdString() : jsonObject.value("Url2d").toString().toStdString();
    movableNodeData->url3D = jsonObject.value("Url3d").isObject() ? jsonObject.value("Url3d").toObject().value("value").toString().toStdString() : jsonObject.value("Url3d").toString().toStdString();
    movableNodeData->imgSrc = jsonObject.value("ImgSrc").isObject() ? jsonObject.value("ImgSrc").toObject().value("value").toString().toStdString() : jsonObject.value("ImgSrc").toString().toStdString();
    movableNodeData->iconSrc = jsonObject.value("IconSrc").isObject() ? jsonObject.value("IconSrc").toObject().value("value").toString().toStdString() : jsonObject.value("IconSrc").toString().toStdString();
    movableNodeData->color = jsonObject.value("Color").isObject() ? jsonObject.value("Color").toObject().value("value").toString().toStdString() : jsonObject.value("Color").toString().toStdString();
    movableNodeData->speed = jsonObject.value("Speed").isObject() ? jsonObject.value("Speed").toObject().value("value").toInt() : jsonObject.value("Speed").toInt();

    for (auto i : jsonObject.value("LayersId").toArray()){
        int id = i.toInt();
        auto layer = findParenticLayer(id);
        if(layer)
            movableNodeData->layers.push_back(layer);
    }

    for (auto &key:jsonObject.keys()){
        if (jsonObject.value(key).isObject()) {
            auto obj = jsonObject.value(key).toObject();
            movableNodeData->fieldData.push_back(NodeFieldData{key, obj.value("value").toVariant(), obj.value("category").toString()});
        }
    }
    if(movableNodeData->layers.size() > 0)
        emit movableNodeDataReceived(movableNodeData);
}

void ServiceManager::nodeData(QJsonObject jsonObject)
{
    NodeData* nodeData = new NodeData();
    nodeData->id = jsonObject.value("Id").isObject() ? jsonObject.value("Id").toObject().value("value").toInt() : jsonObject.value("Id").toInt();
    nodeData->longitude = jsonObject.value("Longitude").isObject() ? jsonObject.value("Longitude").toObject().value("value").toDouble() : jsonObject.value("Longitude").toDouble();
    nodeData->latitude = jsonObject.value("Latitude").isObject() ? jsonObject.value("Latitude").toObject().value("value").toDouble() : jsonObject.value("Latitude").toDouble();
    nodeData->altitude = jsonObject.value("Altitude").isObject() ? jsonObject.value("Altitude").toObject().value("value").toDouble() : jsonObject.value("Altitude").toDouble();
    nodeData->name = jsonObject.value("Name").isObject() ? jsonObject.value("Name").toObject().value("value").toString().toStdString() : jsonObject.value("Name").toString().toStdString();
    nodeData->url2D = jsonObject.value("Url2d").isObject() ? jsonObject.value("Url2d").toObject().value("value").toString().toStdString() : jsonObject.value("Url2d").toString().toStdString();
    nodeData->url3D = jsonObject.value("Url3d").isObject() ? jsonObject.value("Url3d").toObject().value("value").toString().toStdString() : jsonObject.value("Url3d").toString().toStdString();
    nodeData->imgSrc = jsonObject.value("ImgSrc").isObject() ? jsonObject.value("ImgSrc").toObject().value("value").toString().toStdString() : jsonObject.value("ImgSrc").toString().toStdString();
    nodeData->iconSrc = jsonObject.value("IconSrc").isObject() ? jsonObject.value("IconSrc").toObject().value("value").toString().toStdString() : jsonObject.value("IconSrc").toString().toStdString();
    nodeData->color = jsonObject.value("Color").isObject() ? jsonObject.value("Color").toObject().value("value").toString().toStdString() : jsonObject.value("Color").toString().toStdString();

    for (auto i : jsonObject.value("LayersId").toArray()){
        int id = i.toInt();
        auto layer = findParenticLayer(id);
        if(layer)
            nodeData->layers.push_back(layer);
    }

    for (auto &key:jsonObject.keys()){
        if (jsonObject.value(key).isObject()) {
            auto obj = jsonObject.value(key).toObject();
            nodeData->fieldData.push_back(NodeFieldData{key, obj.value("value").toVariant(), obj.value("category").toString()});
        }
    }
    if(nodeData->layers.size() > 0)
        emit nodeDataReceived(nodeData);
}

void ServiceManager::circleData(QJsonObject jsonObject)
{
    CircleData* circleData = new CircleData();
    circleData->id = jsonObject.value("Id").toInt();
    circleData->name = jsonObject.value("Name").toString().toStdString();
    circleData->longitude =  jsonObject.value("Longitude").toDouble();
    circleData->latitude = jsonObject.value("Latitude").toDouble();
    circleData->altitude = jsonObject.value("Altitude").toDouble();
    circleData->radius = jsonObject.value("Radius").toDouble();
    circleData->color = jsonObject.value("Color").toString().toStdString();
    int layerId = jsonObject.value("LayerId").toInt();
    auto layer = findParenticLayer(layerId);
    circleData->layer = layer;
    if (layer)
        emit circleDataReceived(circleData);
}

void ServiceManager::polygonData(QJsonObject jsonObject)
{
    PolygonData *polygonData = new PolygonData;
    polygonData->id = jsonObject.value("Id").toInt();
    polygonData->name = jsonObject.value("Name").toString().toStdString();
    polygonData->width = jsonObject.value("Width").toInt();
    polygonData->color = jsonObject.value("Color").toString().toStdString();
    polygonData->fillColor = jsonObject.value("FillColor").toString().toStdString();

    QJsonArray points = jsonObject.value("Points").toArray();
    for (auto i : points) {
        osg::Vec3d point;
        point.x() = i.toObject().value("Longitude").toDouble();
        point.y() = i.toObject().value("Latitude").toDouble();
        point.z() = i.toObject().value("Altitude").toDouble();
        polygonData->points.push_back(point);
    }
    int layerId = jsonObject.value("LayerId").toInt();
    auto layer = findParenticLayer(layerId);
    polygonData->layer = layer;
    if (layer){
        emit polygonDataReceived(polygonData);
    }
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

