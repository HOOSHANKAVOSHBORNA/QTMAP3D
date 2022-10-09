#include "locationwidget.h"
#include <QQmlEngine>
#include <QQuickWidget>
#include <QWidget>
#include <QQmlContext>
#include <osgEarth/optional>
#include <osgEarth/Viewpoint>
#include <QVBoxLayout>

LocationWidget::LocationWidget(QWidget *parent) : QQuickWidget(parent)
{
    QQmlContext  *context = this->rootContext();
    context->setContextProperty("Location", this);
    setSource(QUrl(QStringLiteral("qrc:/locationwidget/LoactionWidget.qml")));
    this->setResizeMode(QQuickWidget::SizeViewToRootObject);
    this->setAttribute(Qt::WA_AlwaysStackOnTop);
    this->setClearColor(Qt::transparent);
}

void LocationWidget::addLocation(QString name, double latitude, double longitude, double range)
{

     emit savePosition(name ,
                       QString::number(latitude,'f',3).toDouble(),
                       QString::number(longitude,'f',3).toDouble(),
                       range);
}

void LocationWidget::setClose()
{
    emit close();
}

void LocationWidget::setMousePosition(QString location)
{
    emit changePosition(location);
}

void LocationWidget::setCurrentLocation(double latitude, double longitude)
{
    double lat = QString::number(latitude,'f',3).toDouble();
    double lon = QString::number(longitude,'f',3).toDouble();
    emit currentLocation(lat,lon);
}
void LocationWidget::resizeEvent(QResizeEvent* event)
{
    QWidget* par = dynamic_cast<QWidget*>(parent());
    if(par)
        this->move(this->geometry().x(), par->height() - height()-13);
    QQuickWidget::resizeEvent(event);
}
