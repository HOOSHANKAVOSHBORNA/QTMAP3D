#include "serviceManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "networkManager.h"

ServiceManager::ServiceManager(NetworkManager *networkManager, QObject *parent):
    QObject(parent),
    mNetworkManager(networkManager)
{
    connect(mNetworkManager, &NetworkManager::messageReceived, this, &ServiceManager::onMessageReceived);
}

//void ServiceManager::layersData(QJsonObject layers)
//{
//    mMapObject->clearCompositeLayers();
//    for (auto it : layers.value("Data").toArray())
//        parseLayersFromJson(it.toObject());
//    mMapObject->clearOldParenticLayers();
//}

//void ServiceManager::flyableNodeData(QJsonObject jsonObject)
//{
//    NodeData* flyableNodeData = new NodeData();
//    QJsonObject jsonObjectData = jsonObject.value("Data").toObject();

//    flyableNodeData->type = "Flyable";
//    flyableNodeData->id = jsonObjectData.value("Id").isObject() ? jsonObjectData.value("Id").toObject().value("value").toInt() : jsonObjectData.value("Id").toInt();
//    flyableNodeData->longitude = jsonObjectData.value("Longitude").isObject() ? jsonObjectData.value("Longitude").toObject().value("value").toDouble() : jsonObjectData.value("Longitude").toDouble();
//    flyableNodeData->latitude = jsonObjectData.value("Latitude").isObject() ? jsonObjectData.value("Latitude").toObject().value("value").toDouble() : jsonObjectData.value("Latitude").toDouble();
//    flyableNodeData->altitude = jsonObjectData.value("Altitude").isObject() ? jsonObjectData.value("Altitude").toObject().value("value").toDouble() : jsonObjectData.value("Altitude").toDouble();
//    flyableNodeData->name = jsonObjectData.value("Name").isObject() ? jsonObjectData.value("Name").toObject().value("value").toString().toStdString() : jsonObjectData.value("Name").toString().toStdString();
//    flyableNodeData->url2D = jsonObjectData.value("Url2d").isObject() ? jsonObjectData.value("Url2d").toObject().value("value").toString().toStdString() : jsonObjectData.value("Url2d").toString().toStdString();
//    flyableNodeData->url3D = jsonObjectData.value("Url3d").isObject() ? jsonObjectData.value("Url3d").toObject().value("value").toString().toStdString() : jsonObjectData.value("Url3d").toString().toStdString();
//    flyableNodeData->imgSrc = jsonObjectData.value("ImgSrc").isObject() ? jsonObjectData.value("ImgSrc").toObject().value("value").toString().toStdString() : jsonObjectData.value("ImgSrc").toString().toStdString();
//    flyableNodeData->iconSrc = jsonObjectData.value("IconSrc").isObject() ? jsonObjectData.value("IconSrc").toObject().value("value").toString().toStdString() : jsonObjectData.value("IconSrc").toString().toStdString();
//    flyableNodeData->color = jsonObjectData.value("Color").isObject() ? jsonObjectData.value("Color").toObject().value("value").toString().toStdString() : jsonObjectData.value("Color").toString().toStdString();
//    flyableNodeData->speed = jsonObjectData.value("Speed").isObject() ? jsonObjectData.value("Speed").toObject().value("value").toInt() : jsonObjectData.value("Speed").toInt();
//    flyableNodeData->command = jsonObject.value("COMMAND").toString().toStdString();
//    for (auto i : jsonObjectData.value("LayersId").toArray()){
//        int id = i.toInt();
//        auto layer = findParenticLayer(id);
//        if(layer)
//            flyableNodeData->layers.push_back(layer);
//    }


//    for (auto &key:jsonObjectData.keys()){
//        if (jsonObjectData.value(key).isObject()) {
//            auto obj = jsonObjectData.value(key).toObject();
//            flyableNodeData->fieldData.push_back(NodeFieldData{key, obj.value("value").toVariant(), obj.value("category").toString()});
//        }
//    }
//    //    if(flyableNodeData->layers.size() > 0)
//    emit flyableNodeDataReceived(flyableNodeData);
//}

//void ServiceManager::statusNodeData(QJsonObject jsonObject)
//{
//    StatusNodeData *statusNodeData = new StatusNodeData;
//    QJsonObject jsonObjectData = jsonObject.value("Data").toObject();

//    statusNodeData->id = jsonObjectData.value("Id").toInt();
//    statusNodeData->name = jsonObjectData.value("Name").toString().toStdString();
//    statusNodeData->longitude =  jsonObjectData.value("Longitude").toDouble();
//    statusNodeData->latitude = jsonObjectData.value("Latitude").toDouble();
//    statusNodeData->altitude = jsonObjectData.value("Altitude").toDouble();
//    int layerId = jsonObjectData.value("LayerId").toInt();
//    statusNodeData->command = jsonObject.value("COMMAND").toString().toStdString();
//    auto layer = findParenticLayer(layerId);
//    if(layer)
//        statusNodeData->layer = layer;
//    auto jsonObjectFieldData = jsonObjectData.value("FieldData").toObject();
//    for(auto& key:jsonObjectFieldData.keys()){
//        if(jsonObjectFieldData.value(key).type() == QJsonValue::Double)
//            statusNodeData->fieldData.push_back(NodeFieldData{key, /*QObject::tr("%1").arg(*/jsonObjectFieldData.value(key).toDouble()/*,0,'f',4)*/});
//        else
//            statusNodeData->fieldData.push_back(NodeFieldData{key, jsonObjectFieldData.value(key).toString()});
//    }

//    if(statusNodeData->layer)
//        emit statusNodeDataReceived(statusNodeData);
//}

//void ServiceManager::receiveExplosionData(QJsonObject jsonObject)
//{
//    ExplosionData *explosionData = new ExplosionData;
//    explosionData->latitude  = jsonObject.value("latitude").toDouble();
//    explosionData->longitude = jsonObject.value("longitude").toDouble();
//    explosionData->duration  = jsonObject.value("duration").toDouble();
//    explosionData->scale     = jsonObject.value("scale").toDouble();
//    explosionData->command   = jsonObject.value("COMMAND").toString().toStdString();
//    emit explosionDataReceived(explosionData);
//}


//void ServiceManager::receiveAssignmentData(QJsonObject jsonObject)
//{
//    AssignData *assignData = new AssignData;
//    assignData->attackerID = jsonObject.value("attackerID").toInt();
//    assignData->targetID = jsonObject.value("targetID").toInt();
//    assignData->state = jsonObject.value("state").toString().toStdString();
//    assignData->command = jsonObject.value("COMMAND").toString().toStdString();
//    emit assignDataReceived(assignData);
//}

//void ServiceManager::sendJsonAssignData(AssignData data)
//{
//    QJsonObject jsonObject;
//    jsonObject.insert("Type","Assign");
//    jsonObject.insert("COMMAND",QString::fromStdString(data.command));
//    jsonObject.insert("attackerID",data.attackerID);
//    jsonObject.insert("targetID",data.targetID);
//    QJsonDocument jsonDoc;
//    jsonDoc.setObject(jsonObject);
//    sendAction(jsonDoc.toJson(QJsonDocument::Indented));
//}


void ServiceManager::onMessageReceived(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj;
    if (!doc.isNull()) {
        if (doc.isObject()){
            obj = doc.object();
            QString type = obj.value("Type").toString();
//            if      (type == "Layer")
//                layersData(obj);
//            else if (type == "Flyable")
//                flyableNodeData(obj);
//            else if (type == "Status")
//                statusNodeData(obj);
//            else if (type == "Line")
//                polylineData(obj);
//            else if (type == "Assign")
//                receiveAssignmentData(obj);
//            else if (type == "Particle")
//                receiveExplosionData(obj);
//            else if (type == "Movable")
//                movableNodeData(obj);
//            else if(type == "Node")
//                nodeData(obj);
//            else if(type == "Circle")
//                circleData(obj);
//            else if(type == "Polygon")
//                polygonData(obj);
//            else if(type == "SignUp")
//                signUpData(obj);
//            else if(type == "SignIn")
//                signInData(obj);
//            else
//                qDebug() << "type of data is unknown";
        }
    }
}

//void ServiceManager::sendAction(const QString& action)
//{
//    emit actionSent(action);
//}

//void ServiceManager::polylineData(QJsonObject polyline)
//{
//    QJsonObject jsonObjectData = polyline.value("Data").toObject();
//    QJsonArray points = jsonObjectData.value("Points").toArray();
//    PolyLineData *lineNodeData = new PolyLineData;
//    for (auto i : points) {
//        osg::Vec3d point;
//        point.x() = (i.toObject().value("Longitude").toDouble());
//        point.y() = (i.toObject().value("Latitude").toDouble());
//        point.z() = (i.toObject().value("Altitude").toDouble());
//        lineNodeData->points.push_back(point);
//    }
//    int layerId = jsonObjectData.value("LayerId").toInt();
//    auto layer = findParenticLayer(layerId);
//    if (layer){
//        lineNodeData->layer = layer;
//        lineNodeData->name = jsonObjectData.value("name").toString().toStdString();
//        lineNodeData->id = jsonObjectData.value("Id").toInt();
//        lineNodeData->color = jsonObjectData.value("Color").toString().toStdString();
//        lineNodeData->width = jsonObjectData.value("Width").toInt();
//        lineNodeData->command = polyline.value("COMMAND").toString().toStdString();
//        emit lineNodeDataReceived(lineNodeData);
//    }
//}

//void ServiceManager::movableNodeData(QJsonObject jsonObject)
//{
//    QJsonObject jsonObjectData = jsonObject.value("Data").toObject();
//    NodeData* movableNodeData = new NodeData();
//    movableNodeData->type = "Movable";
//    movableNodeData->id = jsonObjectData.value("Id").isObject() ? jsonObjectData.value("Id").toObject().value("value").toInt() : jsonObjectData.value("Id").toInt();
//    movableNodeData->longitude = jsonObjectData.value("Longitude").isObject() ? jsonObjectData.value("Longitude").toObject().value("value").toDouble() : jsonObjectData.value("Longitude").toDouble();
//    movableNodeData->latitude = jsonObjectData.value("Latitude").isObject() ? jsonObjectData.value("Latitude").toObject().value("value").toDouble() : jsonObjectData.value("Latitude").toDouble();
//    movableNodeData->altitude = jsonObjectData.value("Altitude").isObject() ? jsonObjectData.value("Altitude").toObject().value("value").toDouble() : jsonObjectData.value("Altitude").toDouble();
//    movableNodeData->name = jsonObjectData.value("Name").isObject() ? jsonObjectData.value("Name").toObject().value("value").toString().toStdString() : jsonObjectData.value("Name").toString().toStdString();
//    movableNodeData->url2D = jsonObjectData.value("Url2d").isObject() ? jsonObjectData.value("Url2d").toObject().value("value").toString().toStdString() : jsonObjectData.value("Url2d").toString().toStdString();
//    movableNodeData->url3D = jsonObjectData.value("Url3d").isObject() ? jsonObjectData.value("Url3d").toObject().value("value").toString().toStdString() : jsonObjectData.value("Url3d").toString().toStdString();
//    movableNodeData->imgSrc = jsonObjectData.value("ImgSrc").isObject() ? jsonObjectData.value("ImgSrc").toObject().value("value").toString().toStdString() : jsonObjectData.value("ImgSrc").toString().toStdString();
//    movableNodeData->iconSrc = jsonObjectData.value("IconSrc").isObject() ? jsonObjectData.value("IconSrc").toObject().value("value").toString().toStdString() : jsonObjectData.value("IconSrc").toString().toStdString();
//    movableNodeData->color = jsonObjectData.value("Color").isObject() ? jsonObjectData.value("Color").toObject().value("value").toString().toStdString() : jsonObjectData.value("Color").toString().toStdString();
//    movableNodeData->speed = jsonObjectData.value("Speed").isObject() ? jsonObjectData.value("Speed").toObject().value("value").toInt() : jsonObjectData.value("Speed").toInt();
//    movableNodeData->command = jsonObject.value("COMMAND").toString().toStdString();
//    for (auto i : jsonObjectData.value("LayersId").toArray()){
//        int id = i.toInt();
//        auto layer = findParenticLayer(id);
//        if(layer)
//            movableNodeData->layers.push_back(layer);
//    }

//    for (auto &key:jsonObjectData.keys()){
//        if (jsonObjectData.value(key).isObject()) {
//            auto obj = jsonObjectData.value(key).toObject();
//            movableNodeData->fieldData.push_back(NodeFieldData{key, obj.value("value").toVariant(), obj.value("category").toString()});
//        }
//    }
//    if(movableNodeData->layers.size() > 0)
//        emit movableNodeDataReceived(movableNodeData);
//}

//void ServiceManager::signInData(QJsonObject jsonObject)
//{
//    bool status = jsonObject.value("COMMAND").toBool();
//    int role = jsonObject.value("ROLE").toInt(1);
//    emit signInResponseReceived(status, role);
//}

//void ServiceManager::signUpData(QJsonObject jsonObject)
//{
//    bool status = jsonObject.value("COMMAND").toBool();
//    emit signUpResponseReceived(status);
//}

//void ServiceManager::nodeData(QJsonObject jsonObject)
//{
//    QJsonObject jsonObjectData = jsonObject.value("Data").toObject();
//    NodeData* nodeData = new NodeData();
//    nodeData->type = "Node";
//    nodeData->id = jsonObjectData.value("Id").isObject() ? jsonObjectData.value("Id").toObject().value("value").toInt() : jsonObjectData.value("Id").toInt();
//    nodeData->longitude = jsonObjectData.value("Longitude").isObject() ? jsonObjectData.value("Longitude").toObject().value("value").toDouble() : jsonObjectData.value("Longitude").toDouble();
//    nodeData->latitude = jsonObjectData.value("Latitude").isObject() ? jsonObjectData.value("Latitude").toObject().value("value").toDouble() : jsonObjectData.value("Latitude").toDouble();
//    nodeData->altitude = jsonObjectData.value("Altitude").isObject() ? jsonObjectData.value("Altitude").toObject().value("value").toDouble() : jsonObjectData.value("Altitude").toDouble();
//    nodeData->name = jsonObjectData.value("Name").isObject() ? jsonObjectData.value("Name").toObject().value("value").toString().toStdString() : jsonObjectData.value("Name").toString().toStdString();
//    nodeData->url2D = jsonObjectData.value("Url2d").isObject() ? jsonObjectData.value("Url2d").toObject().value("value").toString().toStdString() : jsonObjectData.value("Url2d").toString().toStdString();
//    nodeData->url3D = jsonObjectData.value("Url3d").isObject() ? jsonObjectData.value("Url3d").toObject().value("value").toString().toStdString() : jsonObjectData.value("Url3d").toString().toStdString();
//    nodeData->imgSrc = jsonObjectData.value("ImgSrc").isObject() ? jsonObjectData.value("ImgSrc").toObject().value("value").toString().toStdString() : jsonObjectData.value("ImgSrc").toString().toStdString();
//    nodeData->iconSrc = jsonObjectData.value("IconSrc").isObject() ? jsonObjectData.value("IconSrc").toObject().value("value").toString().toStdString() : jsonObjectData.value("IconSrc").toString().toStdString();
//    nodeData->color = jsonObjectData.value("Color").isObject() ? jsonObjectData.value("Color").toObject().value("value").toString().toStdString() : jsonObjectData.value("Color").toString().toStdString();
//    nodeData->command = jsonObject.value("COMMAND").toString().toStdString();
//    for (auto i : jsonObjectData.value("LayersId").toArray()){
//        int id = i.toInt();
//        auto layer = findParenticLayer(id);
//        if(layer)
//            nodeData->layers.push_back(layer);
//    }

//    for (auto &key:jsonObjectData.keys()){
//        if (jsonObjectData.value(key).isObject()) {
//            auto obj = jsonObjectData.value(key).toObject();
//            nodeData->fieldData.push_back(NodeFieldData{key, obj.value("value").toVariant(), obj.value("category").toString()});
//        }
//    }
//    if(nodeData->layers.size() > 0)
//        emit nodeDataReceived(nodeData);
//}

//void ServiceManager::circleData(QJsonObject jsonObject)
//{
//    QJsonObject jsonObjectData = jsonObject.value("Data").toObject();
//    CircleData* circleData = new CircleData();
//    circleData->id = jsonObjectData.value("Id").toInt();
//    circleData->name = jsonObjectData.value("Name").toString().toStdString();
//    circleData->longitude =  jsonObjectData.value("Longitude").toDouble();
//    circleData->latitude = jsonObjectData.value("Latitude").toDouble();
//    circleData->altitude = jsonObjectData.value("Altitude").toDouble();
//    circleData->radius = jsonObjectData.value("Radius").toDouble();
//    circleData->color = jsonObjectData.value("Color").toString().toStdString();
//    circleData->command = jsonObject.value("COMMAND").toString().toStdString();
//    int layerId = jsonObjectData.value("LayerId").toInt();
//    auto layer = findParenticLayer(layerId);
//    circleData->layer = layer;
//    if (layer)
//        emit circleDataReceived(circleData);
//}

//void ServiceManager::polygonData(QJsonObject jsonObject)
//{
//    QJsonObject jsonObjectData = jsonObject.value("Data").toObject();
//    PolygonData *polygonData = new PolygonData;
//    polygonData->id = jsonObjectData.value("Id").toInt();
//    polygonData->name = jsonObjectData.value("Name").toString().toStdString();
//    polygonData->width = jsonObjectData.value("Width").toInt();
//    polygonData->color = jsonObjectData.value("Color").toString().toStdString();
//    polygonData->fillColor = jsonObjectData.value("FillColor").toString().toStdString();
//    polygonData->command = jsonObject.value("COMMAND").toString().toStdString();

//    QJsonArray points = jsonObjectData.value("Points").toArray();
//    for (auto i : points) {
//        osg::Vec3d point;
//        point.x() = i.toObject().value("Longitude").toDouble();
//        point.y() = i.toObject().value("Latitude").toDouble();
//        point.z() = i.toObject().value("Altitude").toDouble();
//        polygonData->points.push_back(point);
//    }
//    int layerId = jsonObjectData.value("LayerId").toInt();
//    auto layer = findParenticLayer(layerId);
//    polygonData->layer = layer;
//    if (layer){
//        emit polygonDataReceived(polygonData);
//    }
//}

//void ServiceManager::parseLayersFromJson(QJsonObject jsonObject, CompositeAnnotationLayer *parent)
//{
//    if (jsonObject.value("Children").toArray().size() > 0){
//        CompositeAnnotationLayer* comp = new CompositeAnnotationLayer(jsonObject.value("Id").toInt());
//        comp->setName(jsonObject.value("Text").toString().toStdString());
//        comp->setOrder(jsonObject.value("Order").toInt());

//        if (parent){
//            parent->addLayer(comp);
//        }
//        else{
//            mMapObject->addLayerFromServeice(comp);
//        }

//        for (auto it: jsonObject.value("Children").toArray()) {
//            parseLayersFromJson(it.toObject(), comp);
//        }
//    }
//    else {
//        int layerId = jsonObject.value("Id").toInt();
//        ParenticAnnotationLayer *p = new ParenticAnnotationLayer(layerId);
//        p->setName(jsonObject.value("Text").toString().toStdString());
//        parent->addLayer(p);
//        mMapObject->addLayerFromServeice(p);
//        return;
//    }
//}

//ParenticAnnotationLayer *ServiceManager::findParenticLayer(int id)
//{
//    ParenticAnnotationLayer *p = mMapObject->getServiceLayerBiId(id);
//    if (p)
//        return p;
//    qDebug()<<"Can not found layer: "<<id;
//    return nullptr;
//}

//void ServiceManager::setMapObject(MapObject *newMapObject)
//{
//    mMapObject = newMapObject;
//}
