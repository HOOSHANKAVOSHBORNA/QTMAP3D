#ifndef SNOW_H
#define SNOW_H

#include <QObject>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/PrecipitationEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>



class Snow:public QObject, public osgEarth::Annotation::GeoPositionNode
{
    Q_OBJECT
public:
    Snow(MapItem *map);
    osgParticle::PrecipitationEffect *getSnow();
private:
    osg::ref_ptr<osgParticle::PrecipitationEffect> mSnow;
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> m2DNode ;

    osg::ref_ptr<osgEarth::Annotation::CircleNode> precipitationRange;


    bool is3D;

public slots:
    void onModeChanged(bool is3DView);
};


#endif // SNOW_H
