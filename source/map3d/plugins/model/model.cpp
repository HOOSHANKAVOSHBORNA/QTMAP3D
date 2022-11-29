    #include "model.h"
#include "map3dwidget.h"
#include "toolbarwidget.h"
#include "draw.h"
#include "truck.h"
#include "rocket.h"
#include "websocketclient.h"
#include "system.h"
#include "station.h"

#include <QDebug>
#include <QMainWindow>
#include <QTimer>
#include <QDockWidget>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

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
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgViewer/Viewer>

//const QString FLYING = "Flying";
const QString AIRPLANE = "Airplane";
const QString ROCKET = "Rocket";
const QString TRUCK = "Truck";

Model::Model(QWidget *parent)
    : PluginInterface(parent)
{
    Q_INIT_RESOURCE(modelqml);
    Q_INIT_RESOURCE(modelicon);
}

void Model::setUpUI()
{
    mTrackModelWidget = new TrackModelWidget();
    mTrackModelWidget->setMinimaizeWidget(true);
    //    mMainWindow->addMenuWidget(mTrackModelWidget);
    //    mTrackModelWidget->hide();
    /////DockWidget
    QObject::connect(mTrackModelWidget ,&TrackModelWidget::onPin,this,&Model::onToolBarWidgetPin);
    mDockTrackModelWidget = new QDockWidget("Track Models",mMap3dWidget);
    mDockTrackModelWidget->setAllowedAreas( Qt::RightDockWidgetArea);
    mDockTrackModelWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mDockTrackModelWidget->setMaximumWidth(200);
    mDockTrackModelWidget->setMinimumWidth(200);
    mMainWindow->addDockWidget(Qt::RightDockWidgetArea,mDockTrackModelWidget);
    mDockTrackModelWidget->hide();

    ///
    QObject::connect(mTrackModelWidget,&TrackModelWidget::onModelClicked,this ,&Model::clickedTrackNode);

    ToolBarWidget::Category cat = ToolBarWidget::Category::Model;

    QString nameAddAirplaineModel = "Add Airplan Model";
    mToolBar->addItem(cat, nameAddAirplaineModel, "qrc:/res/airplan.png");

    QString nameAddRocketModel = "Add Rocket Model";
    mToolBar->addItem(cat, nameAddRocketModel, "");

    QString nameAddTruckModel = "Add Truck Model";
    mToolBar->addItem(cat, nameAddTruckModel, "qrc:/res/missleLauncher_2.png");

    QString nameAddSystemModel = "Add System Model";
    mToolBar->addItem(cat, nameAddSystemModel, "");

    QString nameAddStationModel = "Add Station Model";
    mToolBar->addItem(cat, nameAddStationModel, "");

    QString nameTrack = "Track Models";
    mToolBar->addItem(cat, nameTrack, "qrc:/res/tracking.png",true);
    //run demo ------------------------------------------------
//    QTimer *timer = new QTimer(this);
//    connect(timer, &QTimer::timeout,this, &Model::demo);
//    timer->start(10000);
    //---------------------------------------------------------

    QObject::connect(mToolBar,&ToolBarWidget::onItemClicked, [=](ToolBarWidget::Category category ,QString name, bool isCheck){
        if(cat == category && name == nameAddAirplaineModel)
        {
            osg::Vec3d position(52.8601, 35.277, 9100);
            QString name = AIRPLANE + QString::number(mModels[AIRPLANE].count());
            addAirplaineModel(name, position, -30);
            //demo();
        }
        if(cat == category && name == nameAddRocketModel)
        {
            // fallow racket
            if(!mModels[AIRPLANE].isEmpty())
            {
                auto truckNames = mModels[TRUCK].keys();
                for(auto truckName: truckNames)
                {
                    auto modeltruck = dynamic_cast<Truck*>(mModels[TRUCK][truckName]);
                    if(modeltruck->hasRocket())
                    {
//                        addRocketModel(modeltruck->getPosition().vec3d());
//                        auto modelRocket = dynamic_cast<Rocket*>(mModels[ROCKET].last());
                        auto activeRocket = modeltruck->getActiveRocket();
                        auto modelAirplane = dynamic_cast<Airplane*>(mModels[AIRPLANE].last());
                        modelAirplane->stop();//
                        activeRocket->setFollowModel(modelAirplane);
                        //modelRocket->setTruckModel(modeltruck);

//                        modeltruck->aimTarget(modelAirplane->getPosition().vec3d());

                        modelAirplane->setFollowModel(activeRocket);
                        modelAirplane->setTruckModel(modeltruck);

                        modeltruck->shoot(modelAirplane->getPosition().vec3d(), 2000);//1000 m/s
//                        activeRocket->setPosition(modeltruck->getPosition());
//                        mMap3dWidget->addNode(activeRocket);
//                        activeRocket->shoot(modelAirplane->getPosition().vec3d(), 3000);

                        mMap3dWidget->setTrackNode(activeRocket->getGeoTransform());

                        //draw line
//                        osg::Vec3d truckPosition;
//                        modeltruck->getPosition().toWorld(truckPosition);
//                        osg::Vec3d wPoint1;
//                        modelAirplane->getPosition().toWorld(wPoint1);
//                        osg::Vec3Array* keyPoint = new osg::Vec3Array;
//                        keyPoint->push_back(truckPosition + osg::Vec3d(5, 0, -2.6));
//                        keyPoint->push_back(wPoint1);
//                        mMap3dWidget->mMapRoot->addChild(drawLine(keyPoint, 1.0));
                        break;
                    }
                }
            }
        }
        else if(cat == category && name == nameAddTruckModel)
        {
            addTruckModel();
        }
        else if(cat == category && name == nameAddSystemModel)
        {
            osg::Vec3d position(52.9, 35.3, 842.5);
            addSystemModel(position);
        }
        else if(cat == category && name == nameAddStationModel)
        {
            osg::Vec3d position(53, 35.2, 842.5);
            addStationModel(position);
        }
        if(cat == category && name == nameTrack)
        {
            if(isCheck)
            {
                mMainWindow->addMenuWidget(mTrackModelWidget);
                mTrackModelWidget->show();
                if(mIsPin)
                    mDockTrackModelWidget->show();
                //mMap3dWidget->setTrackNode(mCurrentModel->getGeoTransform());
            }
            else
            {
                mTrackModelWidget->setUnTrackAll(true);
                mMainWindow->removeMenuWidget(mTrackModelWidget);
                mDockTrackModelWidget->hide();
                //                mTrackModelWidget->move(mMainWindow->width() -200,0);
                mMap3dWidget->unTrackNode();
            }
            //mMap3dWidget->setTrackNode(nullptr);
        }
    });
    //--websocket data-------------------------------------------------------------------
    QObject::connect(mMainWindow->getWebSocket(), &WebSocketClient::messageReceived,this ,&Model::onMessageReceived);
}


void Model::demo()
{
    //    int index = 0;
    auto airplaneNames = mModels[AIRPLANE].keys();
    for (auto name: airplaneNames)
    {
        auto model = dynamic_cast<Airplane*>(mModels[AIRPLANE][name]);
        auto mapPoint = model->getPosition();
        osgEarth::GeoPoint  latLongPoint;
        //latLongPoint.altitudeMode() = osgEarth::AltitudeMode::ALTMODE_ABSOLUTE;
        //mapPoint.transform(osgEarth::SpatialReference::get("wgs84"), latLongPoint);
        osg::Vec3d currentPos;
        mapPoint.toWorld(currentPos);
        //osg::Vec3d currentPos(latLongPoint.vec3d());
        //-- 500 km/h ~ 139 m/s ------------------
        //    int randomX = 10*(138 + (qrand() % 139));
        //    int randomY = 10*(138 + (qrand() % 139));
        //    int val = qrand() % 4;
        //    if(val == 1)
        //        currentPos += osg::Vec3d(randomX, randomY, 0.0);
        //    else if(val == 2)
        //        currentPos += osg::Vec3d(randomX, -randomY, 0.0);
        //    else if(val == 3)
        //        currentPos += osg::Vec3d(-randomX, randomY, 0.0);
        //    else
        //        currentPos += osg::Vec3d(-randomX, -randomY, 0.0);

        int randomX = (100 + (qrand() % 19));
        int randomY = (100 + (qrand() % 19));
        int val = qrand() % 4;
        if(val == 1)
            mapPoint.vec3d() += osg::Vec3d(randomX/10000.0, randomY/10000.0, 0.0);
        else if(val == 2)
            mapPoint.vec3d() += osg::Vec3d(randomX/10000.0, -randomY/10000.0, 0.0);
        else if(val == 3)
            mapPoint.vec3d() += osg::Vec3d(-randomX/10000.0, randomY/10000.0, 0.0);
        else
            mapPoint.vec3d() += osg::Vec3d(-randomX/10000.0, -randomY/10000.0, 0.0);

        //    setPosition(mCurrentWorldPoint, 138);
        latLongPoint.fromWorld(osgEarth::SpatialReference::get("wgs84"), currentPos);
        //qDebug()<<QString::fromUtf8(latLongPoint.toString().c_str());
        model->flyTo(mapPoint.vec3d(),30, 138);

        // fallow racket
        //        auto truckNames = mModels[TRUCK].keys();
        //        for(auto truckName: truckNames)
        //        {
        //            auto modeltruck = dynamic_cast<Truck*>(mModels[TRUCK][truckName]);
        //            if(modeltruck->shoot())
        //            {
        //                addRocketModel(modeltruck->getPosition().vec3d());
        //                auto modelRocket = dynamic_cast<FlyingModel*>(mModels[ROCKET].last());
        //                modelRocket->flyTo(mapPoint.vec3d(), 120);
        //                model->setFollowingModel(modelRocket);
        //                break;
        //            }
        //        }
        //        index += 1;
    }
}

void Model::onToolBarWidgetPin(bool isPin)
{
    mIsPin = isPin;
    if(isPin){
        mMainWindow->removeMenuWidget(mTrackModelWidget);
        mDockTrackModelWidget->show();
        mDockTrackModelWidget->setWidget(mTrackModelWidget);
    }else{
        mDockTrackModelWidget->hide();
        mMainWindow->addMenuWidget(mTrackModelWidget);
        //mTrackModelWidget->setParent(mMainWindow);
        //mTrackModelWidget->move(mMainWindow->width() -200,0);
        mTrackModelWidget->show();

    }

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

void Model::flyTo(QString type, QString name, const osg::Vec3d &pos, double speed)
{
    if(mModels[type].contains(name))
    {
        Airplane* model = dynamic_cast<Airplane*>(mModels[type][name]);
        if(model != nullptr)
            model->flyTo(pos,100, speed);
    }
}

void Model::addTruckModel()
{
    osg::Vec3d position(52.8603, 35.274, 842.5);
    //create and setting model--------------------------------------------
    osg::ref_ptr<Truck> model = new Truck(mMap3dWidget->getMapNode());
    QString name = TRUCK + QString::number(mModels[TRUCK].count());
    model->setName(name.toStdString());
    model->setGeographicPosition(position, 0.0);
    //model->setLocalRotation(osg::Quat(osg::inDegrees(-30.0),osg::Z_AXIS));
    model->setScale(osg::Vec3(1,1,1));



    //    QObject::connect(model.get(), &FlyingModel::positionChanged, [=](osgEarth::GeoPoint position){
    //        positionChanged(ROCKET, name, position);
    //    });

    //add to container-----------------------------------------------------
    mModels[TRUCK][name] = model;

    //add to map ---------------------------------------------------------
    mMap3dWidget->addNode(model);
    mMap3dWidget->goPosition(position.x(), position.y(), position.z() + 600);

    //add to track widget ------------------------------------------------
    mTrackModelWidget->addModel(TRUCK, name);
    mTrackModelWidget->setModelPosition(TRUCK, name, position.x(), position.y(), position.z());

    //create random position ---------------------------------------------
    //    double rnd = QRandomGenerator::global()->generateDouble();
    double rndRotate = qrand() % 360;
    model->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(rndRotate), osg::Z_AXIS));

    double rndPX = (qrand() % 200)/100000.0;
    double rndPY = (qrand() % 200)/100000.0;
    osg::Vec3d nPosition(rndPX, rndPY, 0.0);


    nPosition += position;
    //model->setLatLongPosition(nPosition);
    //mMap3dWidget->goPosition(nPosition.x(), nPosition.y(), nPosition.z() + 500);
    //move
    model->moveTo(nPosition,10);
}

void Model::addAirplaineModel(QString name, osg::Vec3d position, double heading)
{
//    osg::Vec3d position(52.8601, 35.277, 9100);
    //osg::Vec3d position(52.8601, 35.277, 844);

    //create and setting model--------------------------------------------
    osg::ref_ptr<osg::Node>  node = osgDB::readRefNodeFile("../map3dlib/data/models/new-airplane-red.osgb");
    osg::ref_ptr<Airplane> model = new Airplane(mMap3dWidget, mMap3dWidget->getMapNode(), node);
//    QString name = AIRPLANE + QString::number(mModels[AIRPLANE].count());
    model->setName(name.toStdString());
    model->setGeographicPosition(position, heading);
//    model->setScale(osg::Vec3(0.09f,0.09f,0.09f));

    QObject::connect(model.get(), &BaseModel::positionChanged, [=](osgEarth::GeoPoint position){
        //positionChanged(AIRPLANE, name, position);

        auto truckNames = mModels[TRUCK].keys();
        for(auto truckName: truckNames)
        {
            auto truck = dynamic_cast<Truck*>(mModels[TRUCK][truckName]);
            if(truck->hasRocket())
            {
//                osg::Vec3d wPoint;
//                position.toWorld(wPoint);
                truck->aimTarget(position.vec3d());
            }
        }
    });
    //add to container-----------------------------------------------------
    mModels[AIRPLANE][name] = model;


    //add to map ---------------------------------------------------------
    mMap3dWidget->addNode(model);
    //mMap3dWidget->goPosition(position.x(), position.y(), position.z() + 500);

    //add to track widget ------------------------------------------------
    mTrackModelWidget->addModel(AIRPLANE, name);
    mTrackModelWidget->setModelPosition(AIRPLANE, name, position.x(), position.y(), position.z());

    //    double rnd = QRandomGenerator::global()->generateDouble();
    //    double rnd = qrand() % 360;
    //    model->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(rnd), osg::Z_AXIS));

    //hit------------------------------------------------------------------
    QObject::connect(model.get(), &BaseModel::hit, [=](BaseModel *other){

        mModels[AIRPLANE].remove(QString(model->getName().c_str()));
        mTrackModelWidget->removeModel(AIRPLANE, QString(model->getName().c_str()));
    });
}

void Model::addRocketModel(osg::Vec3d position)
{

    //create and setting model--------------------------------------------
    osg::ref_ptr<Rocket> model = new Rocket(mMap3dWidget->getMapNode());
    QString name = ROCKET + QString::number(mModels[ROCKET].count());
    model->setName(name.toStdString());
    model->setGeographicPosition(position, 0.0);
    model->setScale(osg::Vec3(1,1,1));

    QObject::connect(model.get(), &BaseModel::positionChanged, [=](osgEarth::GeoPoint position){
        positionChanged(ROCKET, name, position);
    });

    //add to container-----------------------------------------------------
    mModels[ROCKET][name] = model;


    //add to map ---------------------------------------------------------
    mMap3dWidget->addNode(model);
    //mMap3dWidget->goPosition(position.x(), position.y(), position.z() + 500);
    mMap3dWidget->setTrackNode(model->getGeoTransform());
//    mMap3dWidget->setObjectInfoWidgetVisible(false);

    //add to track widget ------------------------------------------------
    mTrackModelWidget->addModel(ROCKET, name);
    mTrackModelWidget->setModelPosition(ROCKET, name, position.x(), position.y(), position.z());

    //    double rnd = QRandomGenerator::global()->generateDouble();
    //    double rnd = qrand() % 360;
    //    model->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(rnd), osg::Z_AXIS));
    //hit------------------------------------------------------------------
    QObject::connect(model.get(), &BaseModel::hit, [=](BaseModel *other){

        mModels[ROCKET].remove(QString(model->getName().c_str()));
        mTrackModelWidget->removeModel(ROCKET, QString(model->getName().c_str()));
    });
}

void Model::addSystemModel(osg::Vec3d position)
{
    //create and setting model--------------------------------------------
    osg::ref_ptr<System> model = new System(mMap3dWidget->getMapNode());
    QString name = "System" + QString::number(mModels["System"].count());
    model->setName(name.toStdString());
    model->setGeographicPosition(position, 0.0);
    model->setScale(osg::Vec3(1,1,1));
    //add to container-----------------------------------------------------
    mModels["System"][name] = model;


    //add to map ---------------------------------------------------------
    mMap3dWidget->addNode(model);
    //add to track widget ------------------------------------------------
    mTrackModelWidget->addModel("System", name);
    mTrackModelWidget->setModelPosition("System", name, position.x(), position.y(), position.z());
}

void Model::addStationModel(osg::Vec3d position)
{
    //create and setting model--------------------------------------------
    osg::ref_ptr<Station> model = new Station(mMap3dWidget->getMapNode());
    QString name = "Station" + QString::number(mModels["Station"].count());
    model->setName(name.toStdString());
    model->setGeographicPosition(position, 0.0);
    model->setScale(osg::Vec3(1,1,1));
    //add to container-----------------------------------------------------
    mModels["Station"][name] = model;


    //add to map ---------------------------------------------------------
    mMap3dWidget->addNode(model);
    //add to track widget ------------------------------------------------
    mTrackModelWidget->addModel("Station", name);
    mTrackModelWidget->setModelPosition("Station", name, position.x(), position.y(), position.z());
}

void Model::clickedTrackNode(QString type, QString name, bool isClick)
{
    if (isClick){
        //        osg::Node* node =mModels[type][name];
        mMap3dWidget->setTrackNode(mModels[type][name]->getGeoTransform());
        //qDebug()<<"fgd";

//        if(type == AIRPLANE) {
//            mMap3dWidget->setObjectInfoWidgetVisible(true);
//            mMap3dWidget->setSelectedAirplane(mModels[type][name]);
//        } else {
//            mMap3dWidget->setObjectInfoWidgetVisible(false);
//        }

    } else {

        mMap3dWidget->unTrackNode();
//        mMap3dWidget->setObjectInfoWidgetVisible(false);

    }
}

void Model::positionChanged(QString type, QString name, osgEarth::GeoPoint position)
{
    mTrackModelWidget->setModelPosition(type, name, position.x(), position.y(), position.z());
}

void Model::onMessageReceived(const QJsonDocument &message)
{
    if(message.object().value("Name").toString() == "Target")
    {
        QJsonObject data = message.object().value("Data").toObject();
        qDebug()<<"target:"<< data;

        double latitude = data.value("Latitude").toDouble();
        double longitude = data.value("Longitude").toDouble();
        double altitude = data.value("Altitude").toDouble();
        double speed = data.value("Speed").toDouble();
        double heading = data.value("Heading").toDouble();
        osg::Vec3d position(latitude, longitude, altitude);
        QString name = QString::number(data.value("TN").toInt());
        if(mModels.contains(AIRPLANE) && mModels[AIRPLANE].contains(name))
        {
            Airplane* model = dynamic_cast<Airplane*>(mModels[AIRPLANE][name]);
            model->flyTo(position, heading, speed);
        }
        else
            addAirplaineModel(name, position, -heading);


    }
}

