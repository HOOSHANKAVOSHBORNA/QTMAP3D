#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <QObject>
//#include "viewCaptureCallback.h"
#include "QStandardPaths"
#include "mapItem.h"

class SnapShot : public QObject
{
    Q_OBJECT
public:
    SnapShot(MapItem *mapItem, QObject *parent = nullptr);
    ~SnapShot();

    void capture();
private slots:
    void onImageProcessComplete(QString filePath);

private:
    CaptureCallback * mCaptureCallback{nullptr};
    MapItem *mMapItem{nullptr};
    QObject *mCaptureWindow{nullptr};
};

#endif // SNAPSHOT_H
