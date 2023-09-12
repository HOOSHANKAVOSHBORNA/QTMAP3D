#ifndef ContextMenumodel_H
#define ContextMenumodel_H
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QStringList>
#include "plugininterface.h"
class ContextMenumodel : public QAbstractListModel
{
    Q_OBJECT
public:
    ContextMenumodel(QObject * parent = nullptr);

    enum MyRoles {
        TextToShow = Qt::UserRole + 200
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addRow(QString row) {systemsList << row;}
    QHash<int, QByteArray> roleNames() const override;
    QStringList getList() {return systemsList;}
Q_SIGNALS:
    void itemClicked(int index, QString systemName);
private:
    QStringList systemsList;
};


class ContextMenu : public QObject
{
    Q_OBJECT
public:
    ContextMenu(QQmlEngine *qmlEngine, QObject *parent = nullptr);
    void addRow(QString row){mContextMenuModel->addRow(row);}
    void show(int x, int y);
    void hideMenu();
    void updatePosition(int x, int y);
    ContextMenumodel* getModel() {return mContextMenuModel;}
private:
//    UIHandle *mUiHandle;
    QQmlEngine *mQmlEngine;
    ContextMenumodel *mContextMenuModel;
    QQuickItem *mNowContextMenu;

};

#endif // ContextMenumodel_H
