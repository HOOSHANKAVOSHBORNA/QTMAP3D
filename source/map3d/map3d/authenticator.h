#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H
#include "serviceManager.h"

#include <QQuickWindow>

class QQmlEngine;
class Authenticator : public QQuickWindow
{
    Q_OBJECT
public:
    Authenticator(QWindow *parent = nullptr);

    void setServiceManager(ServiceManager *newServiceManager);

public slots:
    void signIn(const QString username, const QString password);
    void signUp(const QString username, const QString password);

signals:
    void signedIn(bool status);
    void signedUp(bool status);

private:
    QQmlEngine* mQmlEngine{nullptr};

    ServiceManager* mServiceManager{nullptr};

};

#endif // AUTHENTICATOR_H
