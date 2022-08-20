#ifndef CAMPASSWIDGET_H
#define CAMPASSWIDGET_H

#include <QWidget>
#include <QQuickView>
#include "QQuickWidget"
//  library for display qml  in widget
#include <QQmlContext>
#include <QQmlEngine>
class CompassWidget : public QWidget
{
    Q_OBJECT
public:
    CompassWidget(QWidget *parent = nullptr);
    void setRotate(double rotate);
    void setPoint(double point);
signals:
    void rotateChange(double newRotate);
    void pointChange(double newRotate);
public slots:
    Q_INVOKABLE double getRotate();

private:
    QQuickWidget *mQQuickWidget;
    double mRotate;
};

#endif // CAMPASSWIDGET_H
