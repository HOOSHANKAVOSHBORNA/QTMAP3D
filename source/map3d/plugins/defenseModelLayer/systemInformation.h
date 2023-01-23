#ifndef SYSTEMINFORMATION_H
#define SYSTEMINFORMATION_H
#include <QAbstractTableModel>
#include <QQuickItem>
#include "dataManager.h"


class SystemInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SystemInfoModel(QObject *parent = nullptr);

    enum MyRoles {
        Terminal = Qt::UserRole + 100,
        Name = Qt::UserRole + 102,
        Numberr = Qt::UserRole + 103,
        Type = Qt::UserRole + 104,
        Latitude = Qt::UserRole + 105,
        Longitude = Qt::UserRole + 106,
        Altitude = Qt::UserRole + 107,
        ViewRange = Qt::UserRole + 108,
        MezRange = Qt::UserRole + 109,
        ReceiveTime = Qt::UserRole + 110,
        Simulation = Qt::UserRole + 111,
        BCCStatus = Qt::UserRole + 112,
        RadarSearchStatus = Qt::UserRole + 113,
        Operational = Qt::UserRole + 114,
        MissileCount = Qt::UserRole + 115,
        RadarMode = Qt::UserRole + 116,
        //combat info
        TN = Qt::UserRole + 117,
        Acceptance = Qt::UserRole + 118,
        Phase = Qt::UserRole + 119,
        Antenna = Qt::UserRole + 120,
        ChanelNo = Qt::UserRole + 121,
        Inrange = Qt::UserRole + 122,
        Active = Qt::UserRole + 123,
        MainInfo = Qt::UserRole + 200,
        MainInfoHeaders = Qt::UserRole + 201,
        LocationInfo = Qt::UserRole + 202,
        LocationInfoHeaders = Qt::UserRole + 203,
        StatusInfo = Qt::UserRole + 204,
        StatusInfoHeaders = Qt::UserRole + 205,
        CombatInfo = Qt::UserRole + 206,
        CombatInfoHeaders = Qt::UserRole + 207
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    SystemInfo getStationInfo() {return mSystemInfo;}
    QHash<int, QByteArray> roleNames() const override;

    void setInformtion(const SystemInfo &systemInfo);
    QStringList getMainInfo() const;
    QStringList getMainInfoHeaders() const;
    QStringList getLocationInfo() const;
    QStringList getLocationInfoHeaders() const;
    QStringList getStatusInfo() const;
    QStringList getStatusInfoHeaders() const;
    QStringList getCombatInfo() const;
    QStringList getCombatInfoHeaders() const;

Q_SIGNALS:
    void gotoButtonClicked();
    void rangeButtonClicked(bool check);
    void wezButtonClicked(bool checked);
    void mezButtonClicked(bool checked);
    void activeButtonToggled(bool checked);
    void moreButtonClicked();

private:
    SystemInfo mSystemInfo;
};

class SystemInformation : public QObject
{
    Q_OBJECT

public:
    SystemInformation(QQmlEngine *qmlEngine, UIHandle *uiHandle, SystemInfo systemInfo, QObject *parent = nullptr);
    SystemInfoModel *getInfo() {return mInfoModel;}
    void show();
private:
    SystemInfoModel *mInfoModel;
    UIHandle *mUiHandle;
    QQuickItem *mItem;
    SystemInfo mInformation;
};

#endif
