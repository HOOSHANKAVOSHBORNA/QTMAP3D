#ifndef RAIN_H
#define RAIN_H


#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/PrecipitationEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>



class Rain: public osgEarth::Annotation::GeoPositionNode
{
public:
    Rain(osgEarth::Annotation::CircleNode *rangeLayer);
    osgParticle::PrecipitationEffect *getRain();
private:
    osg::ref_ptr<osgParticle::PrecipitationEffect> mRain;
};
#endif // RAIN_H
