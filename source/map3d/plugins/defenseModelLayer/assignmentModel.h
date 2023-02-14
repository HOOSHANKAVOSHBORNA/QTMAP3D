
#ifndef ASSIGNMENTMODEL_H
#define ASSIGNMENTMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <deque>
#include <QSharedPointer>
#include <QPair>
#include <defenseDataManager.h>

class AssignmentModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum MyRoles {
      AirDisp = Qt::UserRole + 100,
      SysDisp = Qt::UserRole + 101,
      HeaderText = Qt::UserRole + 102
    };
    AssignmentModel(QObject *parent = nullptr);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    void onSystemClicked(int row);
//    void updateData(int TN, int Number);
    void assignAirToSystem(AircraftInfo &aircraft, SystemInfo &system);
    void addAircraft(AircraftInfo aircraft);
    void addSystem(SystemInfo system);
    void clear();

public slots:
    void onAircraftClicked(int row);
public:
    Q_INVOKABLE int getAircraftNumber(int row) const;
    Q_INVOKABLE int getSystemNumber(int row) const;
    Q_INVOKABLE QString headerText(int column) const;
private:
    std::deque<QPair<int, QSharedPointer<AircraftInfo>>> mAircraftList;
    std::deque<QPair<int, QSharedPointer<SystemInfo>>> mSystemList;
    std::deque<QPair<int, QSharedPointer<AircraftInfo>>> mAircraftListProxy;
    std::deque<QPair<int, QSharedPointer<SystemInfo>>> mSystemListProxy;

};

#endif
