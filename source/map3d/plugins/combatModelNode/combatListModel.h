#ifndef COMBATLISTMODEL_H
#define COMBATLISTMODEL_H

#include "combatManager.h"
#include "mapControllerItem.h"
#include "qqmlengine.h"
#include <QObject>

class CombatListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString title       READ getTitle       WRITE setTitle       NOTIFY actorDataUpdated FINAL)
    Q_PROPERTY(QUrl iconUrl        READ getIconUrl     WRITE setIconUrl     NOTIFY actorDataUpdated FINAL)
    Q_PROPERTY(bool isAttacker     READ getIsAttacker  WRITE setIsAttacker  NOTIFY actorDataUpdated FINAL)
    Q_PROPERTY(int bulletCount     READ getBulletCount WRITE setBulletCount NOTIFY actorDataUpdated FINAL)
public:
    enum CustomRoles {
        ID = Qt::UserRole,
        icon,
        selection,
        hover,
        stateColor
    };

    explicit CombatListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    void addData(assignmentData data);
    Q_INVOKABLE void objectHover(QModelIndex index, bool isHovered);
    Q_INVOKABLE void objectSelect(QModelIndex index, bool isSelected);

    void setTitle(QString title);
    QString getTitle();
    void setIconUrl(QUrl iconUrl);
    QUrl getIconUrl();
    void setIsAttacker(bool isAttacker);
    bool getIsAttacker();
    void setBulletCount(int bulletCount);
    int getBulletCount();

signals:
    void actorDataUpdated();

private:

    QVector<assignmentData> mAssignList;
    QString mTitle;
    QUrl mIconUrl;
    bool mIsAttacker{false};
    int mBulletCount{0};
};


class CombatList:public QObject
{
    Q_OBJECT


public:
    explicit CombatList(QQmlEngine* engine, MapControllerItem *map = nullptr);
    ~CombatList();

    CombatListModel *getCombatModel() const;
    void setCombatMenuVisible(bool visible);

private:
    MapControllerItem *mMapItem;
    CombatListModel *mCombatListModel;



};


#endif // COMBATLISTMODEL_H

