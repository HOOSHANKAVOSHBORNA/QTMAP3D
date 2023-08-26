#ifndef PARTICLE_H
#define PARTICLE_H


#include "compositeAnnotationLayer.h"
#include "explosion.h"
#include "fireSmoke.h"
#include "plugininterface.h"
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/AnnotationLayer>
#include <QObject>


class Particle: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "particle.json")
    Q_INTERFACES(PluginInterface)

public:
    enum class State{
        NONE,
        READY,
        ADDING,
        CANCEL,
        CONFIRM
    };

    enum class Mode{
        NONE,
        FIRE,
        EXPLOSION
    };

    Particle(QObject *parent = nullptr);
    bool setup() override;
    bool mousePressEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;


public slots:
    void onExplodeClicked(bool check);
    void onFireClicked(bool check);

protected:
    void add(const osgEarth::GeoPoint &geoPos);
    void moving(const osgEarth::GeoPoint &geoPos);
    void confirm();
    void cancelAdd();


private:
    State mState{State::NONE};
    Mode mMode{Mode::NONE};
    Explosion *mExplosion;
    FireSmoke *mFire;
    osg::ref_ptr<ParenticAnnotationLayer> mParticleLayer{nullptr};


};


#endif // PARTICLE_H
