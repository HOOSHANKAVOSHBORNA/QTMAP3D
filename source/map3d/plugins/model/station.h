#ifndef STATION_H
#define STATION_H



#include "basemodel.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>


class Station: public BaseModel
{
public:
    Station(osgEarth::MapNode* mapNode, QObject* parent = nullptr);
protected:
private:
private:
};


#endif // STATION_H
