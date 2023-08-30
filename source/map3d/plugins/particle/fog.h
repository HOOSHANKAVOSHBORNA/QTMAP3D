#ifndef FOG_H
#define FOG_H


#include <QObject>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>




class Fog: public QObject , public osgEarth::Annotation::GeoPositionNode
{
    Q_OBJECT
public:
    Fog(MapItem *map);
    osg::ref_ptr<osgParticle::SmokeEffect> getFog();


public slots:
    void onModeChanged(bool is3DView);


private:

    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osgParticle::SmokeEffect> mFog{nullptr};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> m2DNode ;


    bool is3D;
};

#endif // FOG_H
