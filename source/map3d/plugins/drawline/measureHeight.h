#ifndef MEASUREHEIGHT_H
#define MEASUREHEIGHT_H

#include <osg/Group>
#include <osgEarth/GeoTransform>
#include <linenode.h>
#include "mapcontroller.h"
#include <osgEarthAnnotation/PlaceNode>

class MeasureHeight : public osg::Group
{
public:
    MeasureHeight(MapController *mapController);
    void setFirstPoint(osgEarth::GeoPoint P1);
    void setSecondPoint(osgEarth::GeoPoint P2);
    void clear();
    void draw();
    bool started() const;

private:
    double height();
private:
    MapController* mMapController{nullptr};
    LineNode* mHLine{nullptr};
    LineNode* mVLine{nullptr};
    osgEarth::GeoPoint mFirstPoint;
    osgEarth::GeoPoint mSecondPoint;
    osgEarth::GeoPoint mJointPoint;
    bool mStarted{false};

};

#endif // MEASUREHEIGHT_H
