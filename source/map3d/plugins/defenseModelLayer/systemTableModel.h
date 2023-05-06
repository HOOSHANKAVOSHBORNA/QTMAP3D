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
namespace System {
struct Data;
}

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

    void setSystemInfos(QMap<int, System::Data> & info);
    void updateTable(int number);
    void setMode(QString mode);

public slots:
    void setFilterWildcard(const QString& wildcard);
    void onAircraftClicked(int TN);
    void onSystemClicked(int Number);
    bool getShowAssigned();
    void refresh();

signals:
    void systemClicked(int Number);


private:
    QMap<int, System::Data> mSystemsAssigned;

    QString mMode;
    int mTN = -1;
    QString mFilter;

    QMap<int, System::Data> *mSystemInfos;

    QList<int> mSystemInfosProxy;

};

#endif // SYSTEMTABLEMODEL_H
