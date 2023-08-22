#ifndef FLYABLEMODEL_H
#define FLYABLEMODEL_H
#include "MoveableModelNode.h"

class FlyableModel: public MoveableModel
{
public:
    FlyableModel(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent = nullptr);
    void flyTo(osgEarth::GeoPoint destinationPoint, double velocity);
    double speed() const override;
    void setSpeed(double newSpeed) override;
    virtual SimpleModelNode *getNewModel() override;
private:
    osg::ref_ptr<osg::AnimationPathCallback> mMoveAnimationPathCallback;
};
class FmodelAnimationPathCallback: public osg::AnimationPathCallback
{
private:
    FlyableModel* mFlyableModel;
    MapItem* mMapItem;
    osgEarth::GeoPoint mPreGeoPoint;
public:
    FmodelAnimationPathCallback(MapItem* mapItem, FlyableModel* flyableModel);
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
};


#endif // FLYABLEMODEL_H
