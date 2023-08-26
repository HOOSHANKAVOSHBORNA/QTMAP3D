#ifndef SMOKE_H
#define SMOKE_H

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/SmokeEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>



class Smoke: public osgEarth::Annotation::GeoPositionNode
{

public:
    Smoke(MapItem *map);
    osgParticle::SmokeEffect *getSmoke();

private:
    osg::ref_ptr<osgParticle::SmokeEffect> mSmoke;
};

#endif // SMOKE_H
