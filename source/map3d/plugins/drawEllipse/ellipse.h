#ifndef ELLIPSE_H
#define ELLIPSE_H
#include <osgEarthAnnotation/EllipseNode>
#include "mapItem.h"

class Ellipse : public osgEarth::Annotation::EllipseNode
{
public:
    Ellipse();
    void setColor(osgEarth::Color color);
    osgEarth::Color getColor();
    void setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp);
    osgEarth::Symbology::AltitudeSymbol::Clamping getClamp();
    void setHeight(float height);
    float getHeight();
    void setStrokeColor(osgEarth::Color color);
    osgEarth::Color getStrokeColor();
    void setStrokeWidth(float Width);
    float getStrokeWidth();

    void setRadius(const osgEarth::Linear &radius);
    const osgEarth::Linear &getRadius() const;


private:
    MapItem* mMapItem{nullptr};
};

#endif // ELLIPSE_H
