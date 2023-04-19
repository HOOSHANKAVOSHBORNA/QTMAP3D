#include "defenseModelNode.h"
#include "draw.h"
#include "systemModelNode.h"

#include <QDebug>

#include <osgEarthUtil/EarthManipulator>

#include <osg/Material>
#include <osgFX/Outline>

const osg::Node::NodeMask NODE_MASK = 0x00000001;

void ModelAnimationPathCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    DefenseModelNode* defenseModelNode;
    //bool hit = false;
    bool positionCanged = false;
    osgEarth::GeoPoint geoPoint;

    if (_animationPath.valid() &&
            nv->getVisitorType()== osg::NodeVisitor::UPDATE_VISITOR &&
            nv->getFrameStamp())
    {
        double time = nv->getFrameStamp()->getSimulationTime();
        _latestTime = time;

        defenseModelNode = dynamic_cast<DefenseModelNode*>(node);
//        if (defenseModelNode && !defenseModelNode->hasHit())
//            //check collision----------------------------------------------------------------------------
//            if(defenseModelNode->getFollowModel() != nullptr)
//            {
//                osg::Vec3d wPosition;
//                defenseModelNode->getPosition().toWorld(wPosition);
//                osg::Vec3d wFolowPosition;
//                defenseModelNode->getFollowModel()->getPosition().toWorld(wFolowPosition);
//                double distance = (wPosition - wFolowPosition).length();
//                if(distance < 3)
//                    hit = true;
//            }

        if (!_pause)
        {
            // Only update _firstTime the first time, when its value is still DBL_MAX
            if (_firstTime >= DBL_MAX) _firstTime = time;
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
//                defenseModelNode->curentPosition(geoPoint);
            }

            if(defenseModelNode && (_latestTime - _firstTime) > _animationPath->getPeriod())
                defenseModelNode->stop();
        }
    }

    // must call any nested node callbacks and continue subgraph traversal.
    NodeCallback::traverse(node,nv);
//    if(hit)
//        defenseModelNode->collision(defenseModelNode->getFollowModel());
    //if(positionCanged)
    //        baseModel->curentPosition(geoPoint);
}

DefenseModelNode::DefenseModelNode(MapController *mapControler, QObject *parent):
    QObject(parent),
    osgEarth::Annotation::ModelNode(mapControler->getMapNode(), DefenseModelLayer::getDefaultStyle())
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
    mMapControler = mapControler;
//    mSelectOutline =  new osgFX::Outline;
//    mSelectOutline->setWidth( 8 );
//    mSelectOutline->setColor( osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );
//    mSelectOutline->addChild(this);
//    osg::DisplaySettings::instance()->setMinimumNumStencilBits(1);
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
//    auto systemModel =  dynamic_cast<SystemModelNode*>(this);
//    if(mLabelNode && !systemModel)
//        mLabelNode->setText(name.toStdString());
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

void DefenseModelNode::collision()
{
    //    qDebug()<<QString(getQStringName());
//    mHasHit = true;
    //    setPause(true);
    //    if(other != nullptr)
    //    {
    //        playExplosionEffect(1.0f);
    //        other->collision(nullptr);
    //    }
    float scale =  static_cast<float>(getScale().x() * 20);
    playExplosionEffect(scale);
    setNodeMask(false);
    //    mSmoke->setNodeMask(false);
    //    mSmoke->getParticleSystem()->setNodeMask(false);
    //    mFire->setNodeMask(false);
    //    mFire->getParticleSystem()->setNodeMask(false);
    //    //    getMapNode()->removeChild(this);

//    emit hit(collidedWith);
    stop();
}
osgEarth::Annotation::ModelNode *DefenseModelNode::getDragModelNode()
{
    osgEarth::Symbology::Style  style = getStyle();
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 1;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale() = 1700;
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setDiffuse (osg::Material::FRONT_AND_BACK, osgEarth::Color::Gray);
    osg::ref_ptr<osgEarth::Annotation::ModelNode> dragModelNode = new osgEarth::Annotation::ModelNode(getMapNode(), style);
    dragModelNode->getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);

//    dragModelNode->addCullCallback(getCullCallback());
    return dragModelNode.release();
}

osg::ref_ptr<osg::Image> DefenseModelNode::createColoredImage(osg::ref_ptr<osg::Image> sourceImage, osgEarth::Color color)
{
    osg::ref_ptr<osg::Image> newImage = new osg::Image(*sourceImage, osg::CopyOp::DEEP_COPY_ALL);

    unsigned int width = static_cast<unsigned int>(sourceImage->s());
    unsigned int height = static_cast<unsigned int>(sourceImage->t());

    for(unsigned int i = 0; i < width; i++) {
        for(unsigned int j = 0; j < height; j++)
        {
            const osg::Vec4 pixColore = sourceImage->getColor(i, j);
            if(pixColore.a() > 0)
                newImage->setColor(color, i, j);
            else
                newImage->setColor(osg::Vec4(0,0,0,0), i, j);
        }
    }

    return newImage;
}

osg::ref_ptr<osg::Image> DefenseModelNode::createDarkerImage(osg::ref_ptr<osg::Image> sourceImage, float factor)
{
    osg::ref_ptr<osg::Image> newImage = new osg::Image(*sourceImage, osg::CopyOp::DEEP_COPY_ALL);

    unsigned int width = static_cast<unsigned int>(sourceImage->s());
    unsigned int height = static_cast<unsigned int>(sourceImage->t());

    for(unsigned int i = 0; i < width; i++) {
        for(unsigned int j = 0; j < height; j++)
        {
            const osg::Vec4 pixColore = sourceImage->getColor(i, j);
            if(pixColore.a() > 0) {
                const osg::Vec4 newColor(pixColore.r() * factor,
                                         pixColore.g() * factor,
                                         pixColore.b() * factor,
                                         pixColore.a());
                newImage->setColor(newColor, i, j);
            }
            else {
                newImage->setColor(osg::Vec4(0,0,0,0), i, j);
            }
        }
    }

    return newImage;

}

void DefenseModelNode::mousePressEvent(QMouseEvent* event, bool onModel)
{
    if(event->button() != Qt::MiddleButton)
        select(onModel);
}

void DefenseModelNode::mouseMoveEvent(QMouseEvent* /*event*/, bool onModel)
{
    //if(!mIsSelected)
    //{
        hover(onModel);
    //}
}
//void DefenseModelNode::curentPosition(osgEarth::GeoPoint pos)
//{
//    //emit positionChanged(pos);
//}

void DefenseModelNode::select(bool val)
{
//    const osg::BoundingSphere& bound = getBound();
//    qDebug()<<"radius: "<<bound.radius();
//    qDebug()<<"center:("<<bound.center().x()<<", "<<bound.center().y()<<", "<<bound.center().z()<<")";
//    mSelectCircle->setRadius(osgEarth::Distance(bound.radius(), osgEarth::Units::METERS));
//    mSelectSphere->setRadius(osgEarth::Distance(bound.radius(), osgEarth::Units::METERS));
//    if(val)
//    {
////        mSelectCircle->setPosition(getPosition());
////        mMapControler->addNode(mSelectCircle);
//        mSelectSphere->setPosition(getPosition());
//        mMapControler->addNode(mSelectSphere);
//    }
//    else
//        mMapControler->removeNode(mSelectSphere);


//    if(val)
//        mMapControler->addNode(mSelectOutline);
//    else
//        mMapControler->removeNode(mSelectOutline);
    //--------------------------------------------------------------------------------------------------
    mIsSelected = val;
    hover(val);
}

void DefenseModelNode::hover(bool val)
{
    //---------------------------------------------------
    if(mLabelNode)
        mLabelNode->setNodeMask(val);

    //    auto lbStyle = mLabelNode->getStyle();

    osg::ref_ptr<osg::Material> mat = new osg::Material;
    if(!val && !mIsSelected)
    {
        mat->setDiffuse (osg::Material::FRONT_AND_BACK, mModelColor);
    }
    else
    {
        osgEarth::Color color = mModelColor/2;
        color.a() = 1;
        mat->setDiffuse (osg::Material::FRONT_AND_BACK, color);
    }
    getOrCreateStateSet()->setAttributeAndModes(mat, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    //    mLabelNode->setStyle(lbStyle);
}
