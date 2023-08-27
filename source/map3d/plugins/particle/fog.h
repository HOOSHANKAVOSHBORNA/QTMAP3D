#ifndef FOG_H
#define FOG_H


#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/PrecipitationEffect>
//#include <osgParticle/ParticleEffect>
#include <osg/Fog>
#include <mapItem.h>



class Fog : public osgEarth::Annotation::GeoPositionNode
{
public:
    Fog(MapItem *map);
    osg::Fog *getFog();
private:
    osg::ref_ptr<osgParticle::PrecipitationEffect> mFogEffect;
    osg::ref_ptr<osg::Fog> mFog;
};
#endif // FOG_H
