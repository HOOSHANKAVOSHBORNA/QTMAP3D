#ifndef STATIONTABLEMODEL_H
#define STATIONTABLEMODEL_H

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <deque>
#include <QSharedPointer>


struct StationInfo
{
    QString Name;
    QString Type;
    QString PrimSec;

    double Number;
    double Latitude;
    double Longitude;
    double Radius;
    int CycleTime;

    QJsonDocument toJson()
    {
        QJsonObject jsonObject;
        jsonObject.insert("Name", Name);
        jsonObject.insert("Type", Type);
        jsonObject.insert("Primary/Secondary", PrimSec);

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

        Number = data.value("Number").toDouble();
        Latitude = data.value("Latitude").toDouble();
        Longitude = data.value("Longitude").toDouble();
        Radius = data.value("Radius").toDouble();
        CycleTime = data.value("CycleTime").toInt();

    }

};

class StationTableModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    StationTableModel(QObject *parent = nullptr);

    enum CustomRoles {
        BackColorRole = Qt::UserRole + 100,
        TextColorRole = Qt::UserRole + 101,
        HeaderTextRole = Qt::UserRole + 102
    };


public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE QString headerText(int column) const;
    Q_INVOKABLE QString getName(int row) const;

public slots:
    void setFilterWildcard(const QString& wildcard);

public:
    void updateItemData(const QString& jsonStr);
    void updateItemData(const StationInfo& stationInfo);

private:
    std::deque<QPair<int, QSharedPointer<StationInfo>>> mStationInfoList;
    std::deque<QPair<int, QSharedPointer<StationInfo>>> mStationInfoListProxy;

    QString mFilter;

};

#endif // STATIONTABLEMODEL_H
