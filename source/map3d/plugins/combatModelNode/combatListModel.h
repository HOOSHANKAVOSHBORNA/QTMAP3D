#ifndef COMBATLISTMODEL_H
#define COMBATLISTMODEL_H

#include "qqmlengine.h"
#include "qquickwindow.h"
#include "qstandarditemmodel.h"
#include <QObject>

class CombatListModel : public QStandardItemModel
{
    Q_OBJECT

    enum CustomRoles {
        ID = Qt::UserRole,
        icon,
        selection
    };


public:
    explicit CombatListModel(QObject *parent = nullptr);
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QStandardItem *mRootItem;
};


class CombatList:public QObject
{
    Q_OBJECT
public:
    explicit CombatList(QQmlEngine* Engine, QQuickWindow *parent = nullptr);
    ~CombatList();

    CombatListModel *model() const;
    QQuickWindow *window() const;

signals:
    void goToPosition();
    void track();

private:
    CombatListModel *mCombatListModel;
    QQuickWindow *mWindow{nullptr};
};


#endif // COMBATLISTMODEL_H

