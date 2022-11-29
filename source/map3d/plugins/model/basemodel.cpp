#include "basemodel.h"
#include "draw.h"

#include <QDebug>

#include <osgEarthUtil/EarthManipulator>

#include <osg/Material>

void ModelAnimationPathCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    BaseModel* baseModel;
    bool hit = false;
    bool positionCanged = false;
    osgEarth::GeoPoint geoPoint;

    if (_animationPath.valid() &&
            nv->getVisitorType()== osg::NodeVisitor::UPDATE_VISITOR &&
            nv->getFrameStamp())
    {
        double time = nv->getFrameStamp()->getSimulationTime();
        _latestTime = time;

        baseModel = dynamic_cast<BaseModel*>(node);
        if (!baseModel->hasHit())
            //check collision----------------------------------------------------------------------------
            if(baseModel->getFollowModel() != nullptr)
            {
                osg::Vec3d wPosition;
                baseModel->getPosition().toWorld(wPosition);
                osg::Vec3d wFolowPosition;
                baseModel->getFollowModel()->getPosition().toWorld(wFolowPosition);
                double distance = (wPosition - wFolowPosition).length();
                if(distance < 3)
                    hit = true;
            }

        if (!_pause)
        {
            // Only update _firstTime the first time, when its value is still DBL_MAX
            if (_firstTime == DBL_MAX) _firstTime = time;
            //------------------------------------------------------------------------------------------------
            osg::AnimationPath::ControlPoint cp;
            if (getAnimationPath()->getInterpolatedControlPoint(getAnimationTime(),cp))
            {
                geoPoint.fromWorld(baseModel->getMapNode()->getMapSRS(), cp.getPosition());
                baseModel->setPosition(geoPoint);
                baseModel->getPositionAttitudeTransform()->setScale(cp.getScale());
                if(baseModel->mIs3d)
                    baseModel->getPositionAttitudeTransform()->setAttitude(cp.getRotation());
                else
                {
                    double angle;
                    osg::Vec3 vec;
                    cp.getRotation().getRotate(angle, vec);
                    vec.x() = 0;
                    vec.y() = 0;
                    baseModel->getPositionAttitudeTransform()->setAttitude(osg::Quat(angle, vec));
//                    qDebug()<<"angle:"<<osg::RadiansToDegrees(angle);
//                    qDebug()<<"vec:"<<vec.x()<<","<<vec.y()<<","<<vec.z();

                }

                //                double angel;
                //                osg::Vec3d vec;
                //                cp.getRotation().getRotate(angel, vec);
                //                osgEarth::Symbology::Style pm = baseModel->getPlaceNode()->getStyle();
                //                pm.getOrCreate<osgEarth::Symbology::IconSymbol>()->heading() = osg::RadiansToDegrees(angel);
                //                baseModel->getPlaceNode()->setStyle(pm);
                //emit current position----------------------------------------------------------------------
                positionCanged = true;
            }

            if((_latestTime - _firstTime) > _animationPath->getPeriod())
                baseModel->stop();
        }
    }

    // must call any nested node callbacks and continue subgraph traversal.
    NodeCallback::traverse(node,nv);
    if(hit)
        baseModel->collision(baseModel->getFollowModel());
    if(positionCanged)
        emit baseModel->positionChanged(geoPoint);
}


bool PickHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{

    osgViewer::Viewer *view = dynamic_cast<osgViewer::Viewer *>(&aa);

    osgEarth::Util::EarthManipulator* camera;
    switch (ea.getEventType())
    {
    case osgGA::GUIEventAdapter::FRAME:
        camera = dynamic_cast<osgEarth::Util::EarthManipulator*>(view->getCameraManipulator());
        if(camera)
        {
            findSceneModels(view);
        }
        break;
    case (osgGA::GUIEventAdapter::PUSH):
        if (view)
        {
            pick(view, ea);
            if(mCurrentModel) {mCurrentModel->mousePushEvent(true, ea);}
            if(mLastPushModel && mLastPushModel != mCurrentModel)
                mLastPushModel->mousePushEvent(false, ea);
        }
        if(mCurrentModel)
            mLastPushModel = mCurrentModel;
        break;
    case (osgGA::GUIEventAdapter::MOVE):
        if (view)
        {
            pick(view, ea);
            if(mCurrentModel) {mCurrentModel->mouseMoveEvent(true, ea);}
            if(mLastMoveModel && mLastMoveModel != mCurrentModel)
                mLastMoveModel->mouseMoveEvent(false, ea);
        }
        if(mCurrentModel)
            mLastMoveModel = mCurrentModel;
        break;
    case (osgGA::GUIEventAdapter::RELEASE):
        break;
    case (osgGA::GUIEventAdapter::SCROLL):
        break;
    case (osgGA::GUIEventAdapter::DOUBLECLICK):
        break;
    default:
        break;
    }
    return false;
}
void PickHandler::pick(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea)
{
    osg::Group* root = dynamic_cast<osg::Group*>(viewer->getSceneData());
    if (!root) return;

    osgUtil::LineSegmentIntersector::Intersections intersections;
    mCurrentModel = nullptr;
    if (viewer->computeIntersections(ea,intersections))
    {
        for(osgUtil::LineSegmentIntersector::Intersection hit : intersections)
        {
            const osg::NodePath& nodePath = hit.nodePath;
            for(osg::NodePath::const_iterator nitr=nodePath.begin();
                nitr!=nodePath.end();
                ++nitr)
            {
                auto pl = dynamic_cast<osgEarth::Annotation::PlaceNode*>(*nitr);
                if (pl)
                    qDebug()<<pl;
                mCurrentModel = dynamic_cast<BaseModel*>(*nitr);
                if (mCurrentModel)
                    break;
            }
            if(mCurrentModel)
                break;
        }
    }
}

void PickHandler::findSceneModels(osgViewer::Viewer *viewer)
{
//    const osg::NodePath& nodePath = viewer->getCoordinateSystemNodePath();
//    for(osg::NodePath::const_iterator nitr=nodePath.begin();
//        nitr!=nodePath.end();
//        ++nitr)
//    {
//        BaseModel* model = dynamic_cast<BaseModel*>(*nitr);
        if (mLastPushModel)
        {
            osgEarth::Util::EarthManipulator*camera = dynamic_cast<osgEarth::Util::EarthManipulator*>(viewer->getCameraManipulator());
            mLastPushModel->cameraRangeChanged(camera->getViewpoint().getRange());
        }
//    }
}

static bool mAddedEvent = false;
BaseModel::BaseModel(osgEarth::MapNode *mapNode, QObject *parent):
    QObject(parent),
    osgEarth::Annotation::ModelNode(mapNode, osgEarth::Symbology::Style())
{
    //--add place node-------------------------------------------------------------------------------------------
    //    osgEarth::Symbology::Style pm;
    //    pm.getOrCreate<osgEarth::Symbology::IconSymbol>()->url()->setLiteral("/home/client110/Downloads/setting.png");
    //    pm.getOrCreate<osgEarth::Symbology::IconSymbol>()->declutter() = true;
    //    pm.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo() = osgEarth::Color("#5f5f5f");

    //    mPlaceNode = new osgEarth::Annotation::PlaceNode();
    //getGeoTransform()->addChild(mPlaceNode);
    if(!mAddedEvent)
    {
        addEventCallback(new PickHandler());
        mAddedEvent = true;
    }
}

QString BaseModel::getType() const
{
    return mType;
}

void BaseModel::setType(const QString &value)
{
    mType = value;
}

void BaseModel::setQStringName(QString name)
{
    setName(name.toStdString());
}

QString BaseModel::getQStringName()
{
    return QString(getName().c_str());
}

void BaseModel::setGeographicPosition(const osg::Vec3d &pos, double heading)
{
    osgEarth::GeoPoint  geoPoint(getMapNode()->getMapSRS()->getGeographicSRS(), pos);
    //transfer to map srs
    geoPoint.transformInPlace(getMapNode()->getMapSRS());
    setPosition(geoPoint);

    osgEarth::Symbology::Style pm = getStyle();
    pm.getOrCreate<osgEarth::Symbology::ModelSymbol>()->heading() = heading;
    setStyle(pm);

    //draw line------------------------------------------------
    //        osg::Vec3d worldpos;
    //        geoPoint.toWorld(worldpos);
    //        getMapNode()->addChild(drawCordination(worldpos + osg::Vec3d(0,2,0)));
}

osg::Vec3d BaseModel::getGeographicPosition() const
{
    osgEarth::GeoPoint position = getPosition();
    position.makeGeographic();
    return position.vec3d();
}

void BaseModel::playExplosionEffect(float scale)
{
    osg::Vec3d worldPosition;
    getPosition().toWorld(worldPosition);

    osg::ref_ptr<osg::PositionAttitudeTransform> pSphereGroup = new osg::PositionAttitudeTransform;

    osg::ref_ptr<osgParticle::ExplosionEffect> explosion1 = new osgParticle::ExplosionEffect(worldPosition, 8.0f * scale, 0.128f);
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion2 = new osgParticle::ExplosionEffect(worldPosition, 4.0f * scale, 0.25f);
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion3 = new osgParticle::ExplosionEffect(worldPosition, 2.0f * scale, 0.5f);
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion4 = new osgParticle::ExplosionEffect(worldPosition, 1.0f * scale, 1.0f);

    explosion1->setParticleDuration(1.0);
    explosion2->setParticleDuration(1.2);
    explosion3->setParticleDuration(1.4);
    explosion4->setParticleDuration(1.6);

    osg::ref_ptr<osgParticle::ExplosionDebrisEffect> debris1 = new osgParticle::ExplosionDebrisEffect(worldPosition, 4.0f * scale, 0.03125f);

    osg::ref_ptr<osgParticle::SmokeEffect> smoke = new osgParticle::SmokeEffect(worldPosition, 10.0f * scale,2.0);
    smoke->setEmitterDuration(0.6);
    smoke->setParticleDuration(1.6);

    explosion1->setTextureFileName("../map3dlib/data/images/fire_p1.png");
    explosion2->setTextureFileName("../map3dlib/data/images/fire_p2.png");
    explosion3->setTextureFileName("../map3dlib/data/images/fire_p3.png");
    explosion4->setTextureFileName("../map3dlib/data/images/fire_p4.png");

    debris1->setTextureFileName("../map3dlib/data/images/debris_p1.png");

    smoke->setTextureFileName("../map3dlib/data/images/smoke_p1.png");

    //    osgEarth::Registry::shaderGenerator().run(explosion3);
    pSphereGroup->addChild(explosion3);
    //    osgEarth::Registry::shaderGenerator().run(explosion4);
    pSphereGroup->addChild(explosion4);

    //    osgEarth::Registry::shaderGenerator().run(debris1);
    pSphereGroup->addChild(debris1);

    //    osgEarth::Registry::shaderGenerator().run(smoke);
    pSphereGroup->addChild(smoke);
    pSphereGroup->setPosition(worldPosition);

    osgEarth::Registry::shaderGenerator().run(pSphereGroup);// for textures or lighting
    getMapNode()->addChild(pSphereGroup);
}

void BaseModel::collision(BaseModel *collidedWith)
{
    //    qDebug()<<QString(getQStringName());
    mHasHit = true;
    //    setPause(true);
    //    if(other != nullptr)
    //    {
    //        playExplosionEffect(1.0f);
    //        other->collision(nullptr);
    //    }
    playExplosionEffect(1.0f);
    setNodeMask(false);
    //    mSmoke->setNodeMask(false);
    //    mSmoke->getParticleSystem()->setNodeMask(false);
    //    mFire->setNodeMask(false);
    //    mFire->getParticleSystem()->setNodeMask(false);
    //    //    getMapNode()->removeChild(this);

    emit hit(collidedWith);
    stop();
}

BaseModel *BaseModel::getFollowModel() const
{
    return mFollowModel;
}

void BaseModel::setFollowModel(BaseModel *followModel)
{
    mFollowModel = followModel;
}


bool BaseModel::hasHit() const
{
    return mHasHit;
}

void BaseModel::mousePushEvent(bool onModel, const osgGA::GUIEventAdapter &ea)
{
    if(ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
    {
        select(onModel);
        mIsSelected = onModel;
    }
}

void BaseModel::mouseMoveEvent(bool onModel, const osgGA::GUIEventAdapter &ea)
{
    if(!mIsSelected)
    {
        select(onModel);
    }
}

void BaseModel::cameraRangeChanged(double range)
{

    osgEarth::Symbology::Style  style = getStyle();
    if(!mIs3d && range < 200)
    {
        setCullCallback(nullptr);
        style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = false;
        setStyle(style);
        getPositionAttitudeTransform()->setScale(osg::Vec3d(1,1,1));
        mIs3d = true;
        select(mIsSelected);
    }
    if(mIs3d && range > 200)
    {
        setCullCallback(nullptr);
        style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
        setStyle(style);
        getPositionAttitudeTransform()->setScale(osg::Vec3d(1,1,1));
        mIs3d = false;
        select(mIsSelected);
    }
}

void BaseModel::select(bool val)
{

    if(!mIs3d)
    {
        mRoot->setValue(0, false);
        mRoot->setValue(1, !val);
        mRoot->setValue(2, val);
    }
    else
    {
        mRoot->setValue(0, true);
        mRoot->setValue(1, false);
        mRoot->setValue(2, false);

        osg::ref_ptr<osg::Material> mat = new osg::Material;
        if(!val)
            mat->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 0.0, 0.0, 1.0));
        else
            mat->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 0.2f, 1.0));
        getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    }
}
