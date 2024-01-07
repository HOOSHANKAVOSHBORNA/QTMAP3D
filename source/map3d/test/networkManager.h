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
    void sendMessage(const QString &message);
private slots:
    void clientConnected();
    void clientError(QAMQP::Error error);
    void onMap3dQueueDeclare();
    void onMap3dClientQueueDeclare();
    void onMessageReceived();
signals:
    void map3dQueueDeclared();
    void messageReceived(const QString &message);

private:
    QAmqpClient mClient;
    QString mUsername{"Admin"};
    QString mPassword{"e3afed0047b08059d0fada10f400c1e5"};
};

#endif // NETWORKMANAGER_H
