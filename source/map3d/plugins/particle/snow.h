#ifndef SNOW_H
#define SNOW_H

#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/PrecipitationEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>



   class Snow: public osgEarth::Annotation::GeoPositionNode
{
public:
    Snow(MapItem *map);
    osgParticle::PrecipitationEffect *getSnow();
private:
    osg::ref_ptr<osgParticle::PrecipitationEffect> mSnow;
};


#endif // SNOW_H
