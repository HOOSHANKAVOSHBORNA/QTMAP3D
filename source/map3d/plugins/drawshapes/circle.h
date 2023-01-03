#ifndef CIRCLE_H
#define CIRCLE_H
#include <osgEarthAnnotation/CircleNode>

class Circle: public osgEarth::Annotation::CircleNode
{
public:
    void setCircleHeight(float hieght);
    Circle(bool clamp);
    void setColor(osgEarth::Color color);
    void setClamp(bool clamp);

private:
    osgEarth::Symbology::Style circleStyle;
};

#endif // Circle_H
