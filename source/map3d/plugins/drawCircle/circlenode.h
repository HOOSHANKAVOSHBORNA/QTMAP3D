#ifndef CIRCLENODE_H
#define CIRCLENODE_H

#include <osgEarthAnnotation/CircleNode>
#include "mapcontroller.h"

class CircleNode: public osgEarth::Annotation::CircleNode
{
public:
    void setCircleHeight(float hieght);
    CircleNode(MapController *mapController, bool clamp);
    void setColor(osgEarth::Color color);
    osgEarth::Color getColor();
    void setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp);

private:
    osgEarth::Symbology::Style circleStyle;
    MapController* mMapController{nullptr};
};

#endif // CIRCLENODE_H
