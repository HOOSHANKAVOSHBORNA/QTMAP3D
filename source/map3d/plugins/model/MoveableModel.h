#ifndef MOVEABLEMODEL_H
#define MOVEABLEMODEL_H

#include "simpleModelNode.h"
#include <osgAnimation/Keyframe>
#include "lineNode.h"


class MoveableModel : public simpleModelNode
{
public:
    explicit MoveableModel(MapItem* mapControler, const std::string& modelUrl, const std::string &iconUrl, QObject *parent = nullptr);
    void moveTo(osgEarth::GeoPoint destinationPoint, double mSpeed);
    virtual simpleModelNode* getNewModel() override;
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
    MoveableModel* mMoveableModel;
    MapItem* mMapItem;
    osgEarth::GeoPoint mPreGeoPoint;
public:
    ModelAnimationPathCallback(MapItem* mapItem, MoveableModel* moveableModel);
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
};

#endif // MOVEABLEMODEL_H
