#ifndef STATIONTABLEMODEL_H
#define STATIONTABLEMODEL_H

#include "defenseDataManager.h"

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <deque>
#include <QSharedPointer>

class StationTableModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    StationTableModel(QObject *parent = nullptr);

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
    Q_INVOKABLE int getNumber(int row) const;

public slots:
    void setFilterWildcard(const QString& wildcard);

public:
    void updateItemData(const QString& jsonStr);
    void updateItemData(const StationInfo& stationInfo);

private:
    std::deque<QPair<int, QSharedPointer<StationInfo>>> mStationInfoList;
    std::deque<QPair<int, QSharedPointer<StationInfo>>> mStationInfoListProxy;

    QString mFilter;

};

#endif // STATIONTABLEMODEL_H
