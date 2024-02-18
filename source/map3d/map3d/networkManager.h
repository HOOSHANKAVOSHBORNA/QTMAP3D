#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include "qamqpclient.h"


class NetworkManager: public QObject
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = nullptr);
//    void start();
    void setConfig(QString ip, int port, QString username, QString pass);
    void sendMessage(const QString &message);
    bool isConnected();
    bool isConsuming();
private slots:
    void clientConnected();
    void clientError(QAMQP::Error error);
    void onMap3dQueueDeclare();
    void onMap3dClientQueueDeclare();
    void onMessageReceived();
signals:
    void map3dClientQueueDeclared();
    void messageReceived(const QString &message);

private:
    QAmqpClient mClient;
    QAmqpQueue *mMap3dQueue{nullptr};
};

#endif // NETWORKMANAGER_H
