#include "serviceManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ServiceManager::ServiceManager(NetworkManager *networkManager, QObject *parent):
    QObject(parent)
    ,mNetworkManager(networkManager)
{
    connect(mNetworkManager, &NetworkManager::messageReceived, this, &ServiceManager::onMessageReceived);
}

void ServiceManager::sendLayer(const LayerData &layerData)
{
    auto inputJsonObject = layerData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "Layer");
    jsonObject.insert("Data", inputJsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    mNetworkManager->sendMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ServiceManager::sendNode(const NodeData &nodeData)
{
    auto inputJsonObject = nodeData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "Node");
    jsonObject.insert("Data", inputJsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    mNetworkManager->sendMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ServiceManager::sendStatusNode(const StatusNodeData &statusNodeData)
{
    auto inputJsonObject = statusNodeData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "Status");
    jsonObject.insert("Data", inputJsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    mNetworkManager->sendMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ServiceManager::sendAssignment(const AssignmentData &assignmentData)
{
    auto inputJsonObject = assignmentData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "Assignment");
    jsonObject.insert("Data", inputJsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    mNetworkManager->sendMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ServiceManager::sendPolyLine(const PolyLineData &polyLineData)
{
    auto inputJsonObject = polyLineData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "PolyLine");
    jsonObject.insert("Data", inputJsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    mNetworkManager->sendMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ServiceManager::sendPolygon(const PolygonData &polygonData)
{
    auto inputJsonObject = polygonData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "Polygon");
    jsonObject.insert("Data", inputJsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    mNetworkManager->sendMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ServiceManager::sendCircle(const CircleData &circleData)
{
    auto inputJsonObject = circleData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "Circle");
    jsonObject.insert("Data", inputJsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    mNetworkManager->sendMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ServiceManager::sendExplosion(const ExplosionData &explosionData)
{
    auto inputJsonObject = explosionData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "Explosion");
    jsonObject.insert("Data", inputJsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    mNetworkManager->sendMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ServiceManager::onMessageReceived(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj;
    if (!doc.isNull()) {
        if (doc.isObject()){
            obj = doc.object();
            QString type = obj.value("Type").toString();
            if (type == "Assignment"){
                AssignmentData assignmentData;
                assignmentData.fromJson(obj);
                emit assignmentDataReceived(assignmentData);
            }
            else
                qDebug() << "type of data is unknown";
        }
    }
}
