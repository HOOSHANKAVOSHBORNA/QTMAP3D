#ifndef SYSTEMTABLEMODEL_H
#define SYSTEMTABLEMODEL_H

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <deque>
#include <QSharedPointer>

struct SystemInfo
{
    QString Terminal;
    QString Name;
    double Number;
    QString Type;
    double Latitude;
    double Longitude;
    double Altitude;
    double ViewRange{5000};
    double MezRange{4000};
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

class SystemTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SystemTableModel(QObject *parent = nullptr);
    enum CustomRoles {
        BackColorRole = Qt::UserRole + 100,
        TextColorRole = Qt::UserRole + 101,
        HeaderTextRole = Qt::UserRole + 102
    };

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE QString headerText(int column) const;
    Q_INVOKABLE QString getName(int row) const;

public slots:
    void setFilterWildcard(const QString& wildcard);

public:
    void updateItemData(const QString& jsonStr);
    void updateItemData(const SystemInfo& systemInfo);

private:
    std::deque<QPair<int, QSharedPointer<SystemInfo>>> mSystemInfoList;
    std::deque<QPair<int, QSharedPointer<SystemInfo>>> mSystemInfoListProxy;

    QString mFilter;

};

#endif // SYSTEMTABLEMODEL_H
