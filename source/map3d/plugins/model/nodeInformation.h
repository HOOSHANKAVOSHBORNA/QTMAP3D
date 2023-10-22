#ifndef NODEINFORMATION_H
#define NODEINFORMATION_H

#include <QObject>
#include <QStandardItemModel>

struct NodeInformationItem:public QObject{
    Q_OBJECT
public:
    NodeInformationItem(QString parent,QString property, QString value,QString iconImageUrl);
    QString parent;
    QString property;
    QString value;
    QString iconImageUrl;
};

struct HeaderItem
{
public:
    HeaderItem(QString mainImageUrl, QString headerImageUrl, QString windowName, bool isBookmarked);
    QString mainImageUrl;
    QString headerImageUrl;
    QString windowName;
    bool isBookmarked;
};

class NodeInformation:public QStandardItemModel
{
    Q_OBJECT
    enum CustomRoles {
        propertyText = Qt::UserRole + 100,
        valueText,
        iconImageSource
    };


public:
    explicit NodeInformation(QObject *parent = nullptr);
    ~NodeInformation();
    NodeInformationItem* addNodeInformationItem(NodeInformationItem* nodeInformationItem);
    void updateNodeInformationItem(NodeInformationItem* node, QString value);
    virtual QHash<int, QByteArray> roleNames() const override;
    void setHeaderItem(HeaderItem *headerItem);
    Q_INVOKABLE QString getMainImageUrl();
    Q_INVOKABLE QString getHeaderImageUrl();
    Q_INVOKABLE QString getWindowName();
    Q_INVOKABLE bool bookmarkStatus();

private:
    QStandardItem *rootItem;
    HeaderItem *mHeaderItem;
    std::map<QString, std::pair<NodeInformationItem*, QStandardItem*>> mItems;
};

#endif // NODEINFORMATION_H
