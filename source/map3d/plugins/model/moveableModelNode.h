#ifndef MOVEABLEMODEL_H
#define MOVEABLEMODEL_H

#include "simpleModelNode.h"
#include <osgAnimation/Keyframe>


class MoveableModelNode : public SimpleModelNode
{
public:
    explicit MoveableModelNode(QQmlEngine *engine,
                               MapItem *mapControler,
                               const std::string &modelUrl,
                               const std::string &iconUrl,
                               QObject *parent = nullptr);
    void moveTo(osgEarth::GeoPoint destinationPoint, double speed);
    MoveableModelNode* asMoveableModelNode()override {return this;}

    virtual double speed() const;
    virtual void setSpeed(double newSpeed);
    virtual void stop(){}

private:
    double mSpeed{20.0};
    osg::ref_ptr<osg::AnimationPathCallback> mMoveAnimationPathCallback;
};

class ModelAnimationPathCallback: public osg::AnimationPathCallback
{
private:
    MoveableModelNode* mMoveableModel;
    MapItem* mMapItem;
    osgEarth::GeoPoint mPreGeoPoint;
public:
    ModelAnimationPathCallback(MapItem* mapItem, MoveableModelNode* moveableModel);
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
};

#endif // MOVEABLEMODEL_H
