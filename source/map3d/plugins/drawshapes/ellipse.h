#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <osgEarthAnnotation/EllipseNode>

class Ellipse: public osgEarth::Annotation::EllipseNode
{
public:
    Ellipse(bool clamp);
    void setColor(osgEarth::Color color);
    void setClamp(bool clamp);
    void setHeight(float height);

private:
    osgEarth::Symbology::Style ellipseStyle;
};

#endif // ELLIPSE_H
