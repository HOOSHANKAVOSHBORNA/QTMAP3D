#include "defenseModelNode.h"
#include "draw.h"

#include <QDebug>

#include <osgEarthUtil/EarthManipulator>

#include <osg/Material>

const osg::Node::NodeMask NODE_MASK = 0x00000001;

void ModelAnimationPathCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    DefenseModelNode* defenseModelNode;
    bool hit = false;
    bool positionCanged = false;
    osgEarth::GeoPoint geoPoint;

    if (_animationPath.valid() &&
            nv->getVisitorType()== osg::NodeVisitor::UPDATE_VISITOR &&
            nv->getFrameStamp())
    {
        double time = nv->getFrameStamp()->getSimulationTime();
        _latestTime = time;

        defenseModelNode = dynamic_cast<DefenseModelNode*>(node);
        if (defenseModelNode && !defenseModelNode->hasHit())
            //check collision----------------------------------------------------------------------------
            if(defenseModelNode->getFollowModel() != nullptr)
            {
                osg::Vec3d wPosition;
                defenseModelNode->getPosition().toWorld(wPosition);
                osg::Vec3d wFolowPosition;
                defenseModelNode->getFollowModel()->getPosition().toWorld(wFolowPosition);
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
            double animatTime = getAnimationTime();
            if (getAnimationPath()->getInterpolatedControlPoint(animatTime,cp))
            {
                geoPoint.fromWorld(defenseModelNode->getMapNode()->getMapSRS(), cp.getPosition());
                defenseModelNode->setPosition(geoPoint);
                defenseModelNode->getPositionAttitudeTransform()->setScale(cp.getScale());
                if(defenseModelNode->mIs3D)
                    defenseModelNode->getPositionAttitudeTransform()->setAttitude(cp.getRotation());
                else
                {
                    double angle;
                    osg::Vec3 vec;
                    cp.getRotation().getRotate(angle, vec);
                    vec.x() = 0;
                    vec.y() = 0;
                    defenseModelNode->getPositionAttitudeTransform()->setAttitude(osg::Quat(angle, vec));
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
                //if(static_cast<int>(animatTime) % 3 == 0)
                defenseModelNode->curentPosition(geoPoint);
            }

            if(defenseModelNode && (_latestTime - _firstTime) > _animationPath->getPeriod())
                defenseModelNode->stop();
        }
    }

    // must call any nested node callbacks and continue subgraph traversal.
    NodeCallback::traverse(node,nv);
    if(hit)
        defenseModelNode->collision(defenseModelNode->getFollowModel());
    //if(positionCanged)
    //        baseModel->curentPosition(geoPoint);
}


//bool PickHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//{

//    osgViewer::Viewer *view = dynamic_cast<osgViewer::Viewer *>(&aa);

//    switch (ea.getEventType())
//    {
//    case osgGA::GUIEventAdapter::FRAME:
//            findSceneModels(view);
//        break;
//    case (osgGA::GUIEventAdapter::PUSH):
//        if (view)
//        {
//            pick(view, ea);
//            if(mCurrentModel)
//            {
//                mCurrentModel->mousePushEvent(true, ea);

//            }
//            if(mLastPushModel && mLastPushModel != mCurrentModel)
//                mLastPushModel->mousePushEvent(false, ea);
//        }
//        if(mCurrentModel)
//        {
//            mLastPushModel = mCurrentModel;
//            return true;
//        }
//        break;
//    case (osgGA::GUIEventAdapter::MOVE):
//        if (view)
//        {
//            pick(view, ea);
//            if(mCurrentModel) {mCurrentModel->mouseMoveEvent(true, ea);}
//            if(mLastMoveModel && mLastMoveModel != mCurrentModel)
//                mLastMoveModel->mouseMoveEvent(false, ea);
//        }
//        if(mCurrentModel)
//            mLastMoveModel = mCurrentModel;
//        break;
//    case (osgGA::GUIEventAdapter::RELEASE):
//        break;
//    case (osgGA::GUIEventAdapter::SCROLL):
//        break;
//    case (osgGA::GUIEventAdapter::DOUBLECLICK):
//        break;
//    default:
//        break;
//    }
//    return false;
//}
//void PickHandler::pick(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea)
//{
//    osg::Group* root = dynamic_cast<osg::Group*>(viewer->getSceneData());
//    if (!root) return;

//    osgUtil::LineSegmentIntersector::Intersections intersections;
//    mCurrentModel = nullptr;
//    if (viewer->computeIntersections(ea,intersections))
//    {
//        for(osgUtil::LineSegmentIntersector::Intersection hit : intersections)
//        {
//            const osg::NodePath& nodePath = hit.nodePath;
//            for(osg::NodePath::const_iterator nitr=nodePath.begin();
//                nitr!=nodePath.end();
//                ++nitr)
//            {
//                //auto pl = dynamic_cast<osgEarth::Annotation::PlaceNode*>(*nitr);
////                if (pl)
////                    qDebug()<<pl;
//                mCurrentModel = dynamic_cast<BaseModel*>(*nitr);
//                if (mCurrentModel)
//                    break;
//            }
//            if(mCurrentModel)
//                break;
//        }
//    }
//}

//void PickHandler::findSceneModels(osgViewer::Viewer *viewer)
//{
//    osgEarth::Util::EarthManipulator*camera = dynamic_cast<osgEarth::Util::EarthManipulator*>(viewer->getCameraManipulator());
//    if(!camera)
//        return;
//    int range = static_cast<int>(camera->getViewpoint().getRange());
//    if(range != mPreRange && range < 12000)
//    {
//        mPreRange = range;
//        osg::Viewport* viewport = viewer->getCamera()->getViewport();
//        osg::ref_ptr<osgUtil::PolytopeIntersector> intersector{nullptr};
//        intersector = new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, viewport->x(), viewport->y(),
//                                                       viewport->x() + viewport->width(), viewport->y() + viewport->height());

//        intersector->setPrimitiveMask(osgUtil::PolytopeIntersector::ALL_PRIMITIVES);
//        intersector->setIntersectionLimit( osgUtil::Intersector::LIMIT_ONE_PER_DRAWABLE );

//        osgUtil::IntersectionVisitor iv(intersector);
////        iv.setTraversalMask(NODE_MASK);
////        iv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
////        iv.setTraversalNumber(1000);
//        viewer->getCamera()->accept(iv);

//        if(intersector->containsIntersections())
//        {
//            auto intersections = intersector->getIntersections();
//            //qDebug() <<"intersections: "<<intersections.size();
//            for(auto hit : intersections)
//            {

//                const osg::NodePath& nodePath = hit.nodePath;
//                //qDebug() <<"nodePath: "<<nodePath.size();
//                for(osg::NodePath::const_iterator nitr=nodePath.begin();
//                    nitr!=nodePath.end();
//                    ++nitr)
//                {
//                    BaseModel* model = dynamic_cast<BaseModel*>(*nitr);
//                    if (model && model->mCameraRangeChangeable)
//                    {
//                        //qDebug() <<model->getQStringName();
//                        //qDebug() <<"range: "<<camera->getViewpoint().getRange();
//                        //qDebug() <<"z: "<<model->getPosition().z();
//                        double distance = 0;
//                        if(camera->getViewpoint().getRange() < model->getPosition().z())///for track node
//                            distance = camera->getViewpoint().getRange();
//                        else
//                            distance = camera->getViewpoint().getRange() - model->getPosition().z();
//                        model->cameraRangeChanged(distance);
//                        //qDebug() <<"camera->getViewpoint().getRange(): "<<camera->getViewpoint().getRange();
//                        //qDebug() <<"model.getRange(): "<<camera->getViewpoint().getRange() - model->getPosition().z();
//                    }
//                }
//            }

//        }
//    }
//}

//static bool mAddedEvent = false;
DefenseModelNode::DefenseModelNode(osgEarth::MapNode *mapNode, QObject *parent):
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
    //    if(!mAddedEvent)
    //    {
    //        addEventCallback(new PickHandler());
    //        mAddedEvent = true;
    //    }
    //    setNodeMask(NODE_MASK);
}

QString DefenseModelNode::getType() const
{
    return mType;
}

void DefenseModelNode::setType(const QString &value)
{
    mType = value;
}

void DefenseModelNode::setQStringName(QString name)
{
    setName(name.toStdString());
    if(mLableNode)
        mLableNode->setText(name.toStdString());
}

QString DefenseModelNode::getQStringName()
{
    return QString(getName().c_str());
}

void DefenseModelNode::setGeographicPosition(const osgEarth::GeoPoint &pos, double heading)
{
    osgEarth::GeoPoint  geoPoint;
    pos.transform(getMapNode()->getMapSRS(), geoPoint);
    //transfer to map srs
    //geoPoint.transformInPlace(getMapNode()->getMapSRS());
    setPosition(geoPoint);

    osgEarth::Symbology::Style pm = getStyle();
    pm.getOrCreate<osgEarth::Symbology::ModelSymbol>()->heading() = -heading;
    setStyle(pm);

    //draw line------------------------------------------------
    //        osg::Vec3d worldpos;
    //        geoPoint.toWorld(worldpos);
    //        getMapNode()->addChild(drawCordination(worldpos + osg::Vec3d(0,2,0)));
}

osg::Vec3d DefenseModelNode::getGeographicPosition() const
{
    osgEarth::GeoPoint position = getPosition();
    position.makeGeographic();
    return position.vec3d();
}

void DefenseModelNode::playExplosionEffect(float scale)
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

    explosion1->setTextureFileName("../data/images/fire_p1.png");
    explosion2->setTextureFileName("../data/images/fire_p2.png");
    explosion3->setTextureFileName("../data/images/fire_p3.png");
    explosion4->setTextureFileName("../data/images/fire_p4.png");

    debris1->setTextureFileName("../data/images/debris_p1.png");

    smoke->setTextureFileName("../data/images/smoke_p1.png");

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

void DefenseModelNode::collision(DefenseModelNode *collidedWith)
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

DefenseModelNode *DefenseModelNode::getFollowModel() const
{
    return mFollowModel;
}

void DefenseModelNode::setFollowModel(DefenseModelNode *followModel)
{
    mFollowModel = followModel;
}

//void BaseModel::traverse(osg::NodeVisitor &nv)
//{
//    qDebug()<<"traverse:"<<getQStringName();
//    if (nv.getVisitorType() == osg::NodeVisitor::EVENT_VISITOR)
//    {
//        osgGA::EventVisitor* ev = static_cast<osgGA::EventVisitor*>(&nv);
//        for(osgGA::EventQueue::Events::iterator itr = ev->getEvents().begin();
//            itr != ev->getEvents().end();
//            ++itr)
//        {
//            osgGA::GUIEventAdapter* ea = dynamic_cast<osgGA::GUIEventAdapter*>(itr->get());
//            if ( ea && handle(*ea, *(ev->getActionAdapter())))
//                ea->setHandled(true);
//        }
//    }
//    GeoPositionNode::traverse( nv );
//}
//bool BaseModel::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
//{
//    if (ea.getHandled()) return false;
//}


bool DefenseModelNode::hasHit() const
{
    return mHasHit;
}

void DefenseModelNode::mousePressEvent(QMouseEvent* event, bool onModel)
{
    if(event->button() != Qt::MiddleButton)
        select(onModel);
}

void DefenseModelNode::mouseMoveEvent(QMouseEvent* /*event*/, bool onModel)
{
    if(!mIsSelected)
    {
        hover(onModel);
    }
}

//void BaseModel::cameraRangeChanged(double range)
//{

////    osgEarth::Symbology::Style  style = getStyle();
////    if(!mIs3d && range < 300)
////    {
//////        qDebug()<<getQStringName();
////        setCullCallback(nullptr);
////        style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = false;
////        setStyle(style);
////        getPositionAttitudeTransform()->setScale(osg::Vec3d(1,1,1));
////        mIs3d = true;
////        select(mIsSelected);
////    }
////    if(mIs3d && range > 300)
////    {
//////        qDebug()<<getQStringName();
////        setCullCallback(nullptr);
////        style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
////        setStyle(style);
////        getPositionAttitudeTransform()->setScale(osg::Vec3d(1,1,1));
////        mIs3d = false;
////        select(mIsSelected);
////    }
//}

void DefenseModelNode::curentPosition(osgEarth::GeoPoint pos)
{
    emit positionChanged(pos);
}

void DefenseModelNode::select(bool val)
{
    hover(val);
    mIsSelected = val;
    mLableNode->setNodeMask(val);
}

void DefenseModelNode::hover(bool val)
{
    mLableNode->setNodeMask(val);
    mNode2D->setValue(0, val);
    mNode2D->setValue(1, !val);

    //    auto lbStyle = mLableNode->getStyle();

    osg::ref_ptr<osg::Material> mat = new osg::Material;
    if(!val)
    {
        mat->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 0.0, 0.0, 0.5));
        //        lbStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::Red;
    }
    else
    {
        mat->setDiffuse (osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 0.2f, 0.5));
        //        lbStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::Yellow;
    }
    getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    //    mLableNode->setStyle(lbStyle);
}