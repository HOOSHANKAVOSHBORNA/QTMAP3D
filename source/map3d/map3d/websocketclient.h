#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtNetwork/QSslError>

class QWebSocket;
class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(QObject *parent = nullptr);
    void sendMessage(const QJsonDocument &message);
signals:
    void messageReceived(const QJsonDocument &message);
private slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onSslErrors(const QList<QSslError> &errors);
private:
    QWebSocket* mWebSocket;
};

#endif // WEBSOCKETCLIENT_H
