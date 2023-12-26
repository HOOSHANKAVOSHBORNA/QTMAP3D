#include "nodeTest.h"
#include <QJsonObject>
#include <QObject>
#include <QColor>
#include <QRandomGenerator>
#include <QTimer>
#include <QJsonArray>

NodeTest::NodeTest(NetworkManager *networkManager):
    mNetworkManager(networkManager)
{
    QObject::connect(mNetworkManager, &NetworkManager::dataQueueDeclared, [this]{
    //--create and update aircraft info------------------------
        QTimer *timerUpdateAircraft = new QTimer();
        QObject::connect(timerUpdateAircraft, &QTimer::timeout, [this](){
            createInfo();
//            updateInfo();
            for(auto& nodeData: mNodeDataList){
                mNetworkManager->sendData(nodeData.nodeDoc.toJson(QJsonDocument::Compact));
                mNetworkManager->sendData(nodeData.statusDoc.toJson(QJsonDocument::Compact));
                mNetworkManager->sendData(nodeData.circleDoc.toJson(QJsonDocument::Compact));
                mNetworkManager->sendData(nodeData.polygonDoc.toJson(QJsonDocument::Compact));
            }
        });
        timerUpdateAircraft->start(2000);

        QTimer *timerRemoveAircraft = new QTimer();
        QObject::connect(timerRemoveAircraft, &QTimer::timeout, [this](){
            if (mNodeDataList.size() > 0){
                QJsonObject jsonObject = mNodeDataList[0].nodeDoc.object();
                QJsonObject jsonStatusObject = mNodeDataList[0].statusDoc.object();
                QJsonObject jsonCircleObject = mNodeDataList[0].circleDoc.object();
                QJsonObject jsonPolygonObject = mNodeDataList[0].polygonDoc.object();

                jsonObject.remove("COMMAND");
                jsonObject.insert("COMMAND", "REMOVE");
                jsonStatusObject.remove("COMMAND");
                jsonStatusObject.insert("COMMAND", "REMOVE");
                jsonCircleObject.remove("COMMAND");
                jsonCircleObject.insert("COMMAND", "REMOVE");
                jsonPolygonObject.remove("COMMAND");
                jsonPolygonObject.insert("COMMAND", "REMOVE");

                mNodeDataList.pop_front();

                QJsonDocument jsonDoc(jsonObject);
                QJsonDocument jsonStatusDoc(jsonStatusObject);
                QJsonDocument jsonCircleDoc(jsonCircleObject);
                QJsonDocument jsonPolygonDoc(jsonPolygonObject);

                // mNetworkManager->sendData(jsonDoc.toJson(QJsonDocument::Compact));
                // mNetworkManager->sendData(jsonStatusDoc.toJson(QJsonDocument::Compact));
                // mNetworkManager->sendData(jsonCircleDoc.toJson(QJsonDocument::Compact));
                // mNetworkManager->sendData(jsonPolygonDoc.toJson(QJsonDocument::Compact));
            }
        });
        timerRemoveAircraft->start(7000);
    });
}

void NodeTest::createInfo()
{
    if(mCount >= mMaxNumber)
        return;
    //---------------------------------------------------------
    QString name = "Node" + QString::number(mCount);
    int id = 30000 + mCount++;
    QColor color("yellow");
    double longitude = 48 + (QRandomGenerator::global()->generate() % (59 - 48));
    double latitude = 27 + (QRandomGenerator::global()->generate() % (38 - 27));
    double altitude =0;/* (2000 + (QRandomGenerator::global()->generate() % (9000 - 2000)));*/
    //--------------------------------------------------------
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;

    jsonObject.insert("Type", "Node");
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
    jsonData.insert("Url2d", "../data/models/station/station.png");
    jsonData.insert("Url3d", "../data/models/station/station.osgb");
    jsonData.insert("ImgSrc", "qrc:/Resources/system.jpg");

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

    QJsonArray layer;
    double rand = (QRandomGenerator::global()->generate() % (2));
    if(rand < 1)
        layer.push_back(302);
    else
        layer.push_back(303);
    jsonData.insert("LayersId", layer);

    jsonObject.insert("Data", jsonData);
    jsonDocument.setObject(jsonObject);
    NodeData nodeData;
    nodeData.nodeDoc = jsonDocument;

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
    jsonObjectStatusData.insert("LayerId", 305);

    QJsonObject jsonObjectStatusFieldData;
    jsonObjectStatusFieldData.insert("Info1", "info1");
    jsonObjectStatusFieldData.insert("Info2", 200);
    jsonObjectStatusData.insert("FieldData", jsonObjectStatusFieldData);

    jsonObjectStatus.insert("Data", jsonObjectStatusData);
    jsonDocStatus.setObject(jsonObjectStatus);
    nodeData.statusDoc = jsonDocStatus;

    //--circle node------------------------------------------------
    double radius = 1000 + (QRandomGenerator::global()->generate() % (5000 - 1000));
    QColor colorCircle("red");
    colorCircle.setAlpha(100);
    QJsonDocument jsonDocCircle;
    QJsonObject jsonObjectCircle;

    jsonObjectCircle.insert("Type", "Circle");
    jsonObjectCircle.insert("COMMAND", "ADD");

    QJsonObject jsonObjectCircleData;
    jsonObjectCircleData.insert("Name", name + " circle");
    jsonObjectCircleData.insert("Id", id);

    jsonObjectCircleData.insert("Longitude", longitude);
    jsonObjectCircleData.insert("Latitude", latitude);
    jsonObjectCircleData.insert("Altitude", altitude);
    jsonObjectCircleData.insert("Radius", radius);
    jsonObjectCircleData.insert("Color", colorCircle.name(QColor::HexArgb));

    jsonObjectCircleData.insert("LayerId", 304);

    jsonObjectCircle.insert("Data", jsonObjectCircleData);
    jsonDocCircle.setObject(jsonObjectCircle);
    nodeData.circleDoc = jsonDocCircle;
    //--polygon----------------------------------------------------
    QColor colorPolygon("green");
    colorPolygon.setAlpha(50);
    QColor colorStroke("blue");
    QJsonDocument jsonDocPolygon;
    QJsonObject jsonObjectPolygon;

    jsonObjectPolygon.insert("Type", "Polygon");
    jsonObjectPolygon.insert("COMMAND", "ADD");

    QJsonObject jsonObjectPolygonData;
    jsonObjectPolygonData.insert("Name", name + " polygon");
    jsonObjectPolygonData.insert("Id", id);
    jsonObjectPolygonData.insert("Width", 7);
    jsonObjectPolygonData.insert("Color", colorStroke.name(QColor::HexArgb));
    jsonObjectPolygonData.insert("FillColor", colorPolygon.name(QColor::HexArgb));

    QJsonArray points;
    double step = 0.01;
    QJsonObject point1;
    point1.insert("Longitude", longitude - step);
    point1.insert("Latitude", latitude - step);
    point1.insert("Altitude", altitude);
    points.push_back(point1);

    QJsonObject point2;
    point2.insert("Longitude", longitude + step);
    point2.insert("Latitude", latitude - step);
    point2.insert("Altitude", altitude);
    points.push_back(point2);

    QJsonObject point3;
    point3.insert("Longitude", longitude + step);
    point3.insert("Latitude", latitude + step);
    point3.insert("Altitude", altitude);
    points.push_back(point3);

    QJsonObject point4;
    point4.insert("Longitude", longitude - step);
    point4.insert("Latitude", latitude + step);
    point4.insert("Altitude", altitude);
    points.push_back(point4);

    jsonObjectPolygonData.insert("Points", points);
    jsonObjectPolygonData.insert("LayerId", 306);

    jsonObjectPolygon.insert("Data", jsonObjectPolygonData);
    jsonDocPolygon.setObject(jsonObjectPolygon);
    nodeData.polygonDoc = jsonDocPolygon;

    mNodeDataList.append(nodeData);
}

void NodeTest::updateInfo()
{
//    for(auto& nodeData: mNodeDataList)
//    {
//        QJsonObject dataObject = nodeData.nodeDoc.object().value("Data").toObject();
//        //------------------------
//        double latitude = dataObject["Latitude"].toDouble();
//        double longitude = dataObject["Longitude"].toDouble();
//        double altitude = dataObject["Altitude"].toDouble();
//        double speed /*= jsonObject["Speed"].toDouble()*/;
//        double heading = dataObject["Heading"].toDouble();

//        int rn = (0 + (QRandomGenerator::global()->generate() % 10000));
//        if(rn < 1)
//            heading = (0 + (QRandomGenerator::global()->generate() % 361));

//        speed = (138 + (QRandomGenerator::global()->generate() % 137));

//        double pi = 3.14159265359;
//        double teta = (90 - heading)* (pi / 180);
//        double step = 10.0/10000.0;
//        longitude += step * std::cos(teta);
//        latitude += step * std::sin(teta);

//        rn = (0 + (QRandomGenerator::global()->generate() % 2));
////        if(rn < 1)
////            altitude += altitude - 5;
////        else
////            altitude += altitude + 5;
//        altitude = 2000;
//        //--------------------------------------------------------
//        dataObject["Longitude"] = longitude;
//        dataObject["Latitude"] = latitude;
//        dataObject["Altitude"] = altitude;
//        dataObject["Heading"] = heading;
//        dataObject["Speed"] =  speed;

//        QJsonObject jsonObject;
//        jsonObject.insert("Type", "Flyable");
//        jsonObject.insert("COMMAND", "UPDATE");
//        jsonObject.insert("Data", dataObject);
//        flaybleData.flyableDoc.setObject(jsonObject);
//        //--status node-----------------------------------------------
//        QJsonDocument jsonDocStatus;
//        QJsonObject jsonObjectStatus;
//        QJsonObject jsonObjectStatusData;

//        int id = dataObject["Id"].toObject().value("value").toInt();
//        QString name = dataObject["Name"].toObject().value("value").toString();

//        jsonObjectStatus.insert("Type", "Status");
//        jsonObjectStatusData.insert("Name", name);
//        jsonObjectStatusData.insert("Id", id);
//        jsonObjectStatusData.insert("Longitude", longitude);
//        jsonObjectStatusData.insert("Latitude", latitude);
//        jsonObjectStatusData.insert("Altitude", altitude);
//        jsonObjectStatusData.insert("Heading", heading);
//        jsonObjectStatusData.insert("Speed", speed);
//        jsonObjectStatusData.insert("LayerId", 106);
//        jsonObjectStatus.insert("Data", jsonObjectStatusData);
//        jsonDocStatus.setObject(jsonObjectStatus);
//        flaybleData.statusDoc = jsonDocStatus;

//        //--route node------------------------------------------------
//        QJsonDocument jsonDocLine;
//        QJsonObject jsonObjectLine;

//        jsonObjectLine.insert("Type", "Route");
//        jsonObjectLine.insert("COMMAND", "UPDATE");

//        QJsonObject jsonObjectLineData;
//        jsonObjectLineData.insert("Name", name);
//        jsonObjectLineData.insert("Id", id);

//        QJsonArray points = dataObject.value("Points").toArray();
//        QJsonObject point;
//        point.insert("Longitude", longitude);
//        point.insert("Latitude", latitude);
//        point.insert("Altitude", altitude);
//        points.push_back(point);

//        jsonObjectLineData.insert("Points", points);
//        jsonObjectLineData.insert("LayerId", 106);

//        jsonObjectLine.insert("Data", jsonObjectLineData);
//        jsonDocLine.setObject(jsonObjectLine);
//        flaybleData.lineDoc = jsonDocLine;
//    }
}
