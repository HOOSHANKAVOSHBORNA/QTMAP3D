#ifndef STATIONINFOMODEL_H
#define STATIONINFOMODEL_H
#include <QAbstractListModel>
#include <QQuickItem>

#include "dataManager.h"

class StationInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MyRoles {
        Numberr = Qt::UserRole + 200,
        Latitude = Qt::UserRole + 204,
        Longitude = Qt::UserRole + 205,
        Radius = Qt::UserRole + 206,
        CycleTime = Qt::UserRole + 207,
        Active = Qt::UserRole + 208,
        MainInfo = Qt::UserRole + 209,
        MainInfoHeaders = Qt::UserRole + 210,
        LocationInfoHeaders = Qt::UserRole + 211,
        LocationInfo = Qt::UserRole + 212
    };

    StationInfoModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    StationInfo getStationInfo() {return mStationInfo;}
    QHash<int, QByteArray> roleNames() const override;

    void setInformtion(const StationInfo &stationInfo);
    QStringList getMainInfo() const;
    QStringList getMainInfoHeaders() const;
    QStringList getLocationInfo() const;
    QStringList getLocationInfoHeaders() const;

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
