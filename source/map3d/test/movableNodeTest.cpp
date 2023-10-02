#include "movableNodeTest.h"

#include <QColor>
#include <QTimer>
#include <QRandomGenerator>
#include <QJsonObject>
#include <QJsonArray>

MovableNodeTest::MovableNodeTest(NetworkManager *networkManager):
    mNetworkManager(networkManager)
{
    QObject::connect(mNetworkManager, &NetworkManager::dataQueueDeclared, [this]{
        mMovableQueueDeclared = true;

        //--create and update movable info------------------------
        QTimer *timerUpdateMovable = new QTimer();
        QObject::connect(timerUpdateMovable, &QTimer::timeout, [this](){
            createMovableInfo();
            updateMovableInfo();
            if(mMovableQueueDeclared)
                for(auto& movableData: mMovableDataList){
                    mNetworkManager->sendData(movableData.movableDoc.toJson(QJsonDocument::Compact));
                    mNetworkManager->sendData(movableData.statusDoc.toJson(QJsonDocument::Compact));
                    mNetworkManager->sendData(movableData.lineDoc.toJson(QJsonDocument::Compact));
                }
        });
        timerUpdateMovable->start(1000);
    });
}

void MovableNodeTest::createMovableInfo()
{
    if(mMovableDataList.count() >= mMaxMovableNumber)
        return;
    //---------------------------------------------------------
    QString name = "Movable" + QString::number(mMovableDataList.count());
    int id = 40000 + mMovableDataList.count();
    QColor color("red");
    double longitude = 48 + (QRandomGenerator::global()->generate() % (59 - 48));
    double latitude = 27 + (QRandomGenerator::global()->generate() % (38 - 27));
    double altitude =0;/* (2000 + (QRandomGenerator::global()->generate() % (9000 - 2000)));*/
    double heading = (0 + (QRandomGenerator::global()->generate() % 361));
    double speed = (100 + (QRandomGenerator::global()->generate() % (300-100)));
    //--------------------------------------------------------
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;

    jsonObject.insert("Type", "Movable");

    QJsonObject jsonData;
    jsonData.insert("Name", name);
    jsonData.insert("Id", id);
    jsonData.insert("Color", color.name());
    jsonData.insert("Url2d", "../data/images/model/car.png");
    jsonData.insert("Url3d", "../data/models/car.osgb");
    jsonData.insert("Longitude", longitude);
    jsonData.insert("Latitude", latitude);
    jsonData.insert("Altitude", altitude);
    jsonData.insert("Heading", heading);
    jsonData.insert("Speed", speed);

    QJsonArray layer;
    layer.push_back(202);
    if(speed < 200)
        layer.push_back(203);
    else
        layer.push_back(204);
    jsonData.insert("LayersId", layer);

    jsonObject.insert("Data", jsonData);
    jsonDocument.setObject(jsonObject);
    MovableData movableData;
    movableData.movableDoc = jsonDocument;

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
    jsonObjectStatusData.insert("LayerId", 206);

    jsonObjectStatus.insert("Data", jsonObjectStatusData);
    jsonDocStatus.setObject(jsonObjectStatus);
    movableData.statusDoc = jsonDocStatus;

    //--Line node------------------------------------------------
    QJsonDocument jsonDocLine;
    QJsonObject jsonObjectLine;

    jsonObjectLine.insert("Type", "Line");

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
    jsonObjectLineData.insert("LayerId", 206);

    jsonObjectLine.insert("Data", jsonObjectLineData);
    jsonDocLine.setObject(jsonObjectLine);
    movableData.lineDoc = jsonDocLine;

    mMovableDataList.append(movableData);
}

void MovableNodeTest::updateMovableInfo()
{
    for(auto& movableData: mMovableDataList)
    {
        QJsonObject dataObject = movableData.movableDoc.object().value("Data").toObject();
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
        altitude = 0;
        //--------------------------------------------------------
        dataObject["Longitude"] = longitude;
        dataObject["Latitude"] = latitude;
        dataObject["Altitude"] = altitude;
        dataObject["Heading"] = heading;
        dataObject["Speed"] =  speed;

        QJsonObject jsonObject;
        jsonObject.insert("Type", "Movable");
        jsonObject.insert("Data", dataObject);
        movableData.movableDoc.setObject(jsonObject);
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
        jsonObjectStatusData.insert("LayerId", 206);
        jsonObjectStatus.insert("Data", jsonObjectStatusData);
        jsonDocStatus.setObject(jsonObjectStatus);
        movableData.statusDoc = jsonDocStatus;

        //--Line node------------------------------------------------
        QJsonDocument jsonDocLine;
        QJsonObject jsonObjectLine;

        jsonObjectLine.insert("Type", "Line");

        QJsonObject jsonObjectLineData;
        jsonObjectLineData.insert("Name", name);
        jsonObjectLineData.insert("Id", id);

        QJsonArray points = movableData.lineDoc.object().value("Data").toObject().value("Points").toArray();
        QJsonObject point;
        point.insert("Longitude", longitude);
        point.insert("Latitude", latitude);
        point.insert("Altitude", altitude);
        points.push_back(point);
        if (points.size() > 100)
            points.removeFirst();
        jsonObjectLineData.insert("Points", points);
        jsonObjectLineData.insert("LayerId", 205);

        jsonObjectLine.insert("Data", jsonObjectLineData);
        jsonDocLine.setObject(jsonObjectLine);
        movableData.lineDoc = jsonDocLine;
    }
}
