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
        AircraftHoverColor = Qt::UserRole + 104
    };

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
    Q_INVOKABLE QString headerText(int column) const;

    Q_INVOKABLE int getTN(int row) const;

    void setAircraftInfos(QMap<int, AircraftInfo> & aircrafts);
    void updateTable(int tn);

public slots:
    void setFilterWildcard(const QString& wildcard);
    void sortWithHeader(int column);
    void onAircraftClicked(int TN);
    void onSystemClicked(int Number);
    bool getShowAssigned();
    void refresh();
signals:
    void aircraftClicked(int TN);

public:
    void assign(int TN, int Number);
    void cancelAssign(int TN, int Number);
    void cancelAllAssigns();
    void cancelAircraftsAssigned(int ExceptTN, int Number);
    void acceptAssign(int TN, int Number, bool result);
    void setMode(QString mode);

    QMap<int, QList<AircraftAssignInfo>> getAssignmentMap();


private:
    QMap<int, QList<AircraftAssignInfo>> mAircraftsAssigned;

    QMap<int, AircraftInfo> *mAircraftInfos;
    QList<int> mAircraftInfosProxy;


    QString mFilter = "";

    int mNumber = -1;
    QString mMode;
};


#endif // AIRCRAFTTABLEMODEL_H
