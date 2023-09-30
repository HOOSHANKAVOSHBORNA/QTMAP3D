#include "flyableNodeTest.h"
#include <QJsonObject>
#include <QObject>
#include <QColor>
#include <QRandomGenerator>
#include <QTimer>
#include <QJsonArray>

FlyableNodeTest::FlyableNodeTest(NetworkManager *networkManager):
    mNetworkManager(networkManager)
{
    QObject::connect(mNetworkManager, &NetworkManager::dataQueueDeclared, [this]{
    mFlyableQueueDeclared = true;
//    createFlyableInfo();
//    for(auto& jsonDocument: mFlyableDataList)
//                mNetworkManager->sendFlyableData(jsonDocument.toJson(QJsonDocument::Compact));
    //--create and update aircraft info------------------------
    QTimer *timerUpdateAircraft = new QTimer();
    QObject::connect(timerUpdateAircraft, &QTimer::timeout, [this](){
        createFlyableInfo();
        updateFlyableInfo();
        if(mFlyableQueueDeclared)
            for(auto& flaybleData: mFlyableDataList){
                mNetworkManager->sendData(flaybleData.flyableDoc.toJson(QJsonDocument::Compact));
                mNetworkManager->sendData(flaybleData.statusDoc.toJson(QJsonDocument::Compact));
            }
    });
    timerUpdateAircraft->start(1000);
    });
}

void FlyableNodeTest::createFlyableInfo()
{
    if(mFlyableDataList.count() >= mMaxFlyableNumber)
        return;
    //---------------------------------------------------------
    QString name = "Flyable" + QString::number(mFlyableDataList.count());
    int id = 10000 + mFlyableDataList.count();
    QColor color("red");
    double longitude = 48 + (QRandomGenerator::global()->generate() % (59 - 48));
    double latitude = 27 + (QRandomGenerator::global()->generate() % (38 - 27));
    double altitude =0;/* (2000 + (QRandomGenerator::global()->generate() % (9000 - 2000)));*/
    double heading = (0 + (QRandomGenerator::global()->generate() % 361));
    double speed = (100 + (QRandomGenerator::global()->generate() % (300-100)));
    //--------------------------------------------------------
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;

    jsonObject.insert("Type", "Flyable");
    jsonObject.insert("Name", name);
    jsonObject.insert("Id", id);
    jsonObject.insert("Color", color.name());
    jsonObject.insert("Url2d", "../data/models/aircraft/aircraft.png");
    jsonObject.insert("Url3d", "../data/models/aircraft/boeing-747.osgb");
    jsonObject.insert("Longitude", longitude);
    jsonObject.insert("Latitude", latitude);
    jsonObject.insert("Altitude", altitude);
    jsonObject.insert("Heading", heading);
    jsonObject.insert("Speed", speed);

    QJsonArray layer;
    layer.push_back(102);
    if(speed < 200)
        layer.push_back(103);
    else
        layer.push_back(104);
    jsonObject.insert("LayersId", layer);

    jsonDocument.setObject(jsonObject);
    FlayableData flaybleData;
    flaybleData.flyableDoc = jsonDocument;

    //--status node-----------------------------------------------
    QJsonDocument jsonDocStatus;
    QJsonObject jsonObjectStatus;

    jsonObjectStatus.insert("Type", "Status");
    jsonObjectStatus.insert("Name", name);
    jsonObjectStatus.insert("Id", id);
    jsonObjectStatus.insert("Longitude", longitude);
    jsonObjectStatus.insert("Latitude", latitude);
    jsonObjectStatus.insert("Altitude", altitude);
    jsonObjectStatus.insert("Heading", heading);
    jsonObjectStatus.insert("Speed", speed);
    jsonObjectStatus.insert("LayerId", 106);
    jsonDocStatus.setObject(jsonObjectStatus);
    flaybleData.statusDoc = jsonDocStatus;

    mFlyableDataList.append(flaybleData);
}

void FlyableNodeTest::updateFlyableInfo()
{
    for(auto& flaybleData: mFlyableDataList)
    {
        QJsonObject jsonObject = flaybleData.flyableDoc.object();
        //------------------------
        double latitude = jsonObject["Latitude"].toDouble();
        double longitude = jsonObject["Longitude"].toDouble();
        double altitude = jsonObject["Altitude"].toDouble();
        double speed /*= jsonObject["Speed"].toDouble()*/;
        double heading = jsonObject["Heading"].toDouble();

        int rn = (0 + (QRandomGenerator::global()->generate() % 10000));
        if(rn < 1)
            heading = (0 + (QRandomGenerator::global()->generate() % 361));

        speed = (138 + (QRandomGenerator::global()->generate() % 137));

        double pi = 3.14159265359;
        double teta = (90 - heading)* (pi / 180);
        double step = 10.0/10000.0;
        longitude += step * std::cos(teta);
        latitude += step * std::sin(teta);

        rn = (0 + (QRandomGenerator::global()->generate() % 2));
//        if(rn < 1)
//            altitude += altitude - 5;
//        else
//            altitude += altitude + 5;
        altitude = 2000;
        //--------------------------------------------------------
        jsonObject["Longitude"] = longitude;
        jsonObject["Latitude"] = latitude;
        jsonObject["Altitude"] = altitude;
        jsonObject["Heading"] = heading;
        jsonObject["Speed"] =  speed;

        flaybleData.flyableDoc.setObject(jsonObject);
        //--status node-----------------------------------------------
        QJsonDocument jsonDocStatus;
        QJsonObject jsonObjectStatus;

        int id = jsonObject["Id"].toInt();
        QString name = jsonObject["Name"].toString();

        jsonObjectStatus.insert("Type", "Status");
        jsonObjectStatus.insert("Name", name);
        jsonObjectStatus.insert("Id", id);
        jsonObjectStatus.insert("Longitude", longitude);
        jsonObjectStatus.insert("Latitude", latitude);
        jsonObjectStatus.insert("Altitude", altitude);
        jsonObjectStatus.insert("Heading", heading);
        jsonObjectStatus.insert("Speed", speed);
        jsonObjectStatus.insert("LayerId", 106);
        jsonDocStatus.setObject(jsonObjectStatus);
        flaybleData.statusDoc = jsonDocStatus;
    }
}
