#include "flyingmodel.h"
#include "draw.h"

#include <QDebug>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgEarth/Registry>
#include <osgGA/EventVisitor>
#include <osgGA/EventHandler>
#include <osgViewer/View>
#include <osgEarth/IntersectionPicker>
#include <QMouseEvent>

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
                if (!flyNode->isHit() && getAnimationPath()->getInterpolatedControlPoint(getAnimationTime(),cp))
                {
                    osgEarth::GeoPoint  latLongPoint(osgEarth::SpatialReference::get("wgs84"), cp.getPosition());
                    osgEarth::GeoPoint geoPoint;
                    latLongPoint.transform(flyNode->getMapNode()->getMapSRS(), geoPoint);

                    flyNode->setPosition(geoPoint);
                    //                        emit flyNode->positionChanged(latLongPoint);
                    //                        flyNode->setScale(cp.getScale());
                    flyNode->getPositionAttitudeTransform()->setScale(cp.getScale());
                    flyNode->getPositionAttitudeTransform()->setAttitude(cp.getRotation());
                    //check collision------------------------------------------------------
                    if(flyNode->getFollowingModel() != nullptr)
                    {
                        //qDebug()<<"!= nullptr"<< QString(flyNode->getName().c_str());
                        double distance = (flyNode->getPosition().vec3d() - flyNode->getFollowingModel()->getPosition().vec3d()).length();
                        if(distance < 3)
                        {
                            qDebug()<<QString(flyNode->getName().c_str());
                            flyNode->collision(flyNode->getFollowingModel());

                        }
                    }
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


    //    setScale(osg::Vec3(0.09f,0.09f,0.09f));
    qDebug()<<"center:"<<getBound().center().x()<<","<<getBound().center().y()<<","<<getBound().center().z();
    qDebug()<<"radius:"<<getBound().radius();

    osg::Vec3d center = getBound().center();
    float radius = getBound().radius();
    float scale = 3;
    //osgEarth::Registry::shaderGenerator().run(this);
    osgParticle::FireEffect *fire = new osgParticle::FireEffect(center + osg::Vec3f(0, radius,0),scale,50.0);
    getPositionAttitudeTransform()->addChild(fire);
    fire->setUseLocalParticleSystem(false);
    //mRootNode->addChild(fire->getParticleSystem());
    getMapNode()->getParent(0)->getParent(0)->addChild(fire->getParticleSystem());

    fire->setEmitterDuration(360000);
    fire->setParticleDuration(1);

    osgParticle::SmokeTrailEffect *smoke = new osgParticle::SmokeTrailEffect(center + osg::Vec3f(0, radius,0),scale/3,100.0);
    getPositionAttitudeTransform()->addChild(smoke);
    smoke->setUseLocalParticleSystem(false);
    //mRootNode->addChild(smoke->getParticleSystem());
    getMapNode()->getParent(0)->getParent(0)->addChild(smoke->getParticleSystem());

    smoke->setEmitterDuration(360000);
    smoke->setParticleDuration(5);
}

void FlyingModel::setLatLongPosition(const osg::Vec3d &pos)
{
    osgEarth::GeoPoint  pointLatLong(osgEarth::SpatialReference::get("wgs84"), pos.x(), pos.y(), pos.z());
    osgEarth::GeoPoint  mapPoint;
    pointLatLong.transform(getMapNode()->getMapSRS(), mapPoint);
    //    osg::Vec3d worldPoint;
    //    mapPoint.toWorld(worldPoint);
    setPosition(mapPoint);

    //draw line------------------------------------------------
    //    osg::Vec3d worldpos;
    //    mapPoint.toWorld(worldpos);
    //    getMapNode()->addChild(drawCordination(worldpos + osg::Vec3d(0,2,0)));

}

void FlyingModel::flyTo(const osg::Vec3d &pos, double speed)
{
    auto currentGeoPoint = getPosition();
    //to lat long
    currentGeoPoint.makeGeographic();

    osg::Quat rotate;
    osg::Vec3f def = pos - currentGeoPoint.vec3d();
    def.z() = 0;
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
    //    osg::Vec3Array* keyPoint = new osg::Vec3Array;
    //    keyPoint->push_back(currentGeoPoint.vec3d());
    //    keyPoint->push_back(estimatePos);
    //    keyPoint->push_back(pos);
    //    mMap3dWidget->mMapRoot->addChild(drawLine(keyPoint, 1.0));
    //    mMap3dWidget->mMapRoot->addChild(drawCordination(pos));
    //    mMap3dWidget->mMapRoot->addChild(drawCordination(estimatePos));

    emit positionChanged(osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"),pos));
}

void FlyingModel::setPause(bool pause)
{
    mAnimationPathCallback->setPause(pause);
}

bool FlyingModel::getPause() const
{
    return mAnimationPathCallback->getPause();
}

void FlyingModel::setFollowingModel(FlyingModel *followingModel)
{
    mFollowingModel = followingModel;
}

FlyingModel *FlyingModel::getFollowingModel() const
{
    return mFollowingModel;
}

void FlyingModel::collision(FlyingModel *other)
{
    qDebug()<<QString(getName().c_str());
    mIsHit = true;
    setPause(true);
    if(other != nullptr)
    {
        osg::Vec3d worldPosition;
        getPosition().toWorld(worldPosition);

        osg::PositionAttitudeTransform *pSphereGroup = new osg::PositionAttitudeTransform;

        osgParticle::ExplosionEffect *explosion = new osgParticle::ExplosionEffect(worldPosition, 2.0f, 10.0f);
        explosion->setParticleDuration(20);
        //        explosion->setEmitterDuration(10);
        osgParticle::ExplosionDebrisEffect *debris = new osgParticle::ExplosionDebrisEffect(worldPosition, 5.0f, 10.0f);
        //        debris->setEmitterDuration(10);

        osgParticle::SmokeTrailEffect *smoke = new osgParticle::SmokeTrailEffect(worldPosition,5,100.0);
        smoke->setParticleDuration(50);

        pSphereGroup->addChild(explosion);
        pSphereGroup->addChild(debris);
        pSphereGroup->addChild(smoke);
        pSphereGroup->setPosition(worldPosition);

        getMapNode()->getParent(0)->getParent(0)->addChild(pSphereGroup);

        other->collision(nullptr);
    }

    emit hit(other);
    setNodeMask(false);
    //    getMapNode()->removeChild(this);
}

bool FlyingModel::isHit() const
{
    return mIsHit;
}

