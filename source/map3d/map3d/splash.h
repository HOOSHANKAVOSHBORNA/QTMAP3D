#ifndef SPLASH_H
#define SPLASH_H

#include <QQuickWindow>


class Splash : public QQuickWindow
{
    Q_OBJECT
public:
    Splash(QWindow *parent = nullptr);
signals:
    void timeout();
};

#endif // SPLASH_H
