#include "objecttracker.h"
#include "map3dwidget.h"

#include <QAction>
#include <QToolBar>
#include <QDebug>

#include <osgDB/ReadFile>
#include <osgEarthSymbology/GeometryFactory>
#include <osgEarthAnnotation/ModelNode>
#include <osg/PositionAttitudeTransform>
#include <osgEarth/GeoData>
#include <osg/PositionAttitudeTransform>
#include <osg/AnimationPath>
#include <osg/LineWidth>
#include <osg/Array>
#include <osgAnimation/EaseMotion>

ObjectTracker::ObjectTracker(QWidget *parent)
    : PluginInterface(parent)
{
}

void ObjectTracker::setUpUI()
{
    //    const QIcon readIcon (":/res/read.png");
    auto trackAction = new QAction("Traking");
    trackAction->setToolTip("Tracking a motion object.");
    mToolBar->addAction(trackAction);
    QObject::connect(trackAction, &QAction::triggered, this, &ObjectTracker::trackObject);

    auto tAction = new QAction("tre");
    mToolBar->addAction(tAction);
    QObject::connect(tAction, &QAction::triggered, [=]{
        de = de + 1;
        osg::AnimationPath* path = new osg::AnimationPath();
        path->setLoopMode(osg::AnimationPath::NO_LOOPING);
        /** point 1 - start point **/
        double ch = de/1000.0;
        osgEarth::GeoPoint  point;
        if(de % 2 == 0)
            point.set(osgEarth::SpatialReference::get("wgs84"), 52.859 + ch, 35.241, 1100,osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
        else
            point.set(osgEarth::SpatialReference::get("wgs84"), 52.859 , 35.241+ ch, 1100,osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
        osg::Vec3d out_world;
        point.toWorld(out_world);
        setPosition(out_world, 20);
    });
}

void ObjectTracker::trackObject()
{
    addModel();

}

void ObjectTracker::setPosition(const osg::Vec3d &pos, float speed)
{
    osg::Vec3f currentPos = modelNode->getPosition();
    osg::Quat rotate;
    osg::Vec3f def = pos - currentPos;
    double t = static_cast<double>(def.length() / speed);
    rotate.makeRotate(-osg::Y_AXIS, def);

    osg::AnimationPath* path = new osg::AnimationPath();
//    osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(path);
    MyAnimationPathCallback* animationPathCallback = new MyAnimationPathCallback(this);
    //animationPathCallback->setPivotPoint(osg::Vec3d(0,100,0));
    animationPathCallback->setAnimationPath(path);

    path->setLoopMode(osg::AnimationPath::NO_LOOPING);
    //path->insert(0, osg::AnimationPath::ControlPoint(currentPos));
    path->insert(0, osg::AnimationPath::ControlPoint(currentPos,modelNode->getAttitude(),modelNode->getScale()));
    path->insert(t,osg::AnimationPath::ControlPoint(pos,rotate,modelNode->getScale()));

    modelNode->setUpdateCallback(animationPathCallback);

//    animationPathCallback->getAnimationPath()->clear();
//    animationPathCallback->setAnimationPath(path);
//    animationPathCallback->reset();

    //draw line------------------------------------------------
    osg::Vec3Array* keyPoint = new osg::Vec3Array;
    keyPoint->push_back(currentPos);
    keyPoint->push_back(pos);
    mMap3dWidget->mMapRoot->addChild(createLine(keyPoint, 1.0));
    drawCordination(pos);

//    auto curentViewPoint = mMap3dWidget->getViewpoint();
//    osgEarth::GeoPoint point;
//    point.fromWorld(osgEarth::SpatialReference::get("wgs84"),pos);
//    curentViewPoint.focalPoint() = point;
//    curentViewPoint.range()= mMap3dWidget->getViewpoint().range();
//    mMap3dWidget->setViewpoint(curentViewPoint, 0);
}

void ObjectTracker::addModel()
{
    osg::ref_ptr<osg::Node>  node = osgDB::readRefNodeFile("../map3dlib/data/models/air.osgb");

    if (!node)
    {
        return;
    }
    //    osgEarth::Symbology::Style  style;
    //    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);
    ////    osg::ref_ptr<osgEarth::Annotation::ModelNode>  model;
    //    model = new osgEarth::Annotation::ModelNode(mMap3dWidget->getMapNode(), style);
    //    //auto srs = mMap3dWidget->getMapNode()->getMap()->getWorldSRS();
    //    //osgEarth::GeoPoint pos(srs,52.859, 35.241);
    //    //osgEarth::GeoPoint  point(osgEarth::SpatialReference::get("wgs84"), 52.859, 35.241, 800);
    //    osgEarth::GeoPoint  point(osgEarth::SpatialReference::get("wgs84"), 52.859, 35.241, 1100);
    //    model->setPosition(point);
    //    model->setScale(osg::Vec3(1,1,1));
    //    mMap3dWidget->getMapNode()->addChild(model);
    //Add to map -------------------------------------------------------------------
    modelNode = new osg::PositionAttitudeTransform;
    modelNode->addChild(node);
    osgEarth::GeoPoint  point(osgEarth::SpatialReference::get("wgs84"), 52.859, 35.241, 1100);
    mMap3dWidget->mMapRoot->addChild(modelNode);
    //    mMap3dWidget->getMapNode()->addChild(modelNode);
    osg::Vec3d worldPoint;
    point.toWorld(worldPoint);
    modelNode->setPosition(worldPoint);
    modelNode->setScale(osg::Vec3(0.5,0.5,0.5));
    drawCordination(worldPoint);
    //Set view point------------------------------------------------------------------
    osgEarth::Viewpoint vp;
    vp.focalPoint() = point;
    vp.range()= 500;
    vp.heading()->set(50, osgEarth::Units::DEGREES);
    //vp.pitch()->set(-25, osgEarth::Units::DEGREES);
    mMap3dWidget->setViewpoint(vp, 5);
}
osg::Node* ObjectTracker::createLine(osg::Vec3Array* vertex, float lineWidth)
{

    osg::Geode* gnode = new osg::Geode;
    osg::Geometry* geom = new osg::Geometry;
    gnode->addDrawable(geom);

    // Set vertex
    geom->setVertexArray(vertex);

    // Set the color
    osg::Vec4Array* color = new osg::Vec4Array();
    color->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
    geom->setColorArray(color, osg::Array::BIND_OVERALL);

    // Set the line width
    osg::LineWidth* lw = new osg::LineWidth(lineWidth);
    geom->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
    // Turn off the light to make it brighter
    geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

    // Set add as line
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vertex->size()));

    return gnode;
}

void ObjectTracker::drawCordination(const osg::Vec3d &pos)
{
    osg::Geode* gnode = new osg::Geode;
    osg::Geometry* geomX = new osg::Geometry;
    osg::Geometry* geomY = new osg::Geometry;
    osg::Geometry* geomZ = new osg::Geometry;
    gnode->addDrawable(geomX);
    gnode->addDrawable(geomY);
    gnode->addDrawable(geomZ);

    // Set vertex
    osg::Vec3Array* keyPoint = new osg::Vec3Array;
    keyPoint->push_back(pos + osg::Vec3d(10,0,0));
    keyPoint->push_back(pos);
    geomX->setVertexArray(keyPoint);
    // Set the color
    osg::Vec4Array* color = new osg::Vec4Array();
    color->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
    geomX->setColorArray(color, osg::Array::BIND_OVERALL);
    // Set the line width
    osg::LineWidth* lw = new osg::LineWidth(2);
    geomX->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
    // Turn off the light to make it brighter
    geomX->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
    // Set add as line
    geomX->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));

    // Set vertex
    osg::Vec3Array* keyPoint1 = new osg::Vec3Array;
    keyPoint1->push_back(pos+osg::Vec3d(0,10,0));
    keyPoint1->push_back(pos);
    geomY->setVertexArray(keyPoint1);
    // Set the color
    osg::Vec4Array* color1 = new osg::Vec4Array();
    color1->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
    geomY->setColorArray(color1, osg::Array::BIND_OVERALL);
    // Set the line width
    osg::LineWidth* lw1 = new osg::LineWidth(2);
    geomY->getOrCreateStateSet()->setAttribute(lw1, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
    // Turn off the light to make it brighter
    geomY->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
    // Set add as line
    geomY->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));

    // Set vertex
    osg::Vec3Array* keyPoint2 = new osg::Vec3Array;
    keyPoint2->push_back(pos+osg::Vec3d(0,0,10));
    keyPoint2->push_back(pos);
    geomZ->setVertexArray(keyPoint2);
    // Set the color
    osg::Vec4Array* color2 = new osg::Vec4Array();
    color2->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
    geomZ->setColorArray(color2, osg::Array::BIND_OVERALL);
    // Set the line width
    osg::LineWidth* lw2 = new osg::LineWidth(2);
    geomZ->getOrCreateStateSet()->setAttribute(lw2, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
    // Turn off the light to make it brighter
    geomZ->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
    // Set add as line
    geomZ->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));

    mMap3dWidget->mMapRoot->addChild(gnode);

}

//MyAnimationPath::MyAnimationPath(ObjectTracker *objectTarcker):
//    osg::AnimationPath()
//{
//    mObjectTracker = objectTarcker;
//}
//bool MyAnimationPath::getInterpolatedControlPoint(double time, osg::AnimationPath::ControlPoint &controlPoint) const
//{
//    if(static_cast<int>(time) % 1 == 0)
//    {
//        auto curentViewPoint = mObjectTracker->mMap3dWidget->getViewpoint();
//        auto pos = mObjectTracker->modelNode->getPosition();
//        osgEarth::GeoPoint point;
//        point.fromWorld(osgEarth::SpatialReference::get("wgs84"),pos);
//        curentViewPoint.focalPoint() = point;
//        curentViewPoint.range()= curentViewPoint.range();
//        mObjectTracker->mMap3dWidget->setViewpoint(curentViewPoint, 0);
//    }

//    //qDebug()<<time<<"  "<<controlPoint.getPosition().x()<<" " <<controlPoint.getPosition().y()<<" "<<controlPoint.getPosition().z();
//    return osg::AnimationPath::getInterpolatedControlPoint(time, controlPoint);
//}

MyAnimationPathCallback::MyAnimationPathCallback(ObjectTracker* objectTarcker):
    AnimationPathCallback()
{
//    _motion = new osgAnimation::InCircMotion();
    mObjectTracker = objectTarcker;
    mPreviousTime = 0;
//    mPreviousPos = new osg::Vec3d();
}

void MyAnimationPathCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
//    qDebug()<<"mv ";
    //if(!_motion.valid()) return;

//    osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(node);
    osg::PositionAttitudeTransform* mt = dynamic_cast<osg::PositionAttitudeTransform*>(node);

    if(!mt) return;
    double time = nv->getFrameStamp()->getSimulationTime();

//    qDebug()<<time<<"  "<<mt->getPosition().x()<<" " <<mt->getPosition().y()<<" "<<mt->getPosition().z();


    auto curentViewPoint = mObjectTracker->mMap3dWidget->getViewpoint();
    auto pos = mt->getPosition();
    if((pos.x() != mPreviousPos.x() || pos.y() != mPreviousPos.y() || pos.z() != mPreviousPos.z())
            && (time - mPreviousTime) > 0.003)
    {
        osgEarth::GeoPoint point;
        point.fromWorld(osgEarth::SpatialReference::get("wgs84"),pos);
        curentViewPoint.focalPoint() = point;
        curentViewPoint.range()= curentViewPoint.range();
        mObjectTracker->mMap3dWidget->setViewpoint(curentViewPoint, 0);

//        qDebug()<<time;
//        qDebug()<<"mPreviousPos:"<<"  "<<mPreviousPos.x()<<" " <<mPreviousPos.y()<<" "<<mPreviousPos.z();
//        qDebug()<<"pos:"<<"  "<<mt->getPosition().x()<<" " <<mt->getPosition().y()<<" "<<mt->getPosition().z();
//        mPreviousPos = pos;
        mPreviousTime = time;
    }
    mPreviousPos = pos;
//    mPreviousTime = time;
    AnimationPathCallback::operator()(node, nv);
}
