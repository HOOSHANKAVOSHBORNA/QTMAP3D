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

void ServiceManager::sendUser(const UserData &UserData)
{
    auto inputJsonObject = UserData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "User");
    jsonObject.insert("Data", inputJsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    mNetworkManager->sendMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ServiceManager::sendReady(const ReadyForData &readyForData)
{
    auto inputJsonObject = readyForData.toJson();
    QJsonObject jsonObject;
    jsonObject.insert("Type", "Ready");
    jsonObject.insert("Data", inputJsonObject);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObject);
    mNetworkManager->sendMessage(jsonDoc.toJson(QJsonDocument::Compact));
}

void ServiceManager::onMessageReceived(const QString &message)
{
//    qDebug() << "Receive message: "<<message;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj;
    if (!doc.isNull()) {
        if (doc.isObject()){
            obj = doc.object();
            QString type = obj.value("Type").toString();
            QJsonObject data = obj.value("Data").toObject();
            if(type == "Layer"){
                LayerData layerData;
                layerData.fromJson(data);
                emit layerDataReceived(layerData);
            }
            else if (type == "Node"){
                NodeData nodData;
                nodData.fromJson(data);
                emit nodeDataReceived(nodData);
            }
            else if (type == "Status"){
                StatusNodeData statusNodeData;
                statusNodeData.fromJson(data);
                emit statusNodeDataReceived(statusNodeData);
            }
            else if (type == "Assignment"){
                AssignmentData assignmentData;
                assignmentData.fromJson(data);
                emit assignmentDataReceived(assignmentData);
            }
            else if (type == "PolyLine"){
                PolyLineData polyLineData;
                polyLineData.fromJson(data);
                emit polyLineDataReceived(polyLineData);
            }
            else if (type == "Polygon"){
                PolygonData polygonData;
                polygonData.fromJson(data);
                emit polygonDataReceived(polygonData);
            }
            else if (type == "Circle"){
                CircleData circleData;
                circleData.fromJson(data);
                emit circleDataReceived(circleData);
            }
            else if(type == "Explosion"){
                ExplosionData explosionData;
                explosionData.fromJson(data);
                emit explosionDataReceived(explosionData);
            }
            else if(type == "User"){
                UserData userData;
                userData.fromJson(data);
                emit userDataReceived(userData);
            }
            else
                qDebug() << "type of data is unknown";
        }
    }
}
