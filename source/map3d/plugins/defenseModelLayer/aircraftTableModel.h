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

namespace Aircraft {
    struct Data;
}
class SystemModelNode;
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

    void setAircraftInfos(const QMap<int, Aircraft::Data> & aircrafts);
    void updateTable(int tn);
    void setMode(QString mode);

public slots:
    void setFilterWildcard(const QString& wildcard);
    void sortWithHeader(int column);
    void onAircraftClicked(int TN);
    void onSystemClicked(int Number);
    bool getShowAssigned();
    void refresh();

signals:
    void aircraftClicked(int TN);

private:

    const QMap<int, Aircraft::Data> *mAircraftInfos;
    QList<int> mAircraftInfosProxy;


    QString mFilter = "";

    int mNumber = -1;
    QString mMode;
};


#endif // AIRCRAFTTABLEMODEL_H
