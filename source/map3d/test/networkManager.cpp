#include "networkManager.h"
#include "qamqpexchange.h"
#include "qamqpqueue.h"

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

void NetworkManager::sendLayerData(const QString &data)
{
    QAmqpExchange *exchange = mClient.createExchange();
    exchange->publish(data, "layer");
    qDebug() << "Sent layer data: "<<data;
}

void NetworkManager::sendStatusData(const QString &data)
{
    QAmqpExchange *exchange = mClient.createExchange();
    exchange->publish(data, "status");
    qDebug() << "Sent status data: "<<data;
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
    //--layer queue-------------------------------------------
    QAmqpQueue *layerQueue = mClient.createQueue("layer");
    disconnect(layerQueue, 0, 0, 0); // in case this is a reconnect
    connect(layerQueue, &QAmqpQueue::declared, this, &NetworkManager::layerQueueDeclared);
    layerQueue->declare();

    //--flyable queue-------------------------------------------
    QAmqpQueue *flyableQueue = mClient.createQueue("flyable");
    disconnect(flyableQueue, 0, 0, 0); // in case this is a reconnect
    connect(flyableQueue, &QAmqpQueue::declared, this, &NetworkManager::flyableQueueDeclared);
    flyableQueue->declare();

    //--status queue--------------------------------------------
    QAmqpQueue *statusQueue = mClient.createQueue("status");
    disconnect(statusQueue, 0, 0, 0); // in case this is a reconnect
    connect(statusQueue, &QAmqpQueue::declared, this, &NetworkManager::statusQueueDeclared);
    statusQueue->declare();

}

void NetworkManager::clientError(QAMQP::Error error)
{
    qDebug()<<"error: "<<error<<": "<< mClient.errorString();
}
