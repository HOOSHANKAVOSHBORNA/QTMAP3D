#ifndef RECT_H
#define RECT_H


#include <osgEarthAnnotation/RectangleNode>
#include "mapItem.h"

class Rect : public osgEarth::Annotation::RectangleNode
{
public:
    Rect(MapItem *mapItem, bool clamp);
    void setColor(osgEarth::Color color);
    osgEarth::Color getColor();
    void setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp);
    osgEarth::Symbology::AltitudeSymbol::Clamping getClamp();
    void setStrokeColor(osgEarth::Color color);
    osgEarth::Color getStrokeColor();
    void setStrokeWidth(float Width);
    float getStrokeWidth();

private:
    MapItem* mMapItem{nullptr};
};

#endif // RECT_H
