#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <osgEarthAnnotation/ModelNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgEarth/Registry>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>

#include <QObject>
class BaseModel;

class ModelAnimationPathCallback: public osg::AnimationPathCallback
{
public:
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
};

class  PickHandler: public osgGA::GUIEventHandler
{
public:
    PickHandler(){}
    virtual ~PickHandler()override{}
protected:
    bool  handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
private:
    void pick(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea);
    void findSceneModels(osgViewer::Viewer* viewer);
private:
    BaseModel* mLastPushModel{nullptr};
    BaseModel* mLastMoveModel{nullptr};
    BaseModel* mCurrentModel{nullptr};
    int mPreRange{0};
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
    virtual void setGeographicPosition(const osg::Vec3d &pos, double heading);
    osg::Vec3d getGeographicPosition() const;
    bool hasHit() const;
    BaseModel *getFollowModel() const;
    void setFollowModel(BaseModel *followModel);
    virtual void stop(){}

signals:
    void positionChanged(osgEarth::GeoPoint pos);
    void hit(BaseModel *hitWith);
protected:
    virtual void playExplosionEffect(float scale);
    virtual void mousePushEvent(bool /*onModel*/, const osgGA::GUIEventAdapter& /*ea*/);
//    void mouseReleaseEvent(QMouseEvent* event);
//    void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(bool /*onModel*/, const osgGA::GUIEventAdapter& /*ea*/);
    virtual void cameraRangeChanged(double /*range*/);
    virtual void curentPosition(osgEarth::GeoPoint pos);
protected:
    bool mIsSelected{false};
    bool mIs3d{false};
    bool mCameraRangeChangeable{false};
    osg::ref_ptr<osg::Switch> mRoot;
private:
    void collision(BaseModel *collidedWith);
    void select(bool val);
    friend class ModelAnimationPathCallback;
    friend class PickHandler;
private:
    bool mHasHit{false};
    QString mType;
    BaseModel* mFollowModel{nullptr};
};

#endif // BASEMODEL_H
