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
            for(auto& flybleData: mFlyableDataList){
                mNetworkManager->sendData(flybleData.flyableDoc.toJson(QJsonDocument::Compact));
                mNetworkManager->sendData(flybleData.statusDoc.toJson(QJsonDocument::Compact));
                mNetworkManager->sendData(flybleData.lineDoc.toJson(QJsonDocument::Compact));
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

    QJsonObject jsonData;
    jsonData.insert("Name", name);
    jsonData.insert("Id", id);
    jsonData.insert("Color", color.name());
    jsonData.insert("Url2d", "../data/models/aircraft/aircraft.png");
    jsonData.insert("Url3d", "../data/models/aircraft/boeing-747.osgb");
    jsonData.insert("Longitude", longitude);
    jsonData.insert("Latitude", latitude);
    jsonData.insert("Altitude", altitude);
    jsonData.insert("Heading", heading);
    jsonData.insert("Speed", speed);

    QJsonArray layer;
    layer.push_back(102);
    if(speed < 200)
        layer.push_back(103);
    else
        layer.push_back(104);
    jsonData.insert("LayersId", layer);

    jsonObject.insert("Data", jsonData);
    jsonDocument.setObject(jsonObject);
    FlayableData flaybleData;
    flaybleData.flyableDoc = jsonDocument;

    //--status node-----------------------------------------------
    QJsonDocument jsonDocStatus;
    QJsonObject jsonObjectStatus;

    jsonObjectStatus.insert("Type", "Status");

    QJsonObject jsonObjectStatusData;
    jsonObjectStatusData.insert("Name", name);
    jsonObjectStatusData.insert("Id", id);
    jsonObjectStatusData.insert("Longitude", longitude);
    jsonObjectStatusData.insert("Latitude", latitude);
    jsonObjectStatusData.insert("Altitude", altitude);
    jsonObjectStatusData.insert("Heading", heading);
    jsonObjectStatusData.insert("Speed", speed);
    jsonObjectStatusData.insert("LayerId", 106);

    jsonObjectStatus.insert("Data", jsonObjectStatusData);
    jsonDocStatus.setObject(jsonObjectStatus);
    flaybleData.statusDoc = jsonDocStatus;

    //--route node------------------------------------------------
    QJsonDocument jsonDocLine;
    QJsonObject jsonObjectLine;

    jsonObjectLine.insert("Type", "Route");

    QJsonObject jsonObjectLineData;
    jsonObjectLineData.insert("Name", name);
    jsonObjectLineData.insert("Id", id);

    QJsonArray points;
    QJsonObject point1;
    point1.insert("Longitude", longitude);
    point1.insert("Latitude", latitude);
    point1.insert("Altitude", altitude);
    points.push_back(point1);

    jsonObjectLineData.insert("Points", points);
    jsonObjectLineData.insert("LayerId", 106);

    jsonObjectLine.insert("Data", jsonObjectLineData);
    jsonDocLine.setObject(jsonObjectLine);
    flaybleData.lineDoc = jsonDocLine;

    mFlyableDataList.append(flaybleData);
}

void FlyableNodeTest::updateFlyableInfo()
{
    for(auto& flaybleData: mFlyableDataList)
    {
        QJsonObject dataObject = flaybleData.flyableDoc.object().value("Data").toObject();
        //------------------------
        double latitude = dataObject["Latitude"].toDouble();
        double longitude = dataObject["Longitude"].toDouble();
        double altitude = dataObject["Altitude"].toDouble();
        double speed /*= jsonObject["Speed"].toDouble()*/;
        double heading = dataObject["Heading"].toDouble();

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
        dataObject["Longitude"] = longitude;
        dataObject["Latitude"] = latitude;
        dataObject["Altitude"] = altitude;
        dataObject["Heading"] = heading;
        dataObject["Speed"] =  speed;

        QJsonObject jsonObject;
        jsonObject.insert("Type", "Flyable");
        jsonObject.insert("Data", dataObject);
        flaybleData.flyableDoc.setObject(jsonObject);
        //--status node-----------------------------------------------
        QJsonDocument jsonDocStatus;
        QJsonObject jsonObjectStatus;
        QJsonObject jsonObjectStatusData;

        int id = dataObject["Id"].toInt();
        QString name = dataObject["Name"].toString();

        jsonObjectStatus.insert("Type", "Status");
        jsonObjectStatusData.insert("Name", name);
        jsonObjectStatusData.insert("Id", id);
        jsonObjectStatusData.insert("Longitude", longitude);
        jsonObjectStatusData.insert("Latitude", latitude);
        jsonObjectStatusData.insert("Altitude", altitude);
        jsonObjectStatusData.insert("Heading", heading);
        jsonObjectStatusData.insert("Speed", speed);
        jsonObjectStatusData.insert("LayerId", 106);
        jsonObjectStatus.insert("Data", jsonObjectStatusData);
        jsonDocStatus.setObject(jsonObjectStatus);
        flaybleData.statusDoc = jsonDocStatus;

        //--route node------------------------------------------------
        QJsonDocument jsonDocLine;
        QJsonObject jsonObjectLine;

        jsonObjectLine.insert("Type", "Route");

        QJsonObject jsonObjectLineData;
        jsonObjectLineData.insert("Name", name);
        jsonObjectLineData.insert("Id", id);

        QJsonArray points = dataObject.value("Points").toArray();
        QJsonObject point;
        point.insert("Longitude", longitude);
        point.insert("Latitude", latitude);
        point.insert("Altitude", altitude);
        points.push_back(point);

        jsonObjectLineData.insert("Points", points);
        jsonObjectLineData.insert("LayerId", 106);

        jsonObjectLine.insert("Data", jsonObjectLineData);
        jsonDocLine.setObject(jsonObjectLine);
        flaybleData.lineDoc = jsonDocLine;
    }
}
