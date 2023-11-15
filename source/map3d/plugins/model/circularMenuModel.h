#ifndef CIRCULARMENUMODEL_H
#define CIRCULARMENUMODEL_H

#include <QObject>
#include <QAbstractItemModel>

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

#endif // CIRCULARMENUMODEL_H
