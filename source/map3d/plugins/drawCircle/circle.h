#ifndef CIRCLE_H
#define CIRCLE_H
#include <osgEarthAnnotation/CircleNode>
#include "mapcontroller.h"

class Circle: public osgEarth::Annotation::CircleNode
{
public:
    Circle(MapController *mapController, bool clamp);

    void setCircleHeight(float hieght);
    void setColor(osgEarth::Color color);
    void setLineColor(osgEarth::Color color);
    void setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp);
    void setLineWidth(float width);

    float getCircleHeight();
    osgEarth::Color getColor();
    osgEarth::Color getLineColor();
    float getLineWidth();
private:
    osgEarth::Symbology::Style circleStyle;
    MapController* mMapController{nullptr};
};

#endif // Circle_H
