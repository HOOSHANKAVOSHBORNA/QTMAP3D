#include "networkManager.h"

NetworkManager::NetworkManager(QObject *parent): QObject(parent)
{
    mClient.setAutoReconnect(true);
}

void NetworkManager::sendFlyableData(const QString &data)
{
    QAmqpExchange *defaultExchange = mClient.createExchange();
    defaultExchange->publish(data, "flyable");
    qDebug() << "Sent flyable data: "<<data;
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
    //--flyable queue-------------------------------------------
    QAmqpQueue *queue = mClient.createQueue("flyable");
    disconnect(queue, 0, 0, 0); // in case this is a reconnect
    connect(queue, &QAmqpQueue::declared, this, &NetworkManager::flyableQueueDeclared);
    queue->declare();

}

void NetworkManager::clientError(QAMQP::Error error)
{
    qDebug()<<"error: "<<error<<": "<< mClient.errorString();
}
