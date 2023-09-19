#include "snipTool.h"
#include "qquickwindow.h"
#include <QQuickImageProvider>
#include <QStandardPaths>
#include <QTimer>
#include "screenImageProvider.h"
#include <QQmlContext>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/WriteFile>

SnipTool::SnipTool(MapItem *map , QQmlEngine *engine):
    mMap(map),
    mEngine(engine)
{
    mReadyStatus = false;
}

osg::ref_ptr<osg::Image> SnipTool::takeSnapShot(int width , int height)
{
    auto mapHeight =  mMap->boundingRect().height();
    auto mapWidth =  mMap->boundingRect().width();
    auto point = mMap->mapToScene(QPoint(mapWidth/2 -width/2, mapHeight/2 -height/2));
    mOsgImage = new osg::Image();
    mViewCaptureCallback = new ViewCaptureCallback(mOsgImage, point ,width ,height);
    connect(mViewCaptureCallback,&ViewCaptureCallback::imageProcessComplete,this,&SnipTool::onImageProcessComplete);
    mCamera =  mMap->getViewer()->getCamera();
    mCamera->setFinalDrawCallback(mViewCaptureCallback);
    return mOsgImage;
}

bool SnipTool::saveImageFile(QString path)
{
    return osgDB::writeImageFile(*mOsgImage, (path+".png").toStdString());
}



void SnipTool::onImageProcessComplete(osg::Image *iImage , int width , int height)
{
    ScreenImageProvider *qmlImage = new ScreenImageProvider;
    qmlImage->setImage(iImage);
    QSize *imgSize = new QSize(width,height);
    qmlImage->requestPixmap( "SnipToolImage", imgSize,  QSize(width,height)) ;
    mEngine->addImageProvider("sniptoolprovider",qmlImage);
    mCamera->setFinalDrawCallback(NULL);
    mViewCaptureCallback->screenTaken = false;
}



SnipToolInterface::SnipToolInterface(MapItem *map , QQmlEngine *engine):
    mMap(map)
{
    qmlRegisterType<SnipTool>("Crystal", 1, 0, "Snipp");
    QQmlComponent component(engine, QUrl("qrc:/resources/SnipTool.qml"));
    mSnipTool = new SnipTool(mMap , engine);
    mObject = component.create();
    mObject->setProperty("snipp",QVariant::fromValue<SnipTool*>(mSnipTool));
    mObject->setProperty("component",QVariant::fromValue<QQmlComponent*>(&component));
    auto window = dynamic_cast<QQuickWindow*>(mObject);
    auto screenPoint = mMap->mapToGlobal(0,0);
    if(window){
        window->setGeometry(QRect(screenPoint.x()+(mMap->width()/2)-200, screenPoint.y()+(mMap->height()/2)-110, 400, 220));
        window->show();
    }
}

osg::ref_ptr<osg::Image> SnipToolInterface::takeSnipShot(int width,int height )
{
    return mSnipTool->takeSnapShot(width , height);
}

void SnipToolInterface::cancelSnipTool()
{
    mObject->deleteLater();
}
