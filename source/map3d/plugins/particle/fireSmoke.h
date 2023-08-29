#ifndef FIRESMOKE_H
#define FIRESMOKE_H


#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>



class FireSmoke: public osgEarth::Annotation::GeoPositionNode
{
public:
    FireSmoke(MapItem *map);
    osg::PositionAttitudeTransform *getFireSmoke();
private:
    osg::ref_ptr<osgParticle::FireEffect> mFire;
    osg::ref_ptr<osgParticle::SmokeEffect> mSmoke;
};




#endif // FIRESMOKE_H
