#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

#include "qamqpclient.h"

class NetworkManager: public QObject
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = nullptr);
    void start();

private slots:
    void clientConnected();
    void flyableQueueDeclared();
    void flyableMessageReceived();

    void layerQueueDeclared();
    void layerMessageReceived();
signals:
//    void ready();
private:
    QAmqpClient mClient;
};

#endif // NETWORKMANAGER_H
