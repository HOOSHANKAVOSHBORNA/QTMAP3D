#ifndef WIND_H
#define WIND_H

#include <QObject>
#include "mapItem.h"
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/SmokeEffect>

class AutoScaler;

class WindEffect :public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    WindEffect(MapItem *map, double duration = 30);
    ~WindEffect();
    void setScaleRatio(double ratio);

public slots:
    void onModeChanged(bool is3DView);
private:
    osg::ref_ptr<osg::Group> mRoot{nullptr};
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<osg::Image> mImage;
    osg::ref_ptr<osgParticle::SmokeEffect> mWind{nullptr};


    bool mIs3D;
    AutoScaler* mAutoScaler;
    QTimer *mTimerDuration{nullptr};
};

#endif // WIND_H
