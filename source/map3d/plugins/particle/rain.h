#ifndef RAIN_H
#define RAIN_H


#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/PrecipitationEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>



class Rain: public osgEarth::Annotation::GeoPositionNode
{
public:
    Rain(MapItem *map);
    osgParticle::PrecipitationEffect *getRain();
    void removeRain(MapItem *map);
private:
    osg::ref_ptr<osgParticle::PrecipitationEffect> mRain;
};
#endif // RAIN_H
