#ifndef BASEMODEL_H
#define BASEMODEL_H

#include "drawshapes.h"

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
#include <osgFX/Outline>

#include <QMouseEvent>
#include <QObject>

class DefenseModelNode;

class ModelAnimationPathCallback: public osg::AnimationPathCallback
{
public:
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
};

class DefenseModelNode: public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    DefenseModelNode(MapController* mapControler, QObject* parent = nullptr);
    void setType(const QString &value);
    QString getType() const;
    void setQStringName(QString name);
    QString getQStringName();
    virtual void setGeographicPosition(const osgEarth::GeoPoint& pos, double heading);
    osg::Vec3d getGeographicPosition() const;



    virtual void stop(){}
    void collision();
    osgEarth::Annotation::ModelNode* getDragModelNode();


    static osg::ref_ptr<osg::Image> createColoredImage(osg::ref_ptr<osg::Image> sourceImage, osgEarth::Color color);


signals:
protected:
    virtual void playExplosionEffect(float scale);
public:
    virtual void frameEvent() {}
    virtual void mousePressEvent(QMouseEvent* event, bool onModel);
    virtual void mouseMoveEvent(QMouseEvent* event, bool onModel);
    void select(bool val);
    void hover(bool val);

protected:
    bool mIsSelected{false};
    bool mIs3D{false};
    osg::ref_ptr<osg::LOD> mRootNode;
    osg::ref_ptr<osg::Group> mNode3D;
    osg::ref_ptr<osg::Switch> mNode2D;
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mLabelNode;
    osgEarth::Color mModelColor{osgEarth::Color::White};

private:
    friend class ModelAnimationPathCallback;
    friend class PickHandler;

private:
    QString mType;
    osg::ref_ptr<osgFX::Outline> mSelectOutline;
    MapController *mMapControler;
};

#endif // BASEMODEL_H
