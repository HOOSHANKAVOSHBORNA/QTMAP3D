#ifndef AIRCRAFTTABLEMODEL_H
#define AIRCRAFTTABLEMODEL_H

#include "defenseDataManager.h"

#include <QAbstractTableModel>
#include <deque>
#include <QSharedPointer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPair>

struct AircraftAssignInfo {
    int TN;
    QString Phase;
    bool assign;
};

class AircraftTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AircraftTableModel(QObject * parent = nullptr);

    enum CustomRoles {
        BackColorRole = Qt::UserRole + 100,
        TextColorRole = Qt::UserRole + 101,
        HeaderTextRole = Qt::UserRole + 102,
        AircraftColor = Qt::UserRole + 103,
        AssignColor = Qt::UserRole + 104
    };

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE QString headerText(int column) const;

    Q_INVOKABLE int getTN(int row) const;

public slots:
    void setFilterWildcard(const QString& wildcard);
    void onAircraftClicked(int TN);
    void onSystemClicked(int Number);
    void onUpdateTimerTriggered();
    bool getShowAssigned();
    void refresh(int indx);
signals:
    void aircraftClicked(int TN);

public:
    void updateItemData(const QString& jsonStr);
    void updateItemData(const AircraftInfo& aircraftInfo);
    void deleteItem(int TN);
    void assign(int TN, int Number);
    void cancelAssign(int TN, int Number);
    void accept(int TN, int Number, bool result);
    void clear();

private:
    std::deque<QPair<int, QSharedPointer<AircraftInfo>>> mAircraftInfoList;
    std::deque<QPair<int, QSharedPointer<AircraftInfo>>> mAircraftInfoListProxy;
    QMap<int, QList<AircraftAssignInfo>> mAircraftsAssigned;


    QString mFilter;
    QString mFilterProxy = "";

    bool mNeedUpdateOnTimerTrigger = false;
    bool mShowAssigned = false;
    int mMinRowUpdate = -1;
    int mMaxRowUpdate = -1;
    int mNumber = -1;
    int mIndex = -1;
};

#endif // AIRCRAFTTABLEMODEL_H
