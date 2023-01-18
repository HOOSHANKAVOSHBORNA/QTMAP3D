#include "createdata.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

CreateData::CreateData()
{
    QDateTime cd = QDateTime::currentDateTime();
    qsrand(cd.toTime_t());
}

QJsonDocument CreateData::createAircraftInfo()
{
    int tn = 10000 + aircraftList.count();
    QJsonObject jsonObject;
    jsonObject.insert("TN", tn);
    jsonObject.insert("IFFCode", "a12345");
    jsonObject.insert("CallSign", "cls");
    jsonObject.insert("Type", "type1");
    jsonObject.insert("MasterRadar", "radar1");
    jsonObject.insert("Identification", "F");// F, K, Z, X, U, H
    jsonObject.insert("IdentificationMethod", "mt1");//3 char
    jsonObject.insert("Time", "12345678954213");//epoch
    jsonObject.insert("Pos", "pos");
    //
    //    int latitude = ((qrand() % 360) - 180);
    double latitude = 35 + (qrand() % (75 - 35));
    //    int longitude = ((qrand() % 180) - 90);
    double longitude = 25 + (qrand() % (43 - 25));

    double altitude = (2000 + (qrand() % (9000 - 2000)));

    jsonObject.insert("Latitude", latitude);
    jsonObject.insert("Longitude", longitude);
    jsonObject.insert("Altitude", altitude);//meter
    jsonObject.insert("Heading", 30);
    jsonObject.insert("Speed", 150);//m/s
    //
    QJsonArray detectSystems;
    detectSystems.push_back("system1");
    detectSystems.push_back("system2");
    jsonObject.insert("DetectionSystem", detectSystems);
    //
    QJsonArray sends;
    sends.push_back("send1");
    sends.push_back("send2");
    sends.push_back("send3");
    jsonObject.insert("Send", sends);

    QJsonObject jsonMain;
    jsonMain.insert("Name", "Aircraft");
    jsonMain.insert("Data", jsonObject);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonMain);

    aircraftList.append(jsonDoc);
    return jsonDoc;
}

void CreateData::updateAircraftInfo()
{
    for(int i = 0; i < aircraftList.count(); ++i)
    {
        QJsonObject mainObject = aircraftList[i].object();
        QJsonObject data = mainObject.value("Data").toObject();
        //------------------------
        double latitude = data.value("Latitude").toDouble();
        double longitude = data.value("Longitude").toDouble();
        double altitude = data.value("Altitude").toDouble();
        double speed = data.value("Speed").toDouble();
        double heading = data.value("Heading").toDouble();

        //    double latitudeDiff = latitude;
        //    double longitudeDiff = longitude;
        //    double altitudeDiff = altitude;

        int randomX = (100 + (qrand() % 19));
        int randomY = (100 + (qrand() % 19));

        //altitude = (2000 + (qrand() % (9000 - 2000)));

        speed = (138 + (qrand() % 137));
        heading = (0 + (qrand() % 361));
        int val = qrand() % 4;
        if(val == 1)
        {
            latitude += randomX/10000.0;
            longitude += randomY/10000.0;
        }
        else if(val == 2)
        {
            latitude += randomX/10000.0;
            longitude -= randomY/10000.0;
        }
        else if(val == 3)
        {
            latitude -= randomX/10000.0;
            longitude += randomY/10000.0;
        }
        else
        {
            latitude -= randomX/10000.0;
            longitude -= randomY/10000.0;
        }
        //calculat heading----------------------
        //    latitudeDiff = latitude - latitudeDiff;
        //    longitudeDiff = longitude - longitudeDiff;
        //    altitudeDiff = altitude - altitudeDiff;
        //    //north =(0, 1, 0)
        //    heading = acos(longitudeDiff/sqrt(latitudeDiff * latitudeDiff + longitudeDiff * longitudeDiff + altitudeDiff * altitudeDiff));
        //    heading *= (180.0/3.141592653589793238463);
        data["Latitude"] = latitude;
        data["Longitude"] = longitude;
        data["Altitude"] = altitude;
        data["Speed"] = speed;
        data["Heading"] = heading;
        //------------------------
        mainObject["Data"] = data;
        aircraftList[i].setObject(mainObject);

        //        qDebug()<<heading;
    }
}

void CreateData::createStationInfo()
{
    for(int i = 0; i < 10; ++i)
    {
        QString name = "Station" + QString::number(stationList.count());
        double latitude = 48 + (qrand() % (59 - 48));
        double longitude = 27 + (qrand() % (38 - 27));
        double radius = (200000 + (qrand() % (500000 - 200000)));

        QJsonObject jsonObject;
        jsonObject.insert("Name", name);
        jsonObject.insert("Active", true);
        jsonObject.insert("Number", 123456789);
        jsonObject.insert("Type", "Type1");
        jsonObject.insert("Latitude", latitude);
        jsonObject.insert("Longitude", longitude);
        jsonObject.insert("Primary/Secondary", "secondary");
        jsonObject.insert("Radius", radius);//meter
        jsonObject.insert("CycleTime", 123123456);//

        QJsonObject jsonMain;
        jsonMain.insert("Name", "Station");
        jsonMain.insert("Data", jsonObject);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonMain);

        stationList.append(jsonDoc);
    }

}

void CreateData::createSystemInfo()
{
    for(int i = 0; i < 10; ++i)
    {
        QString name = "System" + QString::number(systemList.count());
        double latitude = 48 + (qrand() % (59 - 48));
        double longitude = 27 + (qrand() % (38 - 27));
        double viewRange = (100000 + (qrand() % (300000 - 100000)));
        double mezRange = viewRange - 10000;

        QJsonObject jsonObject;
        jsonObject.insert("Terminal", "terminal1");
        jsonObject.insert("Name", name);
        jsonObject.insert("Number", 123456);
        jsonObject.insert("Type", "Type1");
        jsonObject.insert("Active", true);
        //
        jsonObject.insert("Latitude", latitude);
        jsonObject.insert("Longitude", longitude);
        jsonObject.insert("Altitude", 4000);//meter
        jsonObject.insert("ViewRange", viewRange);//meter
        jsonObject.insert("MezRange", mezRange);//meter
        //status info
        jsonObject.insert("ReceiveTime", "1392/12/01: 12:10");
        jsonObject.insert("Simulation", "simulation");
        jsonObject.insert("BCCStatus", "s");//s, us
        jsonObject.insert("RadarSearch Status", "us");//s, us
        jsonObject.insert("Operational", "operational");
        jsonObject.insert("MissileCount", 5);
        jsonObject.insert("RadarMode", "rMode");
        //combat info
        jsonObject.insert("TN", 10000);
        jsonObject.insert("Acceptance", "acceptance1");
        jsonObject.insert("Phase", "search");//search, lock, ...
        jsonObject.insert("Antenna", 50);//degree (lock sight)
        jsonObject.insert("ChanelNo", "123014s");
        jsonObject.insert("Inrange", "inrange");

        QJsonObject jsonMain;
        jsonMain.insert("Name", "System");
        jsonMain.insert("Data", jsonObject);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonMain);

        systemList.append(jsonDoc);
    }
}

QJsonDocument CreateData::createMinfo()
{
    QJsonObject jsonObject;
    jsonObject.insert("MissileNo", "123456");
    jsonObject.insert("MissileStatus", "st");
    jsonObject.insert("TN", 10000);
    jsonObject.insert("Range", 1000);//distance
    jsonObject.insert("Azimuth", 60);//degree
    jsonObject.insert("Elevation", 100);//meter
    jsonObject.insert("Speed", 60);//m/s
    jsonObject.insert("Latitude", 52.1);
    jsonObject.insert("Longitude", 35.1);

    QJsonObject jsonMain;
    jsonMain.insert("Name", "Missile");
    jsonMain.insert("Data", jsonObject);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonMain);

    return jsonDoc;
}
