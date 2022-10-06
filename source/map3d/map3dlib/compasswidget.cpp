#include "compasswidget.h"

#include "QQuickWidget"
#include <QQmlEngine>
#include <QQmlContext>

CompassWidget::CompassWidget(QWidget *parent):
    QWidget(parent)
{
    mRotate=0.0;
    mQQuickWidget = new QQuickWidget(this);
    QQmlContext  *context = mQQuickWidget->rootContext();
    context->setContextProperty("GetData", this);
    mQQuickWidget->setSource(QUrl("qrc:/Compass.qml"));
    mQQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    mQQuickWidget->setAttribute(Qt::WA_AlwaysStackOnTop);
    mQQuickWidget->setClearColor(Qt::transparent);
    mQQuickWidget->resize(60, 60);
    mQQuickWidget->raise();

    /// set data class datamanager to main qml
    mQQuickWidget->setMinimumSize(60,60);
    mQQuickWidget->setMaximumSize(60,60);

    //QWidget *widget =createWindowContainer(mQQuickWidget, this);
    setMinimumSize(60,60);
    setMaximumSize(60,60);

}

void CompassWidget::setRotate( double rot)
{

    mRotate = rot;
    emit rotateChange(rot);

}

void CompassWidget::setPoint(double point)
{
    mRotate = point;
    emit pointChange(point);
}

double CompassWidget::getRotate()
{
    return mRotate;
}
