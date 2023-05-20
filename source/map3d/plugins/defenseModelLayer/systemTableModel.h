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

class SystemDataManager;
class DefenseModelLayer;
namespace System {
struct Data;
}
namespace Aircraft {
struct Data;
}

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

    void setSystemInfos(const QMap<int, System::Data*> & info);
    void setAircraftInfos(const QMap<int, Aircraft::Data*> &info);
signals:
    void systemClicked(const int&);
public slots:
    void setFilterWildcard(const QString& wildcard);
    void refresh();
    void onInfoChanged(int number);
    void onRemoveData(int number);
    void onSystemClicked(const int &number);
    void onAircraftClicked(const int &tn);
    void updateAssignment(int tn);

private:
    QString mFilter;
    int mTN = -1;
    const QMap<int, System::Data*> *mSystemInfos;
    const QMap<int, Aircraft::Data*> *mAircraftInfos;
    QList<int> mSystemInfosProxy;
};

class SystemTable : QObject
{
    Q_OBJECT
public:
    SystemTable(SystemDataManager *systemDataManger, DefenseModelLayer *defenseModelLayer, QObject *parent=nullptr);
    SystemTableModel* getModel() const;
public slots:
    void onDoubleClicked(int number);
private:
    SystemDataManager *mSystemDataManger;
    DefenseModelLayer *mDefenseModelLayer;
    SystemTableModel *mSystemTableModel;
};

#endif // SYSTEMTABLEMODEL_H
