#ifndef SNOW_H
#define SNOW_H

#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/PrecipitationEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>
#include <osgParticle/SmokeEffect>



   class Snow: public osgEarth::Annotation::GeoPositionNode
{
public:
    Snow(osgEarth::Annotation::CircleNode *map);
    osgParticle::PrecipitationEffect *getSnow();
    void removeSnow(MapItem *map);
private:
    osg::ref_ptr<osgParticle::PrecipitationEffect> mSnow;
};


#endif // SNOW_H
