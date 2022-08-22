#include "compasswidget.h"

CompassWidget::CompassWidget(QWidget *parent):
    QWidget(parent)
{
    mRotate=0.0;
    mQQuickWidget = new QQuickWidget(QUrl(QStringLiteral("qrc:/Compass.qml")),this);
    mQQuickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    mQQuickWidget->setAttribute(Qt::WA_AlwaysStackOnTop);
    mQQuickWidget->setClearColor(Qt::transparent);
    mQQuickWidget->resize(60, 60);
    mQQuickWidget->raise();

    /// set data class datamanager to main qml
    mQQuickWidget->engine()-> rootContext()->setContextProperty("GetData",this);
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
