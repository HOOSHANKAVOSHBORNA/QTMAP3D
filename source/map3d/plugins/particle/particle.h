#ifndef PARTICLE_H
#define PARTICLE_H


#include "compositeAnnotationLayer.h"
#include "explosion.h"
#include "fireSmoke.h"
#include "snow.h"
#include "rain.h"
#include "cloud.h"
#include "wind.h"
#include "fog.h"
#include "plugininterface.h"
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/AnnotationLayer>
#include <QObject>

#define CATEGORY  "Particle"
#define EXPLOSION "Explosion"
#define FIRE      "Fire"
#define SNOW      "Snow"
#define RAIN      "Rain"
#define CLOUD     "Cloud"
#define WIND      "Wind"
#define FOG       "Fog"


class Particle: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "particle.json")
    Q_INTERFACES(PluginInterface)

public:
    enum class State{
        None,
        Ready,
        Init
    };

    enum class Type{
        None,
        Fire,
        Explosion,
        Snow,
        Rain,
        Cloud,
        Wind,
        Fog
    };

    Particle(QObject *parent = nullptr);
    bool setup() override;
    bool mouseClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;

public slots:
    void onExplodeClicked(bool check);
    void onFireClicked(bool check);
    void onSnowClicked(bool check);
    void onRainClicked(bool check);
    void onCloudClicked(bool check);
    void onWindClicked(bool check);
    void onFogClicked(bool check);

    void onExplosionDataReceived(ExplosionData *explosionData);

protected:
    void init(const osgEarth::GeoPoint &geoPos);
//    void moving(const osgEarth::GeoPoint &geoPos);
    void confirm();
    void cancel();


private:
//    MapItem *mapItem() const;
    State mState{State::None};
    Type mType{Type::None};
    osg::ref_ptr<Explosion> mExplosion;
    osg::ref_ptr<FireSmoke> mFire;
    osg::ref_ptr<Snow> mSnow;
    osg::ref_ptr<Cloud> mCloud;
    osg::ref_ptr<WindEffect> mWind;
    osg::ref_ptr<Rain> mRain;
    osg::ref_ptr<Fog> mFog;

    osg::ref_ptr<ParenticAnnotationLayer> mParticleLayer{nullptr};


};


#endif // PARTICLE_H
