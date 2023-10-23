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


public:
    explicit NodeInformation(QObject *parent = nullptr);
    ~NodeInformation();
    void addUpdateNodeInformationItem(NodeData* nodeData, QString imageUrl);
    virtual QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QString getMainImageUrl();
    Q_INVOKABLE QString getWindowName();
signals:
    void bookmarkChecked(bool check);
private:
    QStandardItem *rootItem;
    QString windowName;
    QString mainImageUrl;
    std::map<QString, QStandardItem*> mItems;
};

#endif // NODEINFORMATION_H