#ifndef STATIONINFOMODEL_H
#define STATIONINFOMODEL_H
#include <QAbstractListModel>

#include "datamanager.h"

class StationInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MyRoles {
        Name = Qt::UserRole + 200,
        Type = Qt::UserRole + 201,
        PrimSec = Qt::UserRole + 202,
        Numberr = Qt::UserRole + 203,
        Latitude = Qt::UserRole + 204,
        Longitude = Qt::UserRole + 205
    };

    StationInfoModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    StationInfo getStationInfo() {return mStationInfo;}
    QHash<int, QByteArray> roleNames() const override;

Q_SIGNALS:
    void gotoButtonClicked();
    void routeButtonClicked(bool checked);
    void trackButtonClicked(bool checked);
    void moreButtonClicked();

private:
    StationInfo mStationInfo;
};

#endif
