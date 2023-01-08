#ifndef CIRCLE_H
#define CIRCLE_H
#include <osgEarthAnnotation/CircleNode>
#include "mapcontroller.h"

class Circle: public osgEarth::Annotation::CircleNode
{
public:
    void setCircleHeight(float hieght);
    Circle(MapController *mapController, bool clamp);
    void setColor(osgEarth::Color color);
    void setClamp(bool clamp);

private:
    osgEarth::Symbology::Style circleStyle;
    MapController* mMapController{nullptr};
};

#endif // Circle_H
