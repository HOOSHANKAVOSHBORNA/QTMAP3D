#ifndef NODEINFORMATIONMANAGER_H
#define NODEINFORMATIONMANAGER_H

#include <QObject>
#include <QStandardItemModel>
#include "serviceManager.h"


class NodeInformationModel:public QStandardItemModel
{
    Q_OBJECT
    enum CustomRoles {
        nameText = Qt::UserRole,
        valueText,
        iconImageSource
    };

public:
    explicit NodeInformationModel(QObject *parent = nullptr);
    ~NodeInformationModel();
    void setNodeData(NodeData* nodeData);
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QStandardItem *mRootItem;
    std::map<QString, QStandardItem*> mCategoryItemMap;
    NodeData *mNodeData{nullptr};
};

class NodeInformation:public QObject
{
    Q_OBJECT
public:
    explicit NodeInformation(QQmlEngine* Engine, QQuickWindow *parent = nullptr);
    ~NodeInformation();

    void setNodeData(NodeData* nodeData);
    void show();

    NodeInformationModel *model() const;
    QQuickWindow *window() const;

signals:
    void goToPosition();
    void track();

private:
    NodeInformationModel *mNodeInformationModel;
    QQuickWindow *mWindow{nullptr};
};

#endif // NODEINFORMATIONMANAGER_H
