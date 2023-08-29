#ifndef CLOUD_H
#define CLOUD_H


#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>
#include <osgEarth/Lighting>
#include <osg/LightSource>



class Cloud: public osgEarth::Annotation::GeoPositionNode
{
public:
    Cloud(MapItem *map);
    osg::ref_ptr<osgParticle::SmokeEffect> getCloud();
private:
    osg::ref_ptr<osgParticle::SmokeEffect> mCloud1;
    osg::ref_ptr<osgParticle::SmokeEffect> mCloud2;
    osg::ref_ptr<osgParticle::SmokeEffect> mCloud3;

//    osgEarth::LightGL3 *mThunder;
//    osg::ref_ptr<osg::LightSource> mLightning ;
//    osg::ref_ptr<osg::Light> mLight;
};

#endif // CLOUD_H
