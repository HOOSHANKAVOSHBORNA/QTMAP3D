#ifndef RAIN_H
#define RAIN_H

#include <QObject>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgParticle/PrecipitationEffect>
#include <osgParticle/ParticleEffect>
#include <mapItem.h>



class Rain:public QObject, public osgEarth::Annotation::GeoPositionNode
{
    Q_OBJECT
public:
    Rain(MapItem *map);
    osgParticle::PrecipitationEffect *getRain();
private:
    osg::ref_ptr<osgParticle::PrecipitationEffect> mRain;
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> m2DNode ;
    osg::ref_ptr<osgEarth::Annotation::CircleNode> precipitationRange;


    bool is3D;

public slots:
    void onModeChanged(bool is3DView);

};
#endif // RAIN_H
