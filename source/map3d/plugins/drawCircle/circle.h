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
    osgEarth::Color getColor();
    void setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp);

private:
    osgEarth::Symbology::Style circleStyle;
    MapController* mMapController{nullptr};
    osgEarth::Color mColor;
};

#endif // CIRCLENODE_H
