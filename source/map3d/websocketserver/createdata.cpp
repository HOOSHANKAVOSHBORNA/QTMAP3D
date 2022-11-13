#include "createdata.h"

#include <QJsonArray>
#include <QJsonObject>

CreateData::CreateData()
{

}

QJsonDocument CreateData::createTargetinfo()
{
    QJsonObject jsonObject;
    jsonObject.insert("TN", 10000);
    jsonObject.insert("IFFCode", "a12345");
    jsonObject.insert("CallSign", "cls");
    jsonObject.insert("Type", "type1");
    jsonObject.insert("Master Radar", "radar1");
    jsonObject.insert("Identification", "F");// F, K, Z, X, U, H
    jsonObject.insert("Identification Method", "mt1");//3 char
    jsonObject.insert("time", "12345678954213");//epoch
    jsonObject.insert("Pos", "pos");
    //
    jsonObject.insert("latitude", 52.1);
    jsonObject.insert("longitude", 35.1);
    jsonObject.insert("Altitude", 4000);//meter
    jsonObject.insert("Heading", 30);
    jsonObject.insert("Speed", 300);//m/s
    //
    QJsonArray detectSystems;
    detectSystems.push_back("system1");
    detectSystems.push_back("system2");
    jsonObject.insert("Detection System", detectSystems);
    //
    QJsonArray sends;
    sends.push_back("send1");
    sends.push_back("send2");
    sends.push_back("send3");
    jsonObject.insert("Detection System", sends);

    QJsonObject jsonMain;
    jsonMain.insert("Name", "Target");
    jsonMain.insert("Data", jsonObject);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonMain);

    return jsonDoc;
}

QJsonDocument CreateData::createRinfo()
{
    QJsonObject jsonObject;
    jsonObject.insert("Name", "rName1");
    jsonObject.insert("Number", 123456789);
    jsonObject.insert("type", "rType1");
    jsonObject.insert("latitude", 52.1);
    jsonObject.insert("longitude", 35.1);
    jsonObject.insert("primary/secondary", "secondary");

    QJsonObject jsonMain;
    jsonMain.insert("Name", "Radar");
    jsonMain.insert("Data", jsonObject);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonMain);

    return jsonDoc;
}

QJsonDocument CreateData::createSinfo()
{
    QJsonObject jsonObject;
    jsonObject.insert("Terminal", "terminal1");
    jsonObject.insert("Name", "sName1");
    jsonObject.insert("Number", 123456);
    jsonObject.insert("Type", "sType1");
    jsonObject.insert("latitude", 52.1);
    jsonObject.insert("longitude", 35.1);
    jsonObject.insert("Altitude", 4000);//meter
    jsonObject.insert("View Range", 100);//meter
    jsonObject.insert("Mez Range", 50);//meter
    //status info
    jsonObject.insert("Receive Time", "1392/12/01: 12:10");
    jsonObject.insert("Simulation", "simulation");
    jsonObject.insert("BCC Status", "s");//s, us
    jsonObject.insert("Radar Search Status", "us");//s, us
    jsonObject.insert("Operational", "operational");
    jsonObject.insert("Missile Count", 5);
    jsonObject.insert("Radar Mode", "rMode");
    //combat info
    jsonObject.insert("TN", 10000);
    jsonObject.insert("acceptance", "acceptance1");
    jsonObject.insert("Phase", "search");//search, lock, ...
    jsonObject.insert("Antenna", 50);//degree (lock sight)
    jsonObject.insert("Chanel No", "123014s");
    jsonObject.insert("Inrange", "inrange");

    QJsonObject jsonMain;
    jsonMain.insert("Name", "Samaneh");
    jsonMain.insert("Data", jsonObject);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonMain);

    return jsonDoc;
}

QJsonDocument CreateData::createMinfo()
{
    QJsonObject jsonObject;
    jsonObject.insert("Missile No", "123456");
    jsonObject.insert("Missile Status", "st");
    jsonObject.insert("TN", 10000);
    jsonObject.insert("Range", 1000);//distance
    jsonObject.insert("Azimuth", 60);//degree
    jsonObject.insert("Elevation", 100);//meter
    jsonObject.insert("Speed", 60);//m/s
    jsonObject.insert("latitude", 52.1);
    jsonObject.insert("longitude", 35.1);

    QJsonObject jsonMain;
    jsonMain.insert("Name", "Missile");
    jsonMain.insert("Data", jsonObject);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonMain);

    return jsonDoc;
}
