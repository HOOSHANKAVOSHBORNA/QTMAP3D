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

class SystemTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SystemTableModel(QObject *parent = nullptr);
    enum CustomRoles {
        BackColorRole = Qt::UserRole + 100,
        TextColorRole = Qt::UserRole + 101,
        HeaderTextRole = Qt::UserRole + 102
    };

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE QString headerText(int column) const;
    Q_INVOKABLE QString getName(int row) const;

public slots:
    void setFilterWildcard(const QString& wildcard);

public:
    void updateItemData(const QString& jsonStr);
    void updateItemData(const SystemInfo& systemInfo);

private:
    std::deque<QPair<int, QSharedPointer<SystemInfo>>> mSystemInfoList;
    std::deque<QPair<int, QSharedPointer<SystemInfo>>> mSystemInfoListProxy;

    QString mFilter;

};

#endif // SYSTEMTABLEMODEL_H
