#ifndef INFOMODEL_H
#define INFOMODEL_H
#include <QAbstractListModel>
#include "datamanager.h"

class InfoModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum MyRoles {
        TN = Qt::UserRole + 200,
        IFFCode = Qt::UserRole + 201,
        CallSign = Qt::UserRole + 202,
        Type = Qt::UserRole + 203,
        Master = Qt::UserRole + 204,
        Identification = Qt::UserRole + 205,
        IdentificationMethod = Qt::UserRole + 206,
        Time = Qt::UserRole + 207,
        Pos = Qt::UserRole + 208,

        Latitude = Qt::UserRole + 210,
        Longitude = Qt::UserRole + 211,
        Altitude = Qt::UserRole + 212,
        Heading = Qt::UserRole + 213,
        Speed = Qt::UserRole + 214
    };

    InfoModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    AircraftInfo getAircraftInfo() {return aircraftInfo;}
    void setAircraftInfo(AircraftInfo &a);
    QHash<int, QByteArray> roleNames() const;

Q_SIGNALS:
    void view2D3DButtonClicked();
    void routeButtonClicked();
    void followButtonClicked();
    void moreButtonClicked();

private:
    AircraftInfo aircraftInfo;

};

#endif // INFOMODEL_H
