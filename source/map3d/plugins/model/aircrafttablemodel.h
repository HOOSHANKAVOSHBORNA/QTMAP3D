#ifndef AIRCRAFTTABLEMODEL_H
#define AIRCRAFTTABLEMODEL_H

#include <QAbstractTableModel>
#include <deque>

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

    QString Latitude;
    QString Longitude;
    QString Altitude;
    QString Heading;
    QString Speed;

    QString DetectionSystems;
    QString Sends;
};

class AircraftTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AircraftTableModel(QObject * parent = nullptr);

    enum CustomRoles {
        BackColorRole = Qt::UserRole + 100,
        TextColorRole = Qt::UserRole + 101
    };

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;




public:
    void updateItemData(const QString& jsonStr);
    void updateItemData(const AircraftInfo& aircraftInfo);

private:
    std::deque<AircraftInfo> mAircraftInfoList;
};

#endif // AIRCRAFTTABLEMODEL_H
