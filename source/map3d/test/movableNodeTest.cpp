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

        QTimer *timerRemoveAircraft = new QTimer();
        QObject::connect(timerRemoveAircraft, &QTimer::timeout, [this](){
            if (mMovableDataList.size() > 0){
                QJsonObject jsonObject = mMovableDataList[0].movableDoc.object();
                QJsonObject jsonStatusObject = mMovableDataList[0].statusDoc.object();
                QJsonObject jsonLineObject = mMovableDataList[0].lineDoc.object();

                jsonObject.remove("COMMAND");
                jsonObject.insert("COMMAND", "REMOVE");
                jsonStatusObject.remove("COMMAND");
                jsonStatusObject.insert("COMMAND", "REMOVE");
                jsonLineObject.remove("COMMAND");
                jsonLineObject.insert("COMMAND", "REMOVE");

                mMovableDataList.pop_front();

                QJsonDocument jsonDoc(jsonObject);
                QJsonDocument jsonStatusDoc(jsonStatusObject);
                QJsonDocument jsonLineDoc(jsonLineObject);

                mNetworkManager->sendData(jsonDoc.toJson(QJsonDocument::Compact));
                mNetworkManager->sendData(jsonStatusDoc.toJson(QJsonDocument::Compact));
                mNetworkManager->sendData(jsonLineDoc.toJson(QJsonDocument::Compact));
            }
        });
        timerRemoveAircraft->start(7000);
    });
}

void MovableNodeTest::createMovableInfo()
{
    if(mCount >= mMaxMovableNumber)
        return;
    //---------------------------------------------------------
    QString name = "Movable" + QString::number(mCount);
    int id = 40000 + mCount++;
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
    jsonObject.insert("COMMAND", "ADD");

    QJsonObject jsonData;
    QJsonObject nameObject;
    nameObject.insert("value", name);
    nameObject.insert("category", "Main Information");
    jsonData.insert("Name", nameObject);
    QJsonObject idObject;
    idObject.insert("value", id);
    idObject.insert("category", "Main Information");
    jsonData.insert("Id", idObject);

    jsonData.insert("Color", color.name());
    jsonData.insert("Url2d", "../data/models/car/car.png");
    jsonData.insert("Url3d", "../data/models/car/car.osgb");
    jsonData.insert("ImgSrc", "qrc:/Resources/station.jpg");

    QJsonObject longObject;
    longObject.insert("value", longitude);
    longObject.insert("category", "Location Information");
    jsonData.insert("Longitude", longObject);
    QJsonObject latObject;
    latObject.insert("value", latitude);
    latObject.insert("category", "Location Information");
    jsonData.insert("Latitude", latObject);
    QJsonObject altObject;
    altObject.insert("value", altitude);
    altObject.insert("category", "Location Information");
    jsonData.insert("Altitude", altObject);
    QJsonObject headObject;
    headObject.insert("value", heading);
    headObject.insert("category", "Location Information");
    jsonData.insert("Heading", headObject);
    QJsonObject speedObject;
    speedObject.insert("value", speed);
    speedObject.insert("category", "Location Information");
    jsonData.insert("Speed", speedObject);
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
    jsonObjectStatus.insert("COMMAND", "ADD");

    QJsonObject jsonObjectStatusData;
    jsonObjectStatusData.insert("Name", name);
    jsonObjectStatusData.insert("Id", id);
    jsonObjectStatusData.insert("Longitude", longitude);
    jsonObjectStatusData.insert("Latitude", latitude);
    jsonObjectStatusData.insert("Altitude", altitude);
    jsonObjectStatusData.insert("LayerId", 206);

    QJsonObject jsonObjectStatusFieldData;
    jsonObjectStatusFieldData.insert("Heading", heading);
    jsonObjectStatusFieldData.insert("Speed", speed);
    jsonObjectStatusData.insert("FieldData", jsonObjectStatusFieldData);

    jsonObjectStatus.insert("Data", jsonObjectStatusData);
    jsonDocStatus.setObject(jsonObjectStatus);
    movableData.statusDoc = jsonDocStatus;

    //--Line node------------------------------------------------
    QColor lineColor(255, 0, 255);
    QJsonDocument jsonDocLine;
    QJsonObject jsonObjectLine;

    jsonObjectLine.insert("Type", "Line");
    jsonObjectLine.insert("COMMAND", "ADD");

    QJsonObject jsonObjectLineData;
    jsonObjectLineData.insert("Name", name);
    jsonObjectLineData.insert("Id", id);
    jsonObjectLineData.insert("Color", lineColor.name());
    jsonObjectLineData.insert("Width", 7);

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
        double latitude = dataObject["Latitude"].toObject().value("value").toDouble();
        double longitude = dataObject["Longitude"].toObject().value("value").toDouble();
        double altitude = dataObject["Altitude"].toObject().value("value").toDouble();
        double speed /*= jsonObject["Speed"].toDouble()*/;
        double heading = dataObject["Heading"].toObject().value("value").toDouble();

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
        QJsonObject longObject;
        longObject.insert("value", longitude);
        longObject.insert("category", "Location Information");
        dataObject.insert("Longitude", longObject);
        QJsonObject latObject;
        latObject.insert("value", latitude);
        latObject.insert("category", "Location Information");
        dataObject.insert("Latitude", latObject);
        QJsonObject altObject;
        altObject.insert("value", altitude);
        altObject.insert("category", "Location Information");
        dataObject.insert("Altitude", altObject);
        QJsonObject headObject;
        headObject.insert("value", heading);
        headObject.insert("category", "Location Information");
        dataObject.insert("Heading", headObject);
        QJsonObject speedObject;
        speedObject.insert("value", speed);
        speedObject.insert("category", "Location Information");
        dataObject.insert("Speed", speedObject);

        QJsonObject jsonObject;
        jsonObject.insert("Type", "Movable");
        jsonObject.insert("COMMAND", "UPDATE");
        jsonObject.insert("Data", dataObject);
        movableData.movableDoc.setObject(jsonObject);
        //--status node-----------------------------------------------
        QJsonDocument jsonDocStatus;
        QJsonObject jsonObjectStatus;
        QJsonObject jsonObjectStatusData;

        int id = dataObject["Id"].toObject().value("value").toInt();
        QString name = dataObject["Name"].toObject().value("value").toString();

        jsonObjectStatus.insert("Type", "Status");
        jsonObjectStatus.insert("COMMAND", "UPDATE");
        jsonObjectStatusData.insert("Name", name);
        jsonObjectStatusData.insert("Id", id);
        jsonObjectStatusData.insert("Longitude", longitude);
        jsonObjectStatusData.insert("Latitude", latitude);
        jsonObjectStatusData.insert("Altitude", altitude);
        jsonObjectStatusData.insert("Heading", heading);
        jsonObjectStatusData.insert("Speed", speed);
        jsonObjectStatusData.insert("LayerId", 206);

        QJsonObject jsonObjectStatusFieldData;
        jsonObjectStatusFieldData.insert("Heading", heading);
        jsonObjectStatusFieldData.insert("Speed", speed);
        jsonObjectStatusData.insert("FieldData", jsonObjectStatusFieldData);

        jsonObjectStatus.insert("Data", jsonObjectStatusData);
        jsonDocStatus.setObject(jsonObjectStatus);
        movableData.statusDoc = jsonDocStatus;

        //--Line node------------------------------------------------
        QColor lineColor(255, 0, 255);
        QJsonDocument jsonDocLine;
        QJsonObject jsonObjectLine;

        jsonObjectLine.insert("Type", "Line");
        jsonObjectLine.insert("COMMAND", "UPDATE");

        QJsonObject jsonObjectLineData;
        jsonObjectLineData.insert("Name", name);
        jsonObjectLineData.insert("Id", id);
        jsonObjectLineData.insert("Color", lineColor.name());
        jsonObjectLineData.insert("Width", 7);

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
