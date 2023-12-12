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

public slots:
    void sendData(const QString &action);
private slots:
    void clientConnected();
    void sendDataQueueDeclared();
    void dataQueueDeclared();
    void dataMessageReceived();
signals:
//    void ready();
private:
    ServiceManager *mServiceManager;
    QAmqpClient mClient;
};

#endif // NETWORKMANAGER_H
