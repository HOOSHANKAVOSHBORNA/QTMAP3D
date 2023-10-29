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
        iconImageSource,
        mainImageSource,
        windowText,
        mainIconImageSource
    };

    Q_PROPERTY(QString mainImageUrl READ mainImageUrl WRITE setMainImageUrl NOTIFY MainImageUrlChanged FINAL)

public:
    explicit NodeInformation(QQmlEngine* Engine ,QObject *parent = nullptr);
    ~NodeInformation();
    void addUpdateNodeInformationItem(NodeData* nodeData);
    virtual QHash<int, QByteArray> roleNames() const override;
    void show();
    Q_INVOKABLE QString getWindowName();
    Q_INVOKABLE QString getIconImageUrl();

    QQuickWindow *wnd() const;

    Q_INVOKABLE QString mainImageUrl() const;
    void setMainImageUrl(const QString &newMainImageUrl);

signals:
    void bookmarkChecked(bool check);
    void MainImageUrlChanged();

private:
    QStandardItem *rootItem;
    QString windowName;
    QString iconImageUrl;
    std::map<QString, QStandardItem*> mItems;
    QQuickWindow *mWnd;
    QString mMainImageUrl;
};

#endif // NODEINFORMATION_H
