#include "flyingmodel.h"
#include <QDebug>

class MapAnimationPathCallback: public osg::AnimationPathCallback
{
public:
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override{
        if (_animationPath.valid() &&
                nv->getVisitorType()== osg::NodeVisitor::UPDATE_VISITOR &&
                nv->getFrameStamp())
            {
                double time = nv->getFrameStamp()->getSimulationTime();
                _latestTime = time;

                if (!_pause)
                {
                    // Only update _firstTime the first time, when its value is still DBL_MAX
                    if (_firstTime == DBL_MAX) _firstTime = time;
                    //--------------------------------
                    //update(*node);
                    //my code
//                    osgEarth::GeoTransform* geoNode = dynamic_cast<osgEarth::GeoTransform*>(node);
                    FlyingModel* flyNode = dynamic_cast<FlyingModel*>(node);
                    osg::AnimationPath::ControlPoint cp;
                    if (getAnimationPath()->getInterpolatedControlPoint(getAnimationTime(),cp))
                    {
                        osgEarth::GeoPoint  latLongPoint(osgEarth::SpatialReference::get("wgs84"), cp.getPosition());
                        osgEarth::GeoPoint geoPoint;
                        latLongPoint.transform(flyNode->getMapNode()->getMapSRS(), geoPoint);

                        flyNode->setPosition(geoPoint);
//                        flyNode->setScale(cp.getScale());
                        flyNode->getPositionAttitudeTransform()->setScale(cp.getScale());
                        flyNode->getPositionAttitudeTransform()->setAttitude(cp.getRotation());
                    }

                    //qDebug()<<"p:"<<cp.getPosition().x()<<","<<cp.getPosition().y()<<","<<cp.getPosition().z();
                }
            }

            // must call any nested node callbacks and continue subgraph traversal.
            NodeCallback::traverse(node,nv);
    }
};

FlyingModel::FlyingModel(osgEarth::MapNode* mapNode, const QString &fileName)
                :osgEarth::Annotation::ModelNode(mapNode, osgEarth::Symbology::Style())
{
    osg::ref_ptr<osg::Node>  node = osgDB::readRefNodeFile(fileName.toStdString());

    if (!node)
    {
        //todo show massage here
        return;
    }
    osgEarth::Symbology::Style  style;
    //        style.getOrCreate<osgEarth::Symbology::RenderSymbol>()->depthOffset()->enabled() = true;
    //style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    //style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);

    setStyle(style);
}

void FlyingModel::setLatLongPosition(const osg::Vec3d &pos)
{
    osgEarth::GeoPoint  pointLatLong(osgEarth::SpatialReference::get("wgs84"), pos.x(), pos.y(), pos.z());
    osgEarth::GeoPoint  mapPoint;
    pointLatLong.transform(getMapNode()->getMapSRS(), mapPoint);
//    osg::Vec3d worldPoint;
//    mapPoint.toWorld(worldPoint);
    setPosition(mapPoint);
}

void FlyingModel::flyTo(const osg::Vec3d &pos, double speed)
{
    auto currentGeoPoint = getPosition();
    //to lat long
    currentGeoPoint.makeGeographic();

    osg::Quat rotate;
    osg::Vec3f def = pos - currentGeoPoint.vec3d();
    rotate.makeRotate(-osg::Y_AXIS, def);
    osg::Vec3d estimatePos = pos + (def * def.normalize()) * 30;
//    double t = static_cast<double>((estimatePos - pos).length() / speed);
    osgEarth::GeoPoint geoPos(currentGeoPoint.getSRS(), pos);
    double distance = geoPos.distanceTo(osgEarth::GeoPoint(currentGeoPoint.getSRS(),estimatePos));
    double t =distance / speed;

    osg::AnimationPath* path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);

    path->insert(0, osg::AnimationPath::ControlPoint(currentGeoPoint.vec3d(),getPositionAttitudeTransform()->getAttitude(),getScale()));
    path->insert(2,osg::AnimationPath::ControlPoint(pos,rotate, getScale()));
    path->insert(t,osg::AnimationPath::ControlPoint(estimatePos,rotate, getScale()));

    mAnimationPathCallback = new MapAnimationPathCallback();
    mAnimationPathCallback->setAnimationPath(path);
    //animationPathCallback->setPivotPoint(osg::Vec3d(0,100,0));
    setUpdateCallback(mAnimationPathCallback);

    //draw line------------------------------------------------
    osg::Vec3Array* keyPoint = new osg::Vec3Array;
    keyPoint->push_back(currentGeoPoint.vec3d());
    keyPoint->push_back(estimatePos);
    keyPoint->push_back(pos);
}

void FlyingModel::setPause(bool pause)
{
    mAnimationPathCallback->setPause(pause);
}

bool FlyingModel::getPause() const
{
    return mAnimationPathCallback->getPause();
}
