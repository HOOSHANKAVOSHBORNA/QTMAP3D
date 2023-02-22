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
        Numberr = Qt::UserRole + 102,
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

    void setInformtion(const SystemInfo &systemInfo, const SystemStatusInfo &systemStatusInfo, const SystemCambatInfo &systemCombatInfo);
    void setCombatInfo(const SystemCambatInfo &combatInfo);
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
    SystemCambatInfo mSystemCombatInfo;
};

class SystemInformation : public QObject
{
    Q_OBJECT

public:
    SystemInformation(QQmlEngine *qmlEngine, UIHandle *uiHandle, SystemInfo systemInfo, SystemStatusInfo systemStatusInfo, SystemCambatInfo systemCambatInfo, QObject *parent = nullptr);
    SystemInfoModel *getInfo() {return mInfoModel;}
    void setInfo(const SystemInfo &systemInfo);
    void setStatusInfo(const SystemStatusInfo &systemStatusInfo);
    void setCombatInfo(const SystemCambatInfo &systemCombatInfo);
    void show();
private:
    SystemInfoModel *mInfoModel;
    UIHandle *mUiHandle;
    QQuickItem *mItem;
    SystemInfo mInformation;
};

#endif
