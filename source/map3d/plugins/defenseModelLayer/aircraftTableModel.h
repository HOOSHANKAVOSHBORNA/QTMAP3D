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

class AircraftTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AircraftTableModel(QObject * parent = nullptr);

    enum CustomRoles {
        BackColorRole = Qt::UserRole + 100,
        TextColorRole = Qt::UserRole + 101,
        HeaderTextRole = Qt::UserRole + 102
    };

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE QString headerText(int column) const;

    Q_INVOKABLE QString getTN(int row) const;

public slots:
    void setFilterWildcard(const QString& wildcard);


public:
    void updateItemData(const QString& jsonStr);
    void updateItemData(const AircraftInfo& aircraftInfo);
    void deleteItem(int TN);

private:
    std::deque<QPair<int, QSharedPointer<AircraftInfo>>> mAircraftInfoList;
    std::deque<QPair<int, QSharedPointer<AircraftInfo>>> mAircraftInfoListProxy;

    QString mFilter;
};

#endif // AIRCRAFTTABLEMODEL_H
