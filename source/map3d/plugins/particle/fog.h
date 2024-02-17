#ifndef FOG_H
#define FOG_H


#include <QObject>
#include <osgEarthAnnotation/ModelNode>
#include <osgParticle/SmokeEffect>
#include <mapItem.h>


class AutoScaler;

class Fog: public QObject , public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    Fog(MapItem *map, double duration = 30);
    ~Fog() override;
    void setScaleRatio(double ratio);

public slots:
    void onModeChanged(bool is3DView);

private:
    osg::ref_ptr<osg::Group> mRoot{nullptr};
    osg::ref_ptr<osg::Switch> mSwitchNode{nullptr};
    osg::ref_ptr<osg::Geode> m2DNode;
    osg::ref_ptr<osg::Image> mImage;
    osg::ref_ptr<osgParticle::SmokeEffect> mFog{nullptr};

    bool mIs3D;
    AutoScaler* mAutoScaler;
    QTimer *mTimerDuration{nullptr};

};

#endif // FOG_H
