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
    mUrlList.append(Url{"../data/models/airplane/airplane.png", "../data/models/airplane/airplane.osgb"});
    mUrlList.append(Url{"../data/models/hellicopter/hellicopter.png", "../data/models/hellicopter/hellicopter.osgb"});
    mUrlList.append(Url{"../data/models/drone/drone.png", "../data/models/drone/drone.osgb"});
    mUrlList.append(Url{"../data/models/fighter/fighter.png", "../data/models/fighter/fighter.osgb"});
    mUrlList.append(Url{"../data/models/missile/missile.png", "../data/models/missile/missile.osgb"});

    mColorList.append(QColor("white"));
    mColorList.append(QColor("green"));
    mColorList.append(QColor("yellow"));
    mColorList.append(QColor("orange"));
    mColorList.append(QColor("red"));

    QObject::connect(mNetworkManager, &NetworkManager::dataQueueDeclared, [this]{
        //--create and update aircraft info------------------------
        QTimer *timerUpdateAircraft = new QTimer();
        QObject::connect(timerUpdateAircraft, &QTimer::timeout, [this](){
            createFlyableInfo();
            updateFlyableInfo();
            for(auto& flybleData: mFlyableDataList){
                mNetworkManager->sendData(flybleData.flyableDoc.toJson(QJsonDocument::Compact));
                mNetworkManager->sendData(flybleData.statusDoc.toJson(QJsonDocument::Compact));
                mNetworkManager->sendData(flybleData.lineDoc.toJson(QJsonDocument::Compact));
            }
        });
        timerUpdateAircraft->start(3000);

        QTimer *timerRemoveAircraft = new QTimer();
        QObject::connect(timerRemoveAircraft, &QTimer::timeout, [this](){
            if (mFlyableDataList.size() > 0){
                QJsonObject jsonObject = mFlyableDataList[0].flyableDoc.object();
                QJsonObject jsonStatusObject = mFlyableDataList[0].statusDoc.object();
                QJsonObject jsonLineObject = mFlyableDataList[0].lineDoc.object();

                jsonObject.remove("COMMAND");
                jsonObject.insert("COMMAND", "REMOVE");
                jsonStatusObject.remove("COMMAND");
                jsonStatusObject.insert("COMMAND", "REMOVE");
                jsonLineObject.remove("COMMAND");
                jsonLineObject.insert("COMMAND", "REMOVE");

                mFlyableDataList.pop_front();

                QJsonDocument jsonDoc(jsonObject);
                QJsonDocument jsonStatusDoc(jsonStatusObject);
                QJsonDocument jsonLineDoc(jsonLineObject);

                // mNetworkManager->sendData(jsonDoc.toJson(QJsonDocument::Compact));
                // mNetworkManager->sendData(jsonStatusDoc.toJson(QJsonDocument::Compact));
                // mNetworkManager->sendData(jsonLineDoc.toJson(QJsonDocument::Compact));
            }
        });
        timerRemoveAircraft->start(7000);
    });
}

void FlyableNodeTest::createFlyableInfo()
{
    if(mCount >= mMaxFlyableNumber)
        return;
    //---------------------------------------------------------
    QString name = "Flyable" + QString::number(mCount);
    int id = 10000 + mCount++;
    double longitude = 48 + (QRandomGenerator::global()->generate() % (59 - 48));
    double latitude = 27 + (QRandomGenerator::global()->generate() % (38 - 27));
    double altitude =0;/* (2000 + (QRandomGenerator::global()->generate() % (9000 - 2000)));*/
    double heading = (0 + (QRandomGenerator::global()->generate() % 361));
    double speed = (100 + (QRandomGenerator::global()->generate() % (300-100)));
    int urlIndex = (0 + (QRandomGenerator::global()->generate() % (mUrlList.length())));
//    int colorIndex = (0 + (QRandomGenerator::global()->generate() % (mColorList.length())));
    //--------------------------------------------------------
    QJsonDocument jsonDocument;
    QJsonObject jsonObject;

    jsonObject.insert("Type", "Flyable");
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

    jsonData.insert("Color", mColorList[urlIndex].name());
    jsonData.insert("Url2d", mUrlList[urlIndex].Url2d);
    jsonData.insert("Url3d", mUrlList[urlIndex].Url3d);
    jsonData.insert("ImgSrc", "qrc:/Resources/airplane1.jpg");

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
    jsonObjectStatus.insert("COMMAND", "ADD");

    QJsonObject jsonObjectStatusData;
    jsonObjectStatusData.insert("Name", name);
    jsonObjectStatusData.insert("Id", id);
    jsonObjectStatusData.insert("Longitude", longitude);
    jsonObjectStatusData.insert("Latitude", latitude);
    jsonObjectStatusData.insert("Altitude", altitude);
    jsonObjectStatusData.insert("LayerId", 106);

    QJsonObject jsonObjectStatusFieldData;
    jsonObjectStatusFieldData.insert("Heading", heading);
    jsonObjectStatusFieldData.insert("Speed", speed);
    jsonObjectStatusData.insert("FieldData", jsonObjectStatusFieldData);

    jsonObjectStatus.insert("Data", jsonObjectStatusData);
    jsonDocStatus.setObject(jsonObjectStatus);
    flaybleData.statusDoc = jsonDocStatus;

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
    jsonObjectLineData.insert("LayerId", 105);

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

//        rn = (0 + (QRandomGenerator::global()->generate() % 2));
        //        if(rn < 1)
        //            altitude += altitude - 5;
        //        else
        //            altitude += altitude + 5;
        altitude = 2000;
        int urlIndex = (0 + (QRandomGenerator::global()->generate() % (mUrlList.length())));
//        int colorIndex = (0 + (QRandomGenerator::global()->generate() % (mColorList.length())));
        //--------------------------------------------------------
        if(rn < 1000){
            dataObject["Url2d"] = mUrlList[urlIndex].Url2d;
            dataObject["Url3d"] = mUrlList[urlIndex].Url3d;
            dataObject.insert("Color", mColorList[urlIndex].name());
        }
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

//        dataObject["Longitude"] = longitude;
//        dataObject["Latitude"] = latitude;
//        dataObject["Altitude"] = altitude;
//        dataObject["Heading"] = heading;
//        dataObject["Speed"] =  speed;

        QJsonObject jsonObject;
        jsonObject.insert("Type", "Flyable");
        jsonObject.insert("COMMAND", "UPDATE");
        jsonObject.insert("Data", dataObject);
        flaybleData.flyableDoc.setObject(jsonObject);
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
        jsonObjectStatusData.insert("LayerId", 106);

        QJsonObject jsonObjectStatusFieldData;
        jsonObjectStatusFieldData.insert("Heading", heading);
        jsonObjectStatusFieldData.insert("Speed", speed);
        jsonObjectStatusData.insert("FieldData", jsonObjectStatusFieldData);

        jsonObjectStatus.insert("Data", jsonObjectStatusData);
        jsonDocStatus.setObject(jsonObjectStatus);
        flaybleData.statusDoc = jsonDocStatus;

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

        QJsonArray points = flaybleData.lineDoc.object().value("Data").toObject().value("Points").toArray();
        QJsonObject point;
        point.insert("Longitude", longitude);
        point.insert("Latitude", latitude);
        point.insert("Altitude", altitude);
        points.push_back(point);
        if (points.size() > 100)
            points.removeFirst();
        jsonObjectLineData.insert("Points", points);
        jsonObjectLineData.insert("LayerId", 105);

        jsonObjectLine.insert("Data", jsonObjectLineData);
        jsonDocLine.setObject(jsonObjectLine);
        flaybleData.lineDoc = jsonDocLine;
    }
}

void FlyableNodeTest::removeFlyableInfo()
{

}
