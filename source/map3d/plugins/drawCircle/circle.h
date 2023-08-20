#ifndef CIRCLE_H
#define CIRCLE_H
#include <osgEarthAnnotation/CircleNode>
#include "mapItem.h"



class Circle: public osgEarth::Annotation::CircleNode
{
public:
    Circle();

    void setHeight(float hieght);
    void setFillColor(osgEarth::Color color);
    void setStrokeColor(osgEarth::Color color);
    void setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp);
    void setStrokeWidth(float width);
    float getHeight();
    osgEarth::Color getFillColor();
    osgEarth::Color getStrokeColor();
    float getStrokeWidth();

private:
    osgEarth::Symbology::Style circleStyle;
    MapItem* mMapItem{nullptr};
};

#endif // Circle_H
