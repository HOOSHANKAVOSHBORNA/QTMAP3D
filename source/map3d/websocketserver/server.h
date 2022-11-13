#ifndef SERVER_H
#define SERVER_H

#include<QObject>
#include<QList>
#include<QByteArray>
#include<QtNetwork/QSslError>

class QWebSocketServer;
class QWebSocket;

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject *parent = nullptr);
    ~Server() override;

signals:

public slots:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void onSslErrors(const QList<QSslError> &errors);

private:
    QWebSocketServer *mWebSocketServer;
    QList<QWebSocket *> mClients;
};

#endif // SERVER_H
