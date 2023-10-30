#ifndef FLYABLEMODEL_H
#define FLYABLEMODEL_H
#include "moveableModelNode.h"

class FlyableModelNode: public MoveableModelNode
{
public:
    FlyableModelNode(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QQmlEngine *engine, BookmarkManager *bookmark, QObject *parent = nullptr);
    FlyableModelNode* asFlyableModelNode()override {return this;}
    void flyTo(osgEarth::GeoPoint destinationPoint, double velocity);
    double speed() const override;
    void setSpeed(double newSpeed) override;
private:
    osg::ref_ptr<osg::AnimationPathCallback> mMoveAnimationPathCallback;
};
class FmodelAnimationPathCallback: public osg::AnimationPathCallback
{
private:
    FlyableModelNode* mFlyableModel;
    MapItem* mMapItem;
    osgEarth::GeoPoint mPreGeoPoint;
public:
    FmodelAnimationPathCallback(MapItem* mapItem, FlyableModelNode* flyableModel);
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
};


#endif // FLYABLEMODEL_H
