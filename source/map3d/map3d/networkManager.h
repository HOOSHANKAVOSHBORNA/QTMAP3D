#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

#include "qamqpclient.h"
#include "serviceManager.h"

class NetworkManager: public QObject
{
    Q_OBJECT
public:
    NetworkManager(ServiceManager *serviceManger, QObject *parent = nullptr);
    void start();

private slots:
    void clientConnected();
    void sendData();
    void dataQueueDeclared();
    void dataMessageReceived();
signals:
//    void ready();
private:
    ServiceManager *mServiceManager;
    QAmqpClient mClient;
};

#endif // NETWORKMANAGER_H
