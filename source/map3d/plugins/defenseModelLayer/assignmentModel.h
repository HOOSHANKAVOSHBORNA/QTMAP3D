
#ifndef ASSIGNMENTMODEL_H
#define ASSIGNMENTMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <deque>
#include <QSharedPointer>
#include <QPair>


struct AircraftAssign;

struct SystemAssign {
    int Number;
    QList<AircraftAssign> Aircrafts;
};

struct AircraftAssign {
    int Number;
    QList<SystemAssign> Systems;
};

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

    void onAircraftClicked(int row);
    void onSystemClicked(int row);
    void updateItemData(const AircraftAssign &aircraft);
    void updateData(const SystemAssign &system);
    void clear();
public:
    Q_INVOKABLE int getAircraftNumber(int row) const;
    Q_INVOKABLE int getSystemNumber(int row) const;
    Q_INVOKABLE QString headerText(int column) const;
private:
    std::deque<QPair<int, QSharedPointer<AircraftAssign>>> mAircraftList;
    std::deque<QPair<int, QSharedPointer<SystemAssign>>> mSystemList;
    std::deque<QPair<int, QSharedPointer<AircraftAssign>>> mAircraftListProxy;
    std::deque<QPair<int, QSharedPointer<SystemAssign>>> mSystemListProxy;

};

#endif
