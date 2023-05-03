#ifndef SYSTEMTABLEMODEL_H
#define SYSTEMTABLEMODEL_H

#include "defenseDataManager.h"

#include <QAbstractTableModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <deque>
#include <QSharedPointer>

struct SystemAssignInfo {
    int Number;
    QString Phase;
    bool assign = false;
};

class SystemTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SystemTableModel(QObject *parent = nullptr);
    enum CustomRoles {
        BackColorRole = Qt::UserRole + 100,
        TextColorRole = Qt::UserRole + 101,
        HeaderTextRole = Qt::UserRole + 102,
        AssignColor = Qt::UserRole + 103,
        SystemColor = Qt::UserRole + 104
    };

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE QString headerText(int column) const;
    Q_INVOKABLE int getNumber(int row) const;

public slots:
    void setFilterWildcard(const QString& wildcard);
    void onAircraftClicked(int TN);
    void onSystemClicked(int Number);
    bool getShowAssigned();
    void refresh();

signals:
    void systemClicked(int Number);
public:

    void assign(int TN, int Number);
    void cancelSystemsAssigned(int TN, int ExceptNum);
    void cancelAllAssigns();
    void cancelAssign(int TN, int Number);
    void acceptAssign(int TN, int Number, bool result);
    void setMode(QString mode);

    QMap<int, QList<SystemAssignInfo>> getAssignmentMap();

    void setSystemInfos(QMap<int, SystemInfo> & info);
    void setSystemStatusInfos(QMap<int, SystemStatusInfo> & statusInfos);
    void setSystemCombatInfos(QMap<int, SystemCombatInfo> & combatInfos);
    void updateTable(int number);



private:
    QMap<int, QList<SystemAssignInfo>> mSystemsAssigned;

    QString mMode;
    int mTN = -1;
    QString mFilter;

    QMap<int, SystemInfo> *mSystemInfos;
    QMap<int, SystemStatusInfo> *mSystemStatusInfos;
    QMap<int, SystemCombatInfo> *mSystemCombatInfos;

    QList<int> mSystemInfosProxy;

};

#endif // SYSTEMTABLEMODEL_H
