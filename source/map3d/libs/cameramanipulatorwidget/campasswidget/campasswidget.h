#ifndef CAMPASSWIDGET_H
#define CAMPASSWIDGET_H

#include "campasswidget_global.h"
#include <QWidget>
#include <QQuickView>
#include "QQuickWidget"
//  library for display qml  in widget
#include <QQmlContext>
#include <QQmlEngine>
class CAMPASSWIDGET_EXPORT CampassWidget : public QWidget
{
    Q_OBJECT
public:
    CampassWidget(QWidget *parent = nullptr);
    void setRotate(double rotate);
signals:
    void rotateChange(double newRotate);
public slots:
    Q_INVOKABLE double getRotate();

private:
    QQuickWidget *mQQuickWidget;
    double mRotate;
};

#endif // CAMPASSWIDGET_H
