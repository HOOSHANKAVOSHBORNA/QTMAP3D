#include "networkManager.h"
#include "qamqpexchange.h"
#include "qamqpqueue.h"
#include "qjsonobject.h"

#include <QJsonDocument>

NetworkManager::NetworkManager(QObject *parent): QObject(parent)
{
    mClient.setAutoReconnect(true);
}

void NetworkManager::sendData(const QString &data)
{
    QAmqpExchange *exchange = mClient.createExchange();
    exchange->publish(data, "data");
    // qDebug() << "Sent data: "<<data;
}

void NetworkManager::start()
{
    connect(&mClient, &QAmqpClient::connected, this, &NetworkManager::clientConnected);
    connect(&mClient, qOverload<QAMQP::Error >(&QAmqpClient::error), this, &NetworkManager::clientError);
    mClient.connectToHost();
}

void NetworkManager::clientConnected()
{
    qDebug() << "Client connected.";
    QAmqpQueue *dataQueue = mClient.createQueue("data");
    QAmqpQueue *actionQueue = mClient.createQueue("action");
    disconnect(dataQueue, 0, 0, 0); // in case this is a reconnect
    disconnect(actionQueue, 0, 0, 0);

    //--data -------------------------------------------
    connect(dataQueue, &QAmqpQueue::declared, this, &NetworkManager::dataQueueDeclared);
    connect(actionQueue, &QAmqpQueue::declared, this, &NetworkManager::actionQueueDeclared);
    dataQueue->declare();
    actionQueue->declare();
}

void NetworkManager::clientError(QAMQP::Error error)
{
    qDebug()<<"error: "<<error<<": "<< mClient.errorString();
}

void NetworkManager::actionQueueReceived()
{
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;

    QAmqpMessage message = queue->dequeue();
    QString action = message.payload();

    QJsonDocument doc = QJsonDocument::fromJson(action.toUtf8());
    QJsonObject obj;
    if (!doc.isNull()) {
        if (doc.isObject()){
            obj = doc.object();
            QString type = obj.value("Type").toString();
            if (type == "SignIn"){
                bool status = obj.value("Data").toObject().value("Username").toString() == mUsername &&
                              obj.value("Data").toObject().value("Password").toString() == mPassword;

                QJsonDocument jsonDoc;
                QJsonObject jsonObject;
                jsonObject.insert("Type", "SignIn");
                jsonObject.insert("COMMAND", status);
                jsonObject.insert("ROLE", 1);
                jsonDoc.setObject(jsonObject);
                sendData(jsonDoc.toJson(QJsonDocument::Indented));
                qDebug() << obj.value("Data").toObject().value("Username").toString() << ", \t " << obj.value("Data").toObject().value("Password").toString();
            }
            else if (type == "SignUp"){
                mUsername = obj.value("Data").toObject().value("Username").toString();
                mPassword = obj.value("Data").toObject().value("Password").toString();
                QJsonDocument jsonDoc;
                QJsonObject jsonObject;
                jsonObject.insert("Type", "SignUp");
                jsonObject.insert("COMMAND", true);
                jsonDoc.setObject(jsonObject);
                sendData(jsonDoc.toJson(QJsonDocument::Indented));
            }
            else if (type == "Assign"){
                emit assignDataReceived(obj);
            }
        }
    }

    qDebug() << "message: " << message.payload();
}

void NetworkManager::actionQueueDeclared()
{
    qDebug() << "queue dec";
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue)
        return;
    connect(queue, &QAmqpQueue::messageReceived, this, &NetworkManager::actionQueueReceived);
    queue->consume(QAmqpQueue::coNoAck);
    qDebug() << "Waiting for messages.";

}
