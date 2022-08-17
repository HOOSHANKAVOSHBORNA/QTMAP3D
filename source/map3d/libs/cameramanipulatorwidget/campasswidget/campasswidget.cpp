#include "campasswidget.h"

CampassWidget::CampassWidget(QWidget *parent):
    QWidget(parent)
{
    mRotate=0.0;
    mQQuickWidget = new QQuickWidget(QUrl(QStringLiteral("qrc:/maincampass.qml")),this);
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

void CampassWidget::setRotate( double rot)
{

     mRotate =rot;
     emit rotateChange(rot);

}

double CampassWidget::getRotate()
{
    return mRotate;
}
