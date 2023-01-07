#ifndef STATION_H
#define STATION_H


#include "mapcontroller.h"
#include "basemodel.h"
#include "datamanager.h"
#include "stationinformation.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>


class StationModelNode: public BaseModel
{
public:
    StationModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle* uiHandle, QObject* parent = nullptr);
    void setInformation(const StationInfo &info);
public slots:

    void onLeftButtonClicked(bool val);
public:
    virtual void frameEvent()override;
    void mousePressEvent(QMouseEvent *event, bool onModel) override;
protected:
private slots:
    void onModeChanged(bool is3DView);
private:
    void showInfoWidget();
private:
    MapController* mMapController{nullptr};
    StationInfo mInformation;
    UIHandle* mUIHandle;
    QQmlEngine *mQmlEngine;
};


#endif // STATION_H
