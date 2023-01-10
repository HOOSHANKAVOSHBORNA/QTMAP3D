#ifndef STATIONINFOMODEL_H
#define STATIONINFOMODEL_H
#include <QAbstractListModel>
#include <QQuickItem>

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
        Longitude = Qt::UserRole + 205,
        Radius = Qt::UserRole + 206,
        CycleTime = Qt::UserRole + 207,
        Active = Qt::UserRole + 208

    };

    StationInfoModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    StationInfo getStationInfo() {return mStationInfo;}
    QHash<int, QByteArray> roleNames() const override;

    void setInformtion(const StationInfo &stationInfo);

Q_SIGNALS:
    void gotoButtonClicked();
    void rangeButtonClicked(bool check);
    void visibleButtonClicked(bool checked);
    void activateButtonClicked(bool checked);
    void activeButtonToggled(bool checked);
    void moreButtonClicked();

public slots:
    void onActiveToggled(bool checked);
private:
    StationInfo mStationInfo;
};

class StationInformtion : public QObject
{
    Q_OBJECT

public:
    StationInformtion(QQmlEngine *qmlEngine, UIHandle *uiHandle, StationInfo stationInfo, QObject *parent = nullptr);
    StationInfoModel *getInfo() {return mInfoModel;}
    void show();
private:
    StationInfo mInformation;
    StationInfoModel *mInfoModel;
    UIHandle *mUiHandle;
    QQuickItem *mItem;
};

#endif
