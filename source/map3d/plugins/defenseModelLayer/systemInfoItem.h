#ifndef SYSTEMINFORMATION_H
#define SYSTEMINFORMATION_H
#include <QAbstractTableModel>
#include <QQuickItem>
#include "aircraftTableModel.h"
#include "systemTableModel.h"

class AircraftModelNode;
class UIHandle;
class QQmlEngine;

class SystemInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SystemInfoModel(QObject *parent = nullptr);

    enum MyRoles {
        Numberr = Qt::UserRole + 102,
        Active = Qt::UserRole + 123,
        MainInfo = Qt::UserRole + 200,
        MainInfoHeaders = Qt::UserRole + 201,
        LocationInfo = Qt::UserRole + 202,
        LocationInfoHeaders = Qt::UserRole + 203,
        StatusInfo = Qt::UserRole + 204,
        StatusInfoHeaders = Qt::UserRole + 205,
        CombatInfo = Qt::UserRole + 206,
        CombatInfoHeaders = Qt::UserRole + 207,
        AssignAircraftsName = Qt::UserRole + 208,
        AssignAircraftsType = Qt::UserRole + 209,
        SystemColor = Qt::UserRole + 210

    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    SystemInfo getStationInfo() {return mSystemInfo;}
    QHash<int, QByteArray> roleNames() const override;

    void setInformtion(const SystemInfo &systemInfo, const SystemStatusInfo &systemStatusInfo, const SystemCombatInfo &systemCombatInfo);
    void setCombatInfo(const SystemCombatInfo &combatInfo);
    void setInfo(const SystemInfo &Info);
    void setStatusInfo(const SystemStatusInfo &statusInfo);
    QStringList getMainInfo() const;
    QStringList getMainInfoHeaders() const;
    QStringList getLocationInfo() const;
    QStringList getLocationInfoHeaders() const;
    QStringList getStatusInfo() const;
    QStringList getStatusInfoHeaders() const;
    QStringList getCombatInfo() const;
    QStringList getCombatInfoHeaders() const;
    QStringList getAssignmentsName() const;
    QStringList getAssignmentsType() const;

    void addAssignment(int number, AircraftModelNode *aircraft);
    void removeAssignment(int number);

Q_SIGNALS:
    void gotoButtonClicked();
    void rangeButtonClicked(bool check);
    void wezButtonClicked(bool checked);
    void mezButtonClicked(bool checked);
    void activeButtonToggled(bool checked);
    void moreButtonClicked();

private:
    SystemInfo mSystemInfo;
    SystemStatusInfo mSystemStatusInfo;
    SystemCombatInfo mSystemCombatInfo;
    QMap<int, const AircraftModelNode*> mAircraftsAssigned;
};

class SystemInfoItem : public QObject
{
    Q_OBJECT

public:
    SystemInfoItem(QQmlEngine *qmlEngine, UIHandle *uiHandle, SystemInfo systemInfo, SystemStatusInfo systemStatusInfo, SystemCombatInfo systemCombatInfo, QObject *parent = nullptr);

    SystemInfoModel *getInfo() {return mInfoModel;}
    void setInfo(const SystemInfo &systemInfo);
    void setStatusInfo(const SystemStatusInfo &systemStatusInfo);
    void setCombatInfo(const SystemCombatInfo &systemCombatInfo);

    void addAssignment(int number, AircraftModelNode *aircraft);
    void removeAssignment(int number);

    void show();
private:
    SystemInfoModel *mInfoModel;
    UIHandle *mUiHandle;
    QQuickItem *mItem;
    SystemInfo mInformation;
};

#endif
