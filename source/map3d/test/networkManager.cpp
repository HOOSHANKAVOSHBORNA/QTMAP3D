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
    defaultExchange->publish(data, "data");
    qDebug() << "Sent flyable data: "<<data;
}

void NetworkManager::sendLayerData(const QString &data)
{
    QAmqpExchange *exchange = mClient.createExchange();
    exchange->publish(data, "data");
    qDebug() << "Sent layer data: "<<data;
}

void NetworkManager::sendStatusData(const QString &data)
{
    QAmqpExchange *exchange = mClient.createExchange();
    exchange->publish(data, "data");
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
    QAmqpQueue *dataQueue = mClient.createQueue("data");
    disconnect(dataQueue, 0, 0, 0); // in case this is a reconnect

    //--data -------------------------------------------
    connect(dataQueue, &QAmqpQueue::declared, this, &NetworkManager::dataQueueDeclared);
    dataQueue->declare();
}

void NetworkManager::clientError(QAMQP::Error error)
{
    qDebug()<<"error: "<<error<<": "<< mClient.errorString();
}
