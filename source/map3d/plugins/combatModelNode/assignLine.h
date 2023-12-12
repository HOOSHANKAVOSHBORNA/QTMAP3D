#ifndef ASSIGNLINE_H
#define ASSIGNLINE_H

#include <osgEarth/LineDrawable>
#include "combatManager.h"


class AssignLine : public osgEarth::LineDrawable
{
public:
    AssignLine(AssignState state);
    void setState(AssignState state);

};

#endif // ASSIGNLINE_H
