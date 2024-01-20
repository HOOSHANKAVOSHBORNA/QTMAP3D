#include "serviceManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ServiceManager::ServiceManager(NetworkManager *networkManager, QObject *parent):
    QObject(parent)
    ,mNetworkManager(networkManager)
{
    connect(mNetworkManager, &NetworkManager::messageReceived, this, &ServiceManager::onMessageReceived);
    connect(mNetworkManager, &NetworkManager::map3dQueueDeclared, [this](){
        mIsReadyForSendData = true;
        emit readyForSendData();
    });
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

void ServiceManager::sendUser(const UserData &userData)
{
    auto inputJsonObject = userData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "User");
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
            QJsonObject data = obj.value("Data").toObject();
            if (type == "Assignment"){
                AssignmentData assignmentData;
                assignmentData.fromJson(data);
                emit assignmentDataReceived(assignmentData);
                qDebug() << "message received" << data;
            }
            else if (type == "User"){
                UserData userData;
                userData.fromJson(data);
                emit userDataReceived(userData);
            }
            else
                qDebug() << "type of data is unknown";
        }
    }
}

bool ServiceManager::isReadyForSendData() const
{
    return mIsReadyForSendData;
}
