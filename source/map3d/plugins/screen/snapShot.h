#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <QObject>
#include "qqmlengine.h"
#include "viewCaptureCallback.h"
#include "QStandardPaths"
#include "mapItem.h"

class SnapShot : public QObject
{
    Q_OBJECT
public:
    SnapShot(MapItem *map);

private slots:
    void onImageProcessComplete(osg::Image *iImage);
    void takingProcessFinished();

private:
    osg::Camera* mCamera;
    ViewCaptureCallback* mViewCaptureCallback{nullptr};
    std::string mFileDialog;
    QQmlEngine mEngine;
    QObject *mObject;
    bool mReadyStatus;
    QTimer *timer;
};

#endif // SNAPSHOT_H
