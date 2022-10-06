#include "locationwidget.h"
#include <QQmlEngine>
#include <QQuickWidget>
#include <QQmlContext>
#include <osgEarth/optional>
#include <osgEarth/Viewpoint>
#include <QVBoxLayout>

LocationWidget::LocationWidget(QWidget *parent) : QQuickWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
//    mQQuickWidget = new QQuickWidget(QUrl(QStringLiteral("qrc:/locationwidget/LoactionWidget.qml")),this);
    setSource(QUrl(QStringLiteral("qrc:/locationwidget/LoactionWidget.qml")));
    this->setResizeMode(QQuickWidget::SizeViewToRootObject);
    this->setAttribute(Qt::WA_AlwaysStackOnTop);
    this->setClearColor(Qt::transparent);
//    mQQuickWidget->resize(230, 60);
//    mQQuickWidget->raise();
//    setMinimumSize(300,60);
    qDebug()<<this->size();
    //setMaximumSize(230,300);
//    qDebug()<<;
    connect(this ,&LocationWidget::onCurrentClicked,[=]{
        qDebug()<<this->size();
    });
    this->engine()->rootContext()->setContextProperty("Location",this);
//    layout->addWidget(mQQuickWidget);
//    connect(this,&LocationWidget::onCurrentClicked,[=](QString name){
//        foreach (osgEarth::Viewpoint* i, mListViewpoint) {
//            osgEarth::optional<std::string> str = i->name();
//            QString tempNamp = QString::fromStdString(str.get());
//            if (tempNamp == name){
//                onClickedPosition(i);
//                break;
//            }
//        }
//    });


//    connect(this,&LocationWidget::onOpenWidget,[=](bool a ,bool b , bool c ){
//        if (b ){
//            mQQuickWidget->resize(230, 300);
//            mQQuickWidget->raise();
//            resize(230,300);
//            raise();
////            this->move(0,parent->height() - this->height());

//        }else if (a) {
//            mQQuickWidget->resize(230, 120);
//            mQQuickWidget->raise();
//            resize(230,120);
//            raise();
////            this->move(0,parent->height() - this->height());
//        }else if (c){
//            mQQuickWidget->resize(230, 120);
//            mQQuickWidget->raise();
//            resize(230,120);
//            raise();
////            this->move(0,parent->height() - this->height());
//        }
//        else{
//            mQQuickWidget->resize(230, 60);
//            mQQuickWidget->raise();
//            resize(230,60);
//            raise();
////            this->move(0,parent->height() - this->height());
//        }



//    });
}

void LocationWidget::addViewPoint(osgEarth::Viewpoint point)
{
     osgEarth::optional<std::string> str = point.name();
     QString name = QString::fromStdString(str.get());
     mListViewpoint.append(&point);
     double x = point.focalPoint()->x();
     double y = point.focalPoint()->y();
     emit savePosition(name , x ,y);
     qDebug()<<this->size();
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
