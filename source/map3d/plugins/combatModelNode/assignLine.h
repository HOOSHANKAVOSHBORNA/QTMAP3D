#ifndef ASSIGNLINE_H
#define ASSIGNLINE_H

#include "lineNode.h"
#include "drawCircle.h"

class AssignLine : public LineNode
{
public:
    AssignLine(MapItem *map);

    osg::Image* createSpotLight(const osg::Vec4& centerColor, unsigned int size, float power);

private:
    bool mLightVisible;
    MapItem *mMapItem;
};

#endif // ASSIGNLINE_H
