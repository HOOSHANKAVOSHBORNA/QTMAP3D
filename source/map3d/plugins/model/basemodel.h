#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <osgEarthAnnotation/ModelNode>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgEarth/Registry>

#include <QObject>

class ModelAnimationPathCallback: public osg::AnimationPathCallback
{
public:
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
};

class BaseModel: public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    BaseModel(osgEarth::MapNode* mapNode, QObject* parent = nullptr);
    void setType(const QString &value);
    QString getType() const;
    void setQStringName(QString name);
    QString getQStringName();
    void setGeographicPosition(const osg::Vec3d &pos);
    osg::Vec3d getGeographicPosition() const;
    bool hasHit() const;
    BaseModel *getFollowModel() const;
    void setFollowModel(BaseModel *followModel);
    virtual void stop() = 0;

signals:
    void positionChanged(osgEarth::GeoPoint pos);
    void hit(BaseModel *hitWith);
protected:
    virtual void playExplosionEffect(float scale);
private:
    void collision(BaseModel *collidedWith);
    friend class ModelAnimationPathCallback;
private:
    bool mHasHit{false};
    QString mType;
    BaseModel* mFollowModel{nullptr};

};

#endif // BASEMODEL_H
