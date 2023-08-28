#ifndef CLOUD_H
#define CLOUD_H


#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>



class Cloud: public osgEarth::Annotation::GeoPositionNode
{
public:
    Cloud(MapItem *map);
    osg::ref_ptr<osgParticle::SmokeEffect> getCloud();
private:
    osg::ref_ptr<osgParticle::SmokeEffect> mCloud;
};

#endif // CLOUD_H
