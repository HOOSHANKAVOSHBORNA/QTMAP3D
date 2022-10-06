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
    connect(this ,&LocationWidget::onCurrentClicked,[=]{
        qDebug()<<this->size();
    });

}

void LocationWidget::addViewPoint(osgEarth::Viewpoint point)
{
     osgEarth::optional<std::string> str = point.name();
     QString name = QString::fromStdString(str.get());
     mListViewpoint.append(&point);
     double x = point.focalPoint()->x();
     double y = point.focalPoint()->y();
     emit savePosition(name , x ,y);
}

void LocationWidget::setClose()
{
    emit close();
}

void LocationWidget::setMousePosition(QString location)
{
    emit changePosition(location);
}
void LocationWidget::resizeEvent(QResizeEvent* event)
{
    QWidget* par = dynamic_cast<QWidget*>(parent());
    if(par)
        this->move(this->geometry().x(), par->height() - height()-12);
    QQuickWidget::resizeEvent(event);
}
