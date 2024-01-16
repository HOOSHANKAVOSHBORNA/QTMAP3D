#include "networkManager.h"
#include "qamqpexchange.h"
#include "qamqpqueue.h"

NetworkManager::NetworkManager(QObject *parent): QObject(parent)
{
    mClient.setAutoReconnect(true);
}

void NetworkManager::start()
{
    connect(&mClient, &QAmqpClient::connected, this, &NetworkManager::clientConnected);
    connect(&mClient, qOverload<QAMQP::Error >(&QAmqpClient::error), this, &NetworkManager::clientError);
    mClient.connectToHost();
}

void NetworkManager::sendMessage(const QString &message)
{
    QAmqpExchange *exchange = mClient.createExchange();
    exchange->publish(message, "Map3d");
//    qDebug() << "Sent data: "<<message;
}

void NetworkManager::clientConnected()
{
    qDebug() << "Client connected.";
    QAmqpQueue *map3dQueue = mClient.createQueue("Map3d");
    QAmqpQueue *map3dClientQueue = mClient.createQueue("Map3dClient");
    disconnect(map3dQueue, 0, 0, 0); // in case this is a reconnect
    disconnect(map3dClientQueue, 0, 0, 0);

    //--data -------------------------------------------
    connect(map3dQueue, &QAmqpQueue::declared, this, &NetworkManager::onMap3dQueueDeclare);
    connect(map3dClientQueue, &QAmqpQueue::declared, this, &NetworkManager::onMap3dClientQueueDeclare);
    map3dQueue->declare();
    map3dClientQueue->declare();
}

void NetworkManager::clientError(QAMQP::Error error)
{
    qDebug()<<"error: "<<error<<": "<< mClient.errorString();
}

void NetworkManager::onMap3dQueueDeclare()
{
    qDebug()<<"Map3d queue declared.";
    emit map3dQueueDeclared();
}

void NetworkManager::onMap3dClientQueueDeclare()
{
    qDebug() << "Map3dClient queue declared.";
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;
    connect(queue, &QAmqpQueue::messageReceived, this, &NetworkManager::onMessageReceived);
    queue->consume(QAmqpQueue::coNoAck);
    qDebug() << "Waiting for messages from Map3dClient queue.";
}

void NetworkManager::onMessageReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    QAmqpMessage message = queue->dequeue();
    QString messageString = message.payload();

    emit messageReceived(messageString);
}

//void NetworkManager::actionQueueReceived()
//{
//    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
//    if (!queue)
//        return;

//    QAmqpMessage message = queue->dequeue();
//    QString action = message.payload();

//    QJsonDocument doc = QJsonDocument::fromJson(action.toUtf8());
//    QJsonObject obj;
//    if (!doc.isNull()) {
//        if (doc.isObject()){
//            obj = doc.object();
//            QString type = obj.value("Type").toString();
//            if (type == "SignIn"){
//                bool status = obj.value("Data").toObject().value("Username").toString() == mUsername &&
//                              obj.value("Data").toObject().value("Password").toString() == mPassword;

//                QJsonDocument jsonDoc;
//                QJsonObject jsonObject;
//                jsonObject.insert("Type", "SignIn");
//                jsonObject.insert("COMMAND", status);
//                jsonObject.insert("ROLE", 1);
//                jsonDoc.setObject(jsonObject);
//                sendData(jsonDoc.toJson(QJsonDocument::Indented));
//                qDebug() << obj.value("Data").toObject().value("Username").toString() << ", \t " << obj.value("Data").toObject().value("Password").toString();
//            }
//            else if (type == "SignUp"){
//                mUsername = obj.value("Data").toObject().value("Username").toString();
//                mPassword = obj.value("Data").toObject().value("Password").toString();
//                QJsonDocument jsonDoc;
//                QJsonObject jsonObject;
//                jsonObject.insert("Type", "SignUp");
//                jsonObject.insert("COMMAND", true);
//                jsonDoc.setObject(jsonObject);
//                sendData(jsonDoc.toJson(QJsonDocument::Indented));
//            }
//            else if (type == "Assign"){
//                emit assignDataReceived(obj);
//            }
//        }
//    }

//    qDebug() << "message: " << message.payload();
//}
