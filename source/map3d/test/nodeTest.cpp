#include "nodeTest.h"
#include <QJsonObject>
#include <QObject>
#include <QColor>
#include <QRandomGenerator>
#include <QTimer>
#include <QJsonArray>

NodeTest::NodeTest(ServiceManager *serviceManager):
    mServiceManager(serviceManager)
{
    //--create and update aircraft info------------------------
        QTimer *timerUpdate = new QTimer();
        QObject::connect(timerUpdate, &QTimer::timeout, [this](){
            createInfo();
//            updateInfo();
            for(auto& nodeInfo: mNodeInfoList){
                mServiceManager->sendNode(nodeInfo.nodeData);
                mServiceManager->sendStatusNode(nodeInfo.statusNodeData);
                mServiceManager->sendCircle(nodeInfo.circleData);
                mServiceManager->sendPolygon(nodeInfo.polygonData);
            }
        });
        timerUpdate->start(2000);

        QTimer *timerRemove = new QTimer();
        QObject::connect(timerRemove, &QTimer::timeout, [this](){
            if (mNodeInfoList.size() > 0){
                // QJsonObject jsonObject = mNodeDataList[0].nodeDoc.object();
                // QJsonObject jsonStatusObject = mNodeDataList[0].statusDoc.object();
                // QJsonObject jsonCircleObject = mNodeDataList[0].circleDoc.object();
                // QJsonObject jsonPolygonObject = mNodeDataList[0].polygonDoc.object();

                // jsonObject.remove("COMMAND");
                // jsonObject.insert("COMMAND", "REMOVE");
                // jsonStatusObject.remove("COMMAND");
                // jsonStatusObject.insert("COMMAND", "REMOVE");
                // jsonCircleObject.remove("COMMAND");
                // jsonCircleObject.insert("COMMAND", "REMOVE");
                // jsonPolygonObject.remove("COMMAND");
                // jsonPolygonObject.insert("COMMAND", "REMOVE");

                // mNodeDataList.pop_front();

                // QJsonDocument jsonDoc(jsonObject);
                // QJsonDocument jsonStatusDoc(jsonStatusObject);
                // QJsonDocument jsonCircleDoc(jsonCircleObject);
                // QJsonDocument jsonPolygonDoc(jsonPolygonObject);

                // mNetworkManager->sendData(jsonDoc.toJson(QJsonDocument::Compact));
                // mNetworkManager->sendData(jsonStatusDoc.toJson(QJsonDocument::Compact));
                // mNetworkManager->sendData(jsonCircleDoc.toJson(QJsonDocument::Compact));
                // mNetworkManager->sendData(jsonPolygonDoc.toJson(QJsonDocument::Compact));
            }
        });
        timerRemove->start(7000);
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
    NodeInfo nodInfo;
    NodeData nodData;
    nodData.id = id;
    nodData.name = name;
    nodData.type = NodeType::Fixed;
    nodData.command = Command::Add;
    nodData.iconInfoUrl = "../data/models/station/station.png";
    nodData.imgInfoUrl = "qrc:/Resources/system.jpg";
    nodData.url2D = "../data/models/station/station.png";
    nodData.url3D = "../data/models/station/station.osgb";
    nodData.color = color.name();
    nodData.isAttacker = false;
    nodData.latitude = latitude;
    nodData.longitude = longitude;
    nodData.altitude = altitude;

    double rand = (QRandomGenerator::global()->generate() % (2));
    if(rand < 1)
        nodData.layersId.push_back(302);
    else
        nodData.layersId.push_back(303);

    NodeFieldData nameField;
    nameField.name = "Name";
    nameField.value = name;
    nameField.category = "Main Information";
    nodData.fieldData.push_back(nameField);

    NodeFieldData idField;
    idField.name = "Id";
    idField.value = id;
    idField.category = "Main Information";
    nodData.fieldData.push_back(idField);

    NodeFieldData colorField;
    colorField.name = "Color";
    colorField.value = color.name();
    colorField.category = "Main Information";
    nodData.fieldData.push_back(colorField);

    NodeFieldData latField;
    latField.name = "Latitude";
    latField.value = latitude;
    latField.category = "Location Information";
    nodData.fieldData.push_back(latField);

    NodeFieldData longField;
    longField.name = "Longitude";
    longField.value = longitude;
    longField.category = "Location Information";
    nodData.fieldData.push_back(longField);

    NodeFieldData altField;
    altField.name = "Altitude";
    altField.value = altitude;
    altField.category = "Location Information";
    nodData.fieldData.push_back(altField);

    nodInfo.nodeData = nodData;
    //--status node-----------------------------------------------
    StatusNodeData statusNodeData;
    statusNodeData.id = id;
    statusNodeData.name = "Status" + QString::number(mCount);
    statusNodeData.latitude = latitude;
    statusNodeData.longitude = longitude;
    statusNodeData.altitude = altitude;
    statusNodeData.command = Command::Add;
    statusNodeData.layerId = 305;

    NodeFieldData info1Field;
    info1Field.name = "Info1";
    info1Field.value = 200;
    info1Field.category = "";
    statusNodeData.fieldData.push_back(info1Field);

    NodeFieldData info2Field;
    info2Field.name = "Info2";
    info2Field.value = "StatusInfo";
    info2Field.category = "";
    statusNodeData.fieldData.push_back(info2Field);
    statusNodeData.fieldData.push_back(colorField);

    nodInfo.statusNodeData = statusNodeData;

    //--circle node------------------------------------------------
    QColor colorCircle("red");
    colorCircle.setAlpha(100);
    CircleData circleData;
    circleData.id = id;
    circleData.name = "Circle" + QString::number(mCount);
    circleData.radius = 1000 + (QRandomGenerator::global()->generate() % (5000 - 1000));
    circleData.color = colorCircle.name();
    circleData.strokeColor = colorCircle.name();
    circleData.strokeWidth = 0;
    circleData.altitude = altitude;
    circleData.latitude = latitude;
    circleData.longitude = longitude;
    circleData.command = Command::Add;
    circleData.layerId = 304;

    nodInfo.circleData = circleData;
    //--polygon----------------------------------------------------
    QColor colorPolygon("green");
    colorPolygon.setAlpha(50);
    QColor colorStroke("blue");

    PolygonData polygonData;
    polygonData.id = id;
    polygonData.name = "Polygon" + QString::number(mCount);
    polygonData.fillColor = colorPolygon.name(QColor::HexArgb);
    polygonData.color = colorStroke.name(QColor::HexArgb);
    polygonData.width = 7;
    polygonData.command = Command::Add;
    polygonData.layerId = 306;

    double step = 0.01;
    QVector3D point1;
    point1.setX(longitude - step);
    point1.setY(latitude - step);
    point1.setZ(altitude);
    polygonData.points.push_back(point1);

    QVector3D point2;
    point2.setX(longitude + step);
    point2.setY(latitude - step);
    point2.setZ(altitude);
    polygonData.points.push_back(point2);

    QVector3D point3;
    point3.setX(longitude + step);
    point3.setY(latitude + step);
    point3.setZ(altitude);
    polygonData.points.push_back(point3);

    QVector3D point4;
    point4.setX(longitude - step);
    point4.setY(latitude + step);
    point4.setZ(altitude);
    polygonData.points.push_back(point4);

    nodInfo.polygonData = polygonData;

    mNodeInfoList.append(nodInfo);
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
