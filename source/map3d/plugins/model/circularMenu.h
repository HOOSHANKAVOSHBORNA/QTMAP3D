#ifndef CIRCULARMENU_H
#define CIRCULARMENU_H

#include <QObject>
#include <QAbstractItemModel>
#include <osgEarthAnnotation/ModelNode>

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
        bool    _checkable = false,
        QString _iconCheckedUrl = QString()
    ):
        name            (_name     ),
        iconUrl         (_iconUrl  ),
        checkable       (_checkable),
        iconCheckedUrl  (_iconCheckedUrl)
    {

    }

    QString name;
    QString iconUrl;
    bool    checkable = false;
    bool    checked = false;
    QString iconCheckedUrl;

signals:
    void itemClicked();
    void itemChecked(bool checked);
};

// ------------------------------------------------------- model manager
class CircularMenu : public QObject
{
public:
    CircularMenu(QQuickItem *parentItem, osgEarth::Annotation::GeoPositionNode *osgNode);
    void appendMenuItem(CircularMenuItem* item);
    void removeMenuItem(CircularMenuItem* item);
    void show(bool show);
    void setQmlNode(QmlNode *newQmlNode);
    QmlNode *qmlNode() const;
private:
    void createQML();
private:
    CircularMenuModel *mCircularMenuModel;
    QQuickItem *mParentQmlItem;
    osgEarth::Annotation::GeoPositionNode *mOsgNode;
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
        CheckedRole,
        IconChehckedUrlRole
    };

public:
    explicit CircularMenuModel(QObject *parent = nullptr);

public:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    void appendMenuItem(CircularMenuItem* item);
    void removeMenuItem(CircularMenuItem* item);

public slots:
    void onItemClicked(const QModelIndex &current);
    void onItemChecked(const QModelIndex &current, bool checked);

private:
    QVector<CircularMenuItem*> mItems;
};

#endif // CIRCULARMENU_H
