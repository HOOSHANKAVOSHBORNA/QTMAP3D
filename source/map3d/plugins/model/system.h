#ifndef SYSTEM_H
#define SYSTEM_H

#include "mapcontroller.h"
#include "basemodel.h"

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>


class System: public BaseModel
{
public:
    System(MapController *mapControler, QObject* parent = nullptr);
public:
    virtual void frameEvent()override;
protected:
private slots:
    void onModeChanged(bool is3DView);
private:
};

#endif // SYSTEM_H
