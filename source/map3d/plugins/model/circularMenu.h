#ifndef CIRCULARMENU_H
#define CIRCULARMENU_H

#include <QObject>
#include <QAbstractItemModel>

#include "mapItem.h"
#include "qmlNode.h"
class SimpleModelNode;
class CircularMenuModel;

struct CircularMenuItem : public QObject
{
    Q_OBJECT

public:
    CircularMenuItem(
        QString _name      = QString(),
        QString _iconUrl   = QString(),
        bool    _checkable = false
        ):
        name     (_name     ),
        iconUrl  (_iconUrl  ),
        checkable(_checkable)
    {

    }

    QString name;
    QString iconUrl;
    bool    checkable = false;
    bool    checked = false;

signals:
    void itemClicked();
};

// ------------------------------------------------------- model manager
class CircularMenu : public QObject
{
public:
    CircularMenu(MapItem *mapItem, QQmlEngine *qQmlEngine, QObject *parent = nullptr);

    void createQML(SimpleModelNode *smn);

    void setQmlNode(QmlNode *newQmlNode);

    QmlNode *qmlNode() const;

private:
    CircularMenuModel *mModel;
    MapItem *mMapItem;
    QQmlEngine *mEngine;
    QmlNode *mQmlNode{nullptr};
};

// ------------------------------------------------------- model
class CircularMenuModel : public QAbstractListModel
{
    Q_OBJECT

    enum {
        NameRole,
        IconUrlRole,
        CheckableRole,
        CheckedRole
    };

public:
    explicit CircularMenuModel(QObject *parent = nullptr);

public:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    void onItemClicked(const QModelIndex &current);

private:
    QVector<CircularMenuItem*> mItems;
};

#endif // CIRCULARMENU_H
