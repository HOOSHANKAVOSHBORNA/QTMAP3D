#ifndef DEFENSEDATAMANAGER_H
#define DEFENSEDATAMANAGER_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QObject>

struct AircraftInfo
{
    QString TN;
    QString IFFCode;
    QString CallSign;
    QString Type;
    QString MasterRadar;
    QString Identification;
    QString IdentificationMethod;
    QString Time;
    QString Pos;

    double Latitude;
    double Longitude;
    double Altitude;
    double Heading;
    double Speed;

    QStringList DetectionSystems;
    QStringList Sends;

    QString detectionSystemsToString()
    {
        QString result = "";
        for(auto detectSystem: DetectionSystems)
            result += detectSystem + ", ";
        return result;
    }
    QString sendsToString()
    {
        QString result = "";
        for(auto send: Sends)
            result += send + ", ";
        return result;
    }

    QJsonDocument toJson()
    {
        QJsonObject jsonObject;
        jsonObject.insert("TN", TN);
        jsonObject.insert("IFFCode", IFFCode);
        jsonObject.insert("CallSign", CallSign);
        jsonObject.insert("Type", Type);
        jsonObject.insert("MasterRadar", MasterRadar);
        jsonObject.insert("Identification", Identification);// F, K, Z, X, U, H
        jsonObject.insert("IdentificationMethod", IdentificationMethod);//3 char
        jsonObject.insert("Time", Time);//epoch
        jsonObject.insert("Pos", Pos);

        jsonObject.insert("Latitude", Latitude);
        jsonObject.insert("Longitude", Longitude);
        jsonObject.insert("Altitude", Altitude);//meter
        jsonObject.insert("Heading", Heading);
        jsonObject.insert("Speed", Speed);//m/s
        //
        QJsonArray detectSystems;
        for(auto detectSystem: DetectionSystems)
            detectSystems.push_back(detectSystem);
        jsonObject.insert("DetectionSystem", detectSystems);
        //
        QJsonArray sends;
        for(auto send: Sends)
            sends.push_back(send);
        jsonObject.insert("Send", sends);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonObject);
        return jsonDoc;
    }

    void fromJson(QJsonDocument jsonDoc)
    {
        QJsonObject data = jsonDoc.object();
        TN = QString::number(data.value("TN").toInt());
        IFFCode = data.value("IFFCode").toString();
        CallSign = data.value("CallSign").toString();
        Type = data.value("Type").toString();
        MasterRadar = data.value("MasterRadar").toString();
        Identification = data.value("Identification").toString();
        IdentificationMethod = data.value("IdentificationMethod").toString();
        Time = data.value("Time").toString();
        Pos = data.value("Pos").toString();

        Latitude = data.value("Latitude").toDouble();
        Longitude = data.value("Longitude").toDouble();
        Altitude = data.value("Altitude").toDouble();
        Heading = data.value("Heading").toDouble();
        Speed = data.value("Speed").toDouble();

        QJsonArray detectSystems = data.value("DetectionSystem").toArray();
        for(auto detectSystem: detectSystems)
            DetectionSystems.append(detectSystem.toString());

        QJsonArray sends = data.value("Send").toArray();
        for(auto send: sends)
            Sends.append(send.toString());
    }
};

class DefenseDataManager: public QObject
{
    Q_OBJECT
public:
    DefenseDataManager(QObject* parent = nullptr);
signals:
    void aircraftInfoChanged(AircraftInfo& aircraftInfo);
public slots:
};

class Demo
{
public:
    Demo(DefenseDataManager* defenseDataManager);
    AircraftInfo createAircraftInfo();
    void updateAircraftInfo();
public:
    DefenseDataManager* mDefenseDataManager;
    QList<AircraftInfo> mAircraftList;
};

#endif // DEFENSEDATAMANAGER_H
