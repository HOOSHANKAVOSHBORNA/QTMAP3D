#ifndef NODEINFORMATION_H
#define NODEINFORMATION_H

#include <QObject>
#include <QStandardItemModel>
#include "serviceManager.h"


class NodeInformation:public QStandardItemModel
{
    Q_OBJECT
    enum CustomRoles {
        nameText = Qt::UserRole + 100,
        valueText,
        iconImageSource
    };
    Q_PROPERTY(QString imageUrl READ imageUrl NOTIFY informationChanged)
    Q_PROPERTY(QString icnUrl READ icnUrl NOTIFY informationChanged)
    Q_PROPERTY(QString title READ title NOTIFY informationChanged)

public:
    explicit NodeInformation(QQmlEngine* Engine ,QObject *parent = nullptr);
    ~NodeInformation();
    void addUpdateNodeInformationItem(NodeData* nodeData);
    virtual QHash<int, QByteArray> roleNames() const override;
    void show();
    QQuickWindow *wnd() const;
   Q_INVOKABLE QString imageUrl() const;
   Q_INVOKABLE QString icnUrl() const;
   Q_INVOKABLE QString title() const;

   signals:
    void informationChanged();
    void bookmarkChecked(bool check);

private:
    QStandardItem *rootItem;
    std::map<QString, QStandardItem*> mItems;
    QQuickWindow *mWnd;
    NodeData *mNodeData{nullptr};
};

#endif // NODEINFORMATION_H
