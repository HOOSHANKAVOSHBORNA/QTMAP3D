﻿#ifndef BASEMODEL_H
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

//class  PickHandler: public osgGA::GUIEventHandler
//{
//public:
//    PickHandler(){}
//    virtual ~PickHandler()override{}
//protected:
//    bool  handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
//private:
//    void pick(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea);
//    void findSceneModels(osgViewer::Viewer* viewer);
//private:
//    BaseModel* mLastPushModel{nullptr};
//    BaseModel* mLastMoveModel{nullptr};
//    BaseModel* mCurrentModel{nullptr};
//    int mPreRange{0};
//};

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
//    bool hasHit() const;
//    DefenseModelNode *getFollowModel() const;
//    void setFollowModel(DefenseModelNode *followModel);
    virtual void stop(){}
    void collision();
    osgEarth::Annotation::ModelNode* getDragModelNode();
//    virtual void traverse(osg::NodeVisitor& nv) override;

signals:
//    void positionChanged(osgEarth::GeoPoint pos);
//    void hit(DefenseModelNode *hitWith);
protected:
//    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
    virtual void playExplosionEffect(float scale);
public:
    virtual void frameEvent() {}
    virtual void mousePressEvent(QMouseEvent* event, bool onModel);
//    virtual void mouseDoubleClickEvent(QMouseEvent* event, bool onModel);
    virtual void mouseMoveEvent(QMouseEvent* event, bool onModel);
//    virtual void cameraRangeChanged(double /*range*/);
//    virtual void curentPosition(osgEarth::GeoPoint pos);
    void select(bool val);
    void hover(bool val);
protected:
    bool mIsSelected{false};
    bool mIs3D{false};
    osg::ref_ptr<osg::LOD> mRootNode;
    osg::ref_ptr<osg::Group> mNode3D;
    osg::ref_ptr<osg::Switch> mNode2D;
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> mLableNode;
    osgEarth::Color mModelColor{osgEarth::Color::White};
private:
    friend class ModelAnimationPathCallback;
    friend class PickHandler;
private:
    //bool mHasHit{false};
    QString mType;
    osg::ref_ptr<osgFX::Outline> mSelectOutline;
    MapController *mMapControler;
    //DefenseModelNode* mFollowModel{nullptr};
};

#endif // BASEMODEL_H
