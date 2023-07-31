#ifndef MOVEABLEMODEL_H
#define MOVEABLEMODEL_H

#include "simpleModelNode.h"
#include <osgAnimation/Keyframe>




//class ModelAnimationPathCallback: public osg::AnimationPathCallback
//{
//public:
//    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
//};

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

#endif // MOVEABLEMODEL_H
