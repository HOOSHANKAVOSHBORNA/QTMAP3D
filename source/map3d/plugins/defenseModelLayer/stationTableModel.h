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
\
class StationDataManager;
class DefenseModelLayer;
namespace Station {
struct Data;
}
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
    void onInfoChanged(int number);

public:
    void setStationInfos(const QMap<int, Station::Data*> & stationInfos);
private:
    const QMap<int, Station::Data*> *mStationInfos;
    QList<int> mStationInfosProxy;
    QString mFilter = "";

};

class StationTable : public QObject {
    Q_OBJECT
public:
    StationTable(StationDataManager *stationDataManager, DefenseModelLayer* defenseModelLayer, QObject* parent=nullptr);
public slots:
    void onDoubleClicked(const int&);
private:
    StationDataManager *mStationDataManager;
    DefenseModelLayer *mDefenseModelLayer;
    StationTableModel *mStationTableModel;
};

#endif // STATIONTABLEMODEL_H
