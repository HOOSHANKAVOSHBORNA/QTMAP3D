#include "snapShot.h"
#include "qquickwindow.h"
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>

#include <QQmlEngine>
#include <QTimer>

SnapShot::SnapShot(MapItem *mapItem, QObject *parent):
    QObject(parent),
    mMapItem(mapItem)
{
    mCaptureCallback  = new CaptureCallback();
    mMapItem->addCaptureCallback(mCaptureCallback);

    connect(mCaptureCallback, &CaptureCallback::imageProcessComplete, this, &SnapShot::onImageProcessComplete);
}

SnapShot::~SnapShot()
{
    mMapItem->removeCaptureCallback(mCaptureCallback);
    qDebug()<<"~SnapShot";
}

void SnapShot::capture()
{
    std::string directory = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last().toStdString() + "/QArch SnapShot/" + QDateTime::currentDateTime().date().toString().toStdString();
    std::string filename = "SnapShot ";
    QString timeNow = QDateTime::currentDateTime().time().toString();
    osgDB::makeDirectory(directory);
    QString filePath = QString::fromStdString(osgDB::getRealPath(directory + "/" + filename + timeNow.toStdString() + ".png"));

    auto point = mMapItem->mapToScene(QPoint(0,0));
    auto mapHeight =  mMapItem->boundingRect().height();
    auto mapWidth =  mMapItem->boundingRect().width();

    mCaptureCallback->capture(filePath, point, mapWidth, mapHeight);

    auto screenPoint = mMapItem->mapToGlobal(0,0);
    QQmlComponent component(new QQmlEngine(), QUrl("qrc:/resources/SnapShot.qml"));
    mCaptureWindow = component.create();
    auto window = dynamic_cast<QQuickWindow*>(mCaptureWindow);
    if(window){
        window->setGeometry(QRect(screenPoint.x(), screenPoint.y(), mMapItem->width(), mMapItem->height()));
        window->show();
    }
}

void SnapShot::onImageProcessComplete(QString filePath)
{
    mCaptureWindow->deleteLater();
}
