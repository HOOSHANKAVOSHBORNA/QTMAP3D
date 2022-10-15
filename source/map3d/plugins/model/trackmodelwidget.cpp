#include "trackmodelwidget.h"
#include <QQmlEngine>
#include <QQuickWidget>
#include <QQmlContext>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
TrackModelWidget::TrackModelWidget(QWidget *parent)
    :QQuickWidget(parent)

{
    QQmlContext  *context = this->rootContext();
    context->setContextProperty("DetaliObject", this);
    setSource(QUrl("qrc:/trackwidget/trackwidget.qml"));
    this->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->setAttribute(Qt::WA_AlwaysStackOnTop);
    this->setClearColor(Qt::transparent);
    connect(this ,&TrackModelWidget::onPin,[=](bool t){
        if(t){
            emit isDock(parent->height());
        }
    });

}

void TrackModelWidget::addModel( QString type, QString name)
{

    emit modelAdded(type,name);
}

void TrackModelWidget::setModelPosition(QString t, QString n, double latitude, double longitude, double altitude)
{
    double lat = QString::number(latitude,'f',4).toDouble();
    double lon = QString::number(longitude,'f',4).toDouble();
    double alt = QString::number(altitude,'f',4).toDouble();
    emit modelPosition(t,n,lat,lon,alt);
}

void TrackModelWidget::setClose()
{
    emit close();
}

void TrackModelWidget::setModelInfo(QString type , QString name,QJsonObject info)
{
    QStringList keyInfo;
    QStringList valueInfo;

    keyInfo = info.keys();
    foreach (QString i, info.keys()) {
        valueInfo.append(info.value(i).toString());
    }
    QString string= stringListToString(keyInfo,valueInfo);
    modelInfo(type,name,string);
}

void TrackModelWidget::setMinimaizeWidget(bool isMax)
{
    emit minimize(isMax);
}

void TrackModelWidget::setUnTrackAll(bool isCheck)
{
    emit unTrackAll(isCheck);
}

void TrackModelWidget::resizeEvent(QResizeEvent *event)
{
    QQuickWidget::resizeEvent(event);

}

QString TrackModelWidget::stringListToString(QStringList key, QStringList value)
{
    QString string ;
    for (int i=0;i<key.length();++i) {
        string+=key[i] + " : " + value[i];
        string=string + "\n";
    }
    return  string;
}












