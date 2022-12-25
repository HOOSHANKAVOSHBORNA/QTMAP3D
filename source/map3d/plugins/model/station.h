#ifndef STATION_H
#define STATION_H


#include "mapcontroller.h"
#include "basemodel.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>


class Station: public BaseModel
{
public:
    Station(MapController *mapControler, QObject* parent = nullptr);
public:
    virtual void frameEvent()override;
protected:
private slots:
    void onModeChanged(bool is3DView);
private:
};


#endif // STATION_H
