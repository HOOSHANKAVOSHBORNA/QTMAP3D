#ifndef RECT_H
#define RECT_H
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarth/Color>

class Rect : public osgEarth::Annotation::RectangleNode
{
public:
    Rect(bool clamp, double width, double height);
    void setColor(osgEarth::Color color);
    void setClamp(bool clamp);

private:

    osgEarth::Symbology::Style rectStyle;
};
#endif // RECT_H
