#ifndef MOVEABLEMODEL_H
#define MOVEABLEMODEL_H

#include "simpleModelNode.h"
#include <osgAnimation/Keyframe>


class MoveableModelNode : public SimpleModelNode
{
public:
    explicit MoveableModelNode(MapItem* mapControler, const std::string& modelUrl, const std::string &iconUrl, QQmlEngine *engine, BookmarkManager *bookmark,int bulletCount=0, QObject *parent = nullptr);
    void moveTo(osgEarth::GeoPoint destinationPoint, double mSpeed);
    MoveableModelNode* asMoveableModelNode()override {return this;}

    virtual double speed() const;
    virtual void setSpeed(double newSpeed);
    virtual void stop(){}

private:
    double mSpeed;
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
