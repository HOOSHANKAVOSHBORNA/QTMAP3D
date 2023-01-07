#ifndef STATION_H
#define STATION_H


#include "mapcontroller.h"
#include "basemodel.h"
#include "datamanager.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>


class StationModelNode: public BaseModel
{
public:
    StationModelNode(MapController *mapControler, QObject* parent = nullptr);
    void setInformation(const StationInfo &info);
public:
    virtual void frameEvent()override;
protected:
private slots:
    void onModeChanged(bool is3DView);
private:
    StationInfo mInformation;
};


#endif // STATION_H
