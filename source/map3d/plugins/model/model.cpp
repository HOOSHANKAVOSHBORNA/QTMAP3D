#include "model.h"
#include "map3dwidget.h"
#include "toolbarwidget.h"
#include "draw.h"
#include "vehicle.h"

#include <QDebug>
#include <QTimer>

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
#include <osgEarth/ElevationLayer>

Model::Model(QWidget *parent)
    : PluginInterface(parent)
{
}

void Model::setUpUI()
{
    ToolBarWidget::Category cat = ToolBarWidget::Category::Model;
    QString nameAddModel = "Add Airplan Model";
    mToolBar->addItem(cat, nameAddModel, "");

    QString nameTrack = "Tracking 3D Models";
    mToolBar->addItem(cat, nameTrack, "",true);

    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name){
        if(cat == category && name == nameAddModel)
        {
            addModel();
        }
        //        else if(cat == category && name == nameTrack)
        //        {
        //            auto vp = mMap3dWidget->getViewpoint();
        //            //vp.setNode(modelNode);//to track
        //            vp.setNode(model->getGeoTransform());//to track
        //            mMap3dWidget->setViewpoint(vp);
        //            auto camSet = mMap3dWidget->mEarthManipulator->getSettings();
        //            camSet->setTetherMode(osgEarth::Util::EarthManipulator::TetherMode::TETHER_CENTER);
        //            //    camSet->getBreakTetherActions().push_back(osgEarth::Util::EarthManipulator::ACTION_GOTO );
        //            mMap3dWidget->mEarthManipulator->applySettings(camSet);
        //            demo();
        //            QTimer *timer = new QTimer(this);
        //            connect(timer, &QTimer::timeout,this, &Model::demo);
        //            timer->start(10000);
        //        }
    });
    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name, bool isCheck){
        if(cat == category && name == nameTrack)
        {

            if(isCheck)
            {
                mMap3dWidget->setTrackNode(model->getGeoTransform());
                demo();
                QTimer *timer = new QTimer(this);
                connect(timer, &QTimer::timeout,this, &Model::demo);
                timer->start(10000);
            }
            else
            {
                mMap3dWidget->setTrackNode(nullptr);//to untrack
            }
        }
    });

}


void Model::demo()
{

    //    qDebug()<<random;
    //    osgEarth::GeoPoint  point(osgEarth::SpatialReference::get("wgs84"), 52.859 , 35.241, 1000,osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
    //    if(de % 2 == 0)
    //        point.set(osgEarth::SpatialReference::get("wgs84"), 52.859 + ch, 35.241, 1100,osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
    //    else
    //        point.set(osgEarth::SpatialReference::get("wgs84"), 52.859 , 35.241+ ch, 1100,osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
    //    osg::Vec3d out_world;
    //    point.toWorld(out_world);

    //-- 500 km/h ~ 139 m/s ------------------
    //    int randomX = 10*(138 + (qrand() % 139));
    //    int randomY = 10*(138 + (qrand() % 139));
    int randomX = 1*(138 + (qrand() % 139));
    int randomY = 1*(138 + (qrand() % 139));

    int val = qrand() % 4;
    if(val == 1)
        mCurrentWorldPoint += osg::Vec3d(randomX, randomY, 0);
    else if(val == 2)
        mCurrentWorldPoint += osg::Vec3d(randomX, -randomY, 0);
    else if(val == 3)
        mCurrentWorldPoint += osg::Vec3d(-randomX, randomY, 0);
    else
        mCurrentWorldPoint += osg::Vec3d(-randomX, -randomY, 0);

    setPosition(mCurrentWorldPoint, 138);
}
//osg::AnimationPath* ObjectTracker::createAnimationPath(const osg::Vec3d &pos1, const osg::Vec3d &pos2, float speed)
//{
//    osg::Vec3Array* keyPoint = new osg::Vec3Array;
//    osg::Vec3f def = pos2 - pos1;
//    float looptime = (def.length() / speed);
//    float radius = def.length()/2;
//    osg::Vec3d center = (pos1 + pos2) / 2;
//    // set up the animation path
//    osg::AnimationPath* animationPath = new osg::AnimationPath;
//    animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);

//    int numSamples = 40;
//    float yaw = 0.0f;
//    float yaw_delta = static_cast<float>(2.0 * osg::PI )/ (numSamples -1.0f);
//    float roll = osg::inDegrees(0.0f);

//    double time = 0.0;
//    float time_delta = looptime / numSamples;
//    for(int i=0;i<numSamples;++i)
//    {
//        osg::Vec3 position(center + osg::Vec3(sinf(yaw)*radius, cosf(yaw)*radius, 0.0f));
//        drawCordination(position);
//        keyPoint->push_back(position);
//        osg::Quat rotation(osg::Quat(roll,osg::Vec3(0.0,1.0,0.0))*osg::Quat((yaw+osg::inDegrees(90.0f)),osg::Vec3(0.0,0.0,1.0)));

//        animationPath->insert(time,osg::AnimationPath::ControlPoint(position,rotation));

//        yaw += yaw_delta;
//        time += static_cast<double>(time_delta);

//    }

//    mMap3dWidget->mMapRoot->addChild(createLine(keyPoint, 1.0));

//    return animationPath;
//}

void Model::setPosition(const osg::Vec3d &pos, double speed)
{
    //    osgEarth::GeoPoint  pointLatLong(osgEarth::SpatialReference::get("wgs84"), pos);
    //    osgEarth::GeoPoint  mapPos;
    //    pointLatLong.transform(mMap3dWidget->getMapSRS(), mapPos);
    //    osg::Vec3d &WorldPos
    //    mapPos.toWorld(currentPos,mMap3dWidget->getMapNode()->getTerrain());

    //    osg::Vec3d currentPos = modelNode->getPosition();
    osg::Vec3d currentPos;
    auto point = model->getGeoTransform()->getPosition();
    point.toWorld(currentPos);

    osg::Quat rotate;
    osg::Vec3f def = pos - currentPos;
    rotate.makeRotate(-osg::Y_AXIS, def);
    osg::Vec3d estimatePos = pos + (def * def.normalize()) * 30;
    double t = static_cast<double>((estimatePos - pos).length() / speed);

    osg::AnimationPath* path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);
    //    path->insert(0, osg::AnimationPath::ControlPoint(currentPos,modelNode->getAttitude(),modelNode->getScale()));
    //    path->insert(1,osg::AnimationPath::ControlPoint(pos,rotate,modelNode->getScale()));
    //    path->insert(t,osg::AnimationPath::ControlPoint(estimatePos,rotate,modelNode->getScale()));
    path->insert(0, osg::AnimationPath::ControlPoint(currentPos,model->getPositionAttitudeTransform()->getAttitude(), model->getScale()));
    path->insert(2,osg::AnimationPath::ControlPoint(pos,rotate, model->getScale()));
    path->insert(t,osg::AnimationPath::ControlPoint(estimatePos,rotate, model->getScale()));

    //auto path = createAnimationPath(currentPos, pos, speed);

    osg::AnimationPathCallback* animationPathCallback = new osg::AnimationPathCallback(path);
    animationPathCallback->setAnimationPath(path);
    //animationPathCallback->setPivotPoint(osg::Vec3d(0,100,0));
    //    modelNode->setUpdateCallback(animationPathCallback);
    model->getGeoTransform()->setUpdateCallback(animationPathCallback);

    //draw line------------------------------------------------
    osg::Vec3Array* keyPoint = new osg::Vec3Array;
    keyPoint->push_back(currentPos);
    keyPoint->push_back(estimatePos);
    keyPoint->push_back(pos);
    //    mMap3dWidget->mMapRoot->addChild(drawLine(keyPoint, 1.0));
    //    mMap3dWidget->mMapRoot->addChild(drawCordination(pos));
    //    mMap3dWidget->mMapRoot->addChild(drawCordination(estimatePos));

    //    auto curentViewPoint = mMap3dWidget->getViewpoint();
    //    osgEarth::GeoPoint point;
    //    point.fromWorld(osgEarth::SpatialReference::get("wgs84"),pos);
    //    curentViewPoint.focalPoint() = point;
    //    curentViewPoint.range()= mMap3dWidget->getViewpoint().range();
    //    mMap3dWidget->setViewpoint(curentViewPoint, 0);
}

void Model::addModel()
{
    osg::ref_ptr<osg::Node>  node = osgDB::readRefNodeFile("../map3dlib/data/models/air.osgb");

    if (!node)
    {
        return;
    }
    osgEarth::Symbology::Style  style;
    //        style.getOrCreate<osgEarth::Symbology::RenderSymbol>()->depthOffset()->enabled() = true;
    //style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    //style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);
    //        style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->url()->setLiteral("../map3dlib/data/models/dumptruck.osg");


    model = new osgEarth::Annotation::ModelNode(mMap3dWidget->getMapNode(), style);
    //auto srs = mMap3dWidget->getMapNode()->getMap()->getWorldSRS();
    //osgEarth::GeoPoint pos(srs,52.859, 35.241);
    //osgEarth::GeoPoint  point(osgEarth::SpatialReference::get("wgs84"), 52.859, 35.241, 800);
    osgEarth::GeoPoint  point(mMap3dWidget->getMapNode()->getMapSRS(), 52.8601, 35.277, 2100, osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
    point.toWorld(mCurrentWorldPoint, mMap3dWidget->getMapNode()->getTerrain());
    mMap3dWidget->getMapNode()->addChild(model);
    //mMap3dWidget->mMapRoot->addChild(model);
    model->setPosition(point);
    model->setScale(osg::Vec3(0.09f,0.09f,0.09f));
    //Add to map -------------------------------------------------------------------
    //    modelNode = new osg::PositionAttitudeTransform;
    //    modelNode->addChild(node);
    //    osgEarth::GeoPoint  point(mMap3dWidget->getMapNode()->getMapSRS(), 52.859, 35.241, 3300,osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
    //    mMap3dWidget->mMapRoot->addChild(modelNode);
    //    //    mMap3dWidget->getMapNode()->addChild(modelNode);
    //    point.toWorld(mCurrentWorldPoint, mMap3dWidget->getMapNode()->getTerrain());
    //    modelNode->setPosition(mCurrentWorldPoint);
    //    modelNode->setScale(osg::Vec3(0.5,0.5,0.5));
    //    mMap3dWidget->mMapRoot->addChild(drawCordination(mCurrentWorldPoint));
    //Set view point------------------------------------------------------------------
    //    osgEarth::Viewpoint vp;
    //    vp.focalPoint() = point;
    //    vp.range()= 500;
    //    vp.heading()->set(50, osgEarth::Units::DEGREES);
    //    //vp.pitch()->set(-25, osgEarth::Units::DEGREES);
    //    mMap3dWidget->setViewpoint(vp, 5);

    //----------------------------------------------------------------------------
    auto model_3d = new vehicle;
    //model_3d->movePack(osg::Vec3(5,5,0),10);
    //model_3d->spinHolder(osg::Vec3(-20,20,0));

    auto animCall = new AnimtkUpdateCallback;
    animCall->setVehicle(model_3d);
    animCall->moveTruck(osg::Vec3(10, 10, 0), 10);
    animCall->spinHolder(osg::Vec3(20, 20, 5));
    animCall->setupAnimtkNode();
    osgEarth::Symbology::Style  style1;
    style1.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(model_3d);

    auto modelTruck = new osgEarth::Annotation::ModelNode(mMap3dWidget->getMapNode(), style1);
    mMap3dWidget->getMapNode()->addChild(modelTruck);
    //mMap3dWidget->mMapRoot->addChild(model);
    osgEarth::GeoPoint  point1(mMap3dWidget->getMapNode()->getMapSRS(), 52.8603, 35.277, 843.253, osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);

    osg::Vec3d vPoint1;
    point1.toWorld(vPoint1, mMap3dWidget->getMapNode()->getTerrain());
    modelTruck->setPosition(point1);
    modelTruck->setScale(osg::Vec3(1,1,1));

    osgEarth::Viewpoint vp;
    vp.focalPoint() = point1;
    vp.range()= 500;
    vp.heading()->set(50, osgEarth::Units::DEGREES);
    //vp.pitch()->set(-25, osgEarth::Units::DEGREES);
    mMap3dWidget->setViewpoint(vp, 5);
}
