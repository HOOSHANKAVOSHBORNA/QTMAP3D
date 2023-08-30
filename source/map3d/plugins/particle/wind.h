#ifndef WIND_H
#define WIND_H

#include <QObject>
#include "mapItem.h"
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/SmokeEffect>


class WindEffect :public QObject, public osgEarth::Annotation::GeoPositionNode
{
    Q_OBJECT
public:
    WindEffect(MapItem *map);
    osg::ref_ptr<osgParticle::SmokeEffect> getWind();
public slots:
    void onModeChanged(bool is3DView);
private:
    osg::ref_ptr<osgParticle::SmokeEffect> mWind;
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> m2DNode ;


    bool is3D;
};

#endif // WIND_H
