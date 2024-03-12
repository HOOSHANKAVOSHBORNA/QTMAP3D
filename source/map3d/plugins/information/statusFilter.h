#ifndef STATUSFILTER_H
#define STATUSFILTER_H

#include "mainwindow.h"
#include "qabstractitemmodel.h"
#include "qobject.h"
#include "qqmlengine.h"
#include "qsettings.h"
#include "serviceManager.h"


class StatusFieldModel;

struct fieldModelData{
    QString name;
    bool isActive = true;
};

class StatusFilter : public QObject
{
    Q_OBJECT
public:
    StatusFilter(QQmlEngine *engine,MainWindow *window);
    StatusNodeData filterStatusData(const StatusNodeData &statusNodeData);

private:
    StatusFieldModel *mFilterModel{nullptr};
    StatusNodeData data;
};

class StatusFieldModel : public QAbstractListModel
{
    Q_OBJECT

    enum CustomRoles{
        fieldName = Qt::UserRole,
        fieldIsActive
    };

public:


    StatusFieldModel(QObject *parent = nullptr);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void toggleCheck(const QModelIndex &index, bool check);
    void addField(QString name);
    QVector<QString> getStatusActiveFields();

    Q_INVOKABLE void fakeData();


private:
    QSettings *mStatusSettings;
    QVector<fieldModelData> mFields;
};



#endif // STATUSFILTER_H
