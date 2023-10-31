#include "snapShot.h"
#include "qquickwindow.h"

#include <QTimer>

SnapShot::SnapShot(MapItem *map)
{
    mReadyStatus = false;
    auto point = map->mapToScene(QPoint(0,0));
    auto mapHeight =  map->boundingRect().height();
    auto mapWidth =  map->boundingRect().width();
    osg::ref_ptr<osg::Image> img = new osg::Image();
    mViewCaptureCallback = new ViewCaptureCallback(img, point,mapWidth ,mapHeight);
    connect(mViewCaptureCallback,&ViewCaptureCallback::imageProcessComplete,this,&SnapShot::onImageProcessComplete);
    mCamera =  map->getViewer()->getCamera();
    mCamera->setFinalDrawCallback(mViewCaptureCallback);
    auto screenPoint = map->mapToGlobal(0,0);
    qDebug() << screenPoint;
    QQmlComponent component(&mEngine, QUrl("qrc:/resources/SnapShot.qml"));
    mObject = component.create();
    auto window = dynamic_cast<QQuickWindow*>(mObject);
    if(window){
        window->setGeometry(QRect(screenPoint.x(), screenPoint.y(), map->width(), map->height()));
        window->show();
    }
    timer = new QTimer(this);
    timer->singleShot(50, this, SLOT(takingProcessFinished()));
    timer->deleteLater();

}

void SnapShot::takingProcessFinished()
{
    mObject->deleteLater();
}

void SnapShot::onImageProcessComplete(osg::Image *iImage)
{
    std::string directory = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last().toStdString() + "/QArch SnapShot/" + QDateTime::currentDateTime().date().toString().toStdString();
    std::string filename = "SnapShot ";
    QString timeNow = QDateTime::currentDateTime().time().toString();
    osgDB::makeDirectory(directory);
    mFileDialog = osgDB::getRealPath(directory + "/" + filename + timeNow.toStdString() + ".png");
    bool resultSnap = osgDB::writeImageFile(*iImage, mFileDialog);


    mCamera->setFinalDrawCallback(NULL);
    mViewCaptureCallback->screenTaken = false;
}
