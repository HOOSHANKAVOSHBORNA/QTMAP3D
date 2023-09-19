#ifndef SNIPTOOL_H
#define SNIPTOOL_H

#include "qqmlengine.h"
//#include "qtimer.h"
#include "viewCaptureCallback.h"
#include <QObject>
#include "mapItem.h"

class SnipTool : public QObject
{
    Q_OBJECT
public:
    SnipTool(MapItem *map, QQmlEngine *engine);
    Q_INVOKABLE osg::ref_ptr<osg::Image> takeSnapShot(int width, int height);
    Q_INVOKABLE bool saveImageFile(QString path);

private slots:
    void onImageProcessComplete(osg::Image *iImage, int width, int height);
    void takingProcessFinished();

signals:

private:
    osg::Camera* mCamera;
    ViewCaptureCallback* mViewCaptureCallback{nullptr};
    std::string mFileDialog;
    QQmlEngine *mEngine;
    osg::ref_ptr<osg::Image> mOsgImage ;
    bool mReadyStatus;
    MapItem *mMap;
};

class SnipToolInterface : public QObject
{
    Q_OBJECT
public:
    SnipToolInterface(MapItem *map, QQmlEngine *engine);

private slots:
    osg::ref_ptr<osg::Image> takeSnipShot(int width = 400,int height =220);
    void cancelSnipTool();

private:
    QObject *mObject;
    QImage *mImage;
    std::string mFilePath;
    SnipTool *mSnipTool{nullptr};
    MapItem *mMap;
};

#endif // SNIPTOOL_H
