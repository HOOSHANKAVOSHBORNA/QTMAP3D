#ifndef ASSIGNLINE_H
#define ASSIGNLINE_H

#include <osgEarth/LineDrawable>
#include "combatManager.h"

class AssignLine : public osgEarth::Annotation::AnnotationNode
{
public:
    AssignLine(osgEarth::GeoPoint start,osgEarth::GeoPoint end, AssignState state);
    void setState(AssignState state);
    void setLinePosition(osgEarth::GeoPoint start,osgEarth::GeoPoint end);

private:
    osgEarth::LineDrawable *mLine{nullptr};
};

#endif // ASSIGNLINE_H
