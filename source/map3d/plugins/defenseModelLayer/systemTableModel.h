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
    void refresh(int indx);

signals:
    void systemClicked(int Number);
public:
    void updateItemData(const QString& jsonStr);
    void updateItemData(const SystemInfo& systemInfo);
    void updateItemData(const SystemStatusInfo& systemStatusInfo);
    void updateItemData(const SystemCambatInfo& systemCambatInfo);
    void assign(int TN, int Number);
    void cancelSystemsAssigned(int TN, int ExceptNum);
    void cancelAllAssigns();
    void cancelAssign(int TN, int Number);
    void acceptAssign(int TN, int Number, bool result);
    void clearList();

    QMap<int, QList<SystemAssignInfo>> getAssignmentMap();


private:
    std::deque<QPair<int, QSharedPointer<SystemInfo>>> mSystemInfoList;
    std::deque<QPair<int, QSharedPointer<SystemInfo>>> mSystemInfoListProxy;
    std::deque<QPair<int, QSharedPointer<SystemStatusInfo>>> mSystemStatusInfoList;
    std::deque<QPair<int, QSharedPointer<SystemStatusInfo>>> mSystemStatusInfoListProxy;
    std::deque<QPair<int, QSharedPointer<SystemCambatInfo>>> mSystemCombatInfoList;
    std::deque<QPair<int, QSharedPointer<SystemCambatInfo>>> mSystemCombatInfoListProxy;

    QMap<int, QList<SystemAssignInfo>> mSystemsAssigned;

    bool mshowAssigned = false;
    int mTN = -1;
    QString mFilter;
    int mIndex = -1;

};

#endif // SYSTEMTABLEMODEL_H
