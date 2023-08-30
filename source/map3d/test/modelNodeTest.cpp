#include "modelNodeTest.h"
#include <QJsonObject>
#include<QObject>
#include <QColor>
#include <QRandomGenerator>
#include <QTimer>

ModelNodeTest::ModelNodeTest(NetworkManager *networkManager):
    mNetworkManager(networkManager)
{
    QObject::connect(mNetworkManager, &NetworkManager::flyableQueueDeclared, [this]{mFlyableQueueDeclared = true;});
    //--create and update aircraft info------------------------
    QTimer *timerUpdateAircraft = new QTimer();
    QObject::connect(timerUpdateAircraft, &QTimer::timeout, [this](){
        createFlyableInfo();
        updateFlyableInfo();
        for(auto& jsonDocument: mFlyableDataList)
        mNetworkManager->sendFlyableData(jsonDocument.toJson(QJsonDocument::Compact));
    });
    timerUpdateAircraft->start(1000);
}

void ModelNodeTest::createFlyableInfo()
{
    if(mFlyableDataList.count() > mMaxFlyableNumber)
        return;
    //---------------------------------------------------------
    QString name = "Flyable" + QString::number(mFlyableDataList.count());
    int id = 10000 + mFlyableDataList.count();
    QColor color("red");
    double longitude = 48 + (QRandomGenerator::global()->generate() % (59 - 48));
    double latitude = 27 + (QRandomGenerator::global()->generate() % (38 - 27));
    double altitude = (2000 + (QRandomGenerator::global()->generate() % (9000 - 2000)));
    double heading = (0 + (QRandomGenerator::global()->generate() % 361));
    //--------------------------------------------------------
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;

    jsonObject.insert("Name", name);
    jsonObject.insert("Id", id);
    jsonObject.insert("Color", color.name());
    jsonObject.insert("Url2d", "/url2d");
    jsonObject.insert("Url3d", "/url3d");
    jsonObject.insert("Longitude", longitude);
    jsonObject.insert("Latitude", latitude);
    jsonObject.insert("Altitude", altitude);
    jsonObject.insert("Heading", heading);
    jsonObject.insert("Speed", 200);

    jsonDocument.setObject(jsonObject);
    mFlyableDataList.append(jsonDocument);
}

void ModelNodeTest::updateFlyableInfo()
{
    for(auto& jsonDocument: mFlyableDataList)
    {
        QJsonObject jsonObject = jsonDocument.object();
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
        if(rn < 1)
            altitude += altitude - 50;
        else
            altitude += altitude + 50;
        //--------------------------------------------------------
        jsonObject["Longitude"] = longitude;
        jsonObject["Latitude"] = latitude;
        jsonObject["Altitude"] = altitude;
        jsonObject["Heading"] = heading;
        jsonObject["Speed"] =  speed;

        jsonDocument.setObject(jsonObject);
    }
}