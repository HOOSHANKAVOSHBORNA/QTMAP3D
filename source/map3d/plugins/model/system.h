#ifndef SYSTEM_H
#define SYSTEM_H


#include "basemodel.h"
#include "map3dwidget.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>


class System: public BaseModel
{
public:
    System(osgEarth::MapNode* mapNode, QObject* parent = nullptr);
protected:
private:
private:
};

#endif // SYSTEM_H
