#ifndef DEFENSEDATAMANAGER_H
#define DEFENSEDATAMANAGER_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QObject>

struct AircraftInfo
{
    enum Identify{
        F,//green
        K,//yellow
        Z,//orange
        X,//red
        U,//white
        H//red
    };
    //-------------------------------------
    QString TN;
    QString IFFCode;
    QString CallSign;
    QString Type;
    QString MasterRadar;
    Identify Identification;
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
    //---------------------------------------------
public:
    QString identifyToString() const
    {
        QString result = "";
        switch (Identification) {
        case F:
            result = "F";
            break;
        case K:
            result = "K";
            break;
        case Z:
            result = "Z";
            break;
        case X:
            result = "X";
            break;
        case U:
            result = "U";
            break;
        case H:
            result = "H";
            break;
        }
        return result;
    }
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
        //Identification = data.value("Identification").toInt();
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

struct StationInfo
{
    QString Name;
    QString Type;
    QString PrimSec;
    bool Active;

    double Number;
    double Latitude;
    double Longitude;
    double Radius{500000};
    int CycleTime;

    QJsonDocument toJson()
    {
        QJsonObject jsonObject;
        jsonObject.insert("Name", Name);
        jsonObject.insert("Type", Type);
        jsonObject.insert("Primary/Secondary", PrimSec);
        jsonObject.insert("Active", Active);

        jsonObject.insert("Number", Number);
        jsonObject.insert("Latitude", Latitude);
        jsonObject.insert("Longitude", Longitude);
        jsonObject.insert("Radius", Radius);
        jsonObject.insert("CycleTime", CycleTime);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonObject);
        return jsonDoc;
    }

    void fromJson(QJsonDocument jsonDoc)
    {
        QJsonObject data = jsonDoc.object();
        Name = data.value("Name").toString();
        Type = data.value("Type").toString();
        PrimSec = data.value("Primary/Secondary").toString();
        Active = data.value("Active").toBool();

        Number = data.value("Number").toDouble();
        Latitude = data.value("Latitude").toDouble();
        Longitude = data.value("Longitude").toDouble();
        Radius = data.value("Radius").toDouble();
        CycleTime = data.value("CycleTime").toInt();

    }

};

struct SystemInfo
{
    QString Terminal;
    QString Name;
    double Number;
    QString Type;
    double Latitude;
    double Longitude;
    double Altitude;
    double ViewRange{300000};
    double MezRange{250000};
    //status info
    QString ReceiveTime;
    QString Simulation;
    QString BCCStatus;
    QString RadarSearchStatus;
    QString Operational;
    QString MissileCount;
    QString RadarMode;
    //combat info
    double TN;
    QString Acceptance;
    QString Phase;
    double Antenna;
    QString ChanelNo;
    QString Inrange;
    bool Active;

    QJsonDocument toJson()
    {
        QJsonObject jsonObject;
        jsonObject.insert("Terminal", Terminal);
        jsonObject.insert("Name", Name);
        jsonObject.insert("Number", Number);
        jsonObject.insert("Type", Type);
        jsonObject.insert("Latitude", Latitude);
        jsonObject.insert("Longitude", Longitude);
        jsonObject.insert("Altitude", Altitude);
        jsonObject.insert("ViewRange", ViewRange);
        jsonObject.insert("MezRange", MezRange);

        jsonObject.insert("ReceiveTime", ReceiveTime);
        jsonObject.insert("Simulation", Simulation);
        jsonObject.insert("BCCStatus", BCCStatus);
        jsonObject.insert("RadarSearchStatus", RadarSearchStatus);
        jsonObject.insert("Operational", Operational);
        jsonObject.insert("MissileCount", MissileCount);
        jsonObject.insert("RadarMode", RadarMode);

        jsonObject.insert("TN", TN);
        jsonObject.insert("Acceptance", Acceptance);
        jsonObject.insert("Phase", Phase);
        jsonObject.insert("Antenna", Antenna);
        jsonObject.insert("ChanelNo", ChanelNo);
        jsonObject.insert("Inrange", Inrange);
        jsonObject.insert("Active", Active);

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonObject);
        return jsonDoc;
    }

    void fromJson(QJsonDocument jsonDoc)
    {
        QJsonObject data = jsonDoc.object();
        Terminal = data.value("Terminal").toString();
        Name = data.value("Name").toString();
        Number = data.value("Number").toDouble();
        Type = data.value("Type").toString();
        Latitude = data.value("Latitude").toDouble();
        Longitude = data.value("Longitude").toDouble();
        Altitude = data.value("Altitude").toDouble();
        ViewRange = data.value("ViewRange").toDouble();
        MezRange = data.value("MezRange").toDouble();

        ReceiveTime = data.value("ReceiveTime").toString();
        Simulation = data.value("Simulation").toString();
        BCCStatus = data.value("BCCStatus").toString();
        RadarSearchStatus = data.value("RadarSearchStatus").toString();
        Operational = data.value("Operational").toString();
        MissileCount = data.value("MissileCount").toString();
        RadarMode = data.value("RadarMode").toString();
        TN = data.value("TN").toDouble();
        Acceptance = data.value("Acceptance").toString();
        Phase = data.value("Phase").toString();
        Antenna = data.value("Antenna").toDouble();
        ChanelNo = data.value("ChanelNo").toString();
        Inrange = data.value("Inrange").toString();
        Active = data.value("Active").toBool();
    }

};


class DefenseDataManager: public QObject
{
    Q_OBJECT
public:
    DefenseDataManager(QObject* parent = nullptr);
signals:
    void aircraftInfoChanged(AircraftInfo& aircraftInfo);
    void systemInfoChanged(SystemInfo& systemInfo);
    void stationInfoChanged(StationInfo& stationInfo);
    void clearAircraft(QString tn);
public slots:
};

class Demo
{
public:
    Demo(DefenseDataManager* defenseDataManager);
    AircraftInfo createAircraftInfo();
    void updateAircraftInfo();
    void createStationInfo();
    void createSystemInfo();
public:
    DefenseDataManager* mDefenseDataManager;
    QList<AircraftInfo> mAircraftList;
    QList<StationInfo> stationList;
    QList<SystemInfo> systemList;
};

#endif // DEFENSEDATAMANAGER_H
