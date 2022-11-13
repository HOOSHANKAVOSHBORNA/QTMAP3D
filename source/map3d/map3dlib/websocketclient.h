#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtNetwork/QSslError>

class QWebSocket;
class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(QString name, QObject *parent = nullptr);
    void sendTextMessage(const QString &message);
signals:

public slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onSslErrors(const QList<QSslError> &errors);
private:
    QWebSocket* mWebSocket;
    QString mName;
};

#endif // WEBSOCKETCLIENT_H
