#ifndef CIRCULARMENUMODEL_H
#define CIRCULARMENUMODEL_H

#include <QObject>
#include <QAbstractItemModel>

struct CircularMenuItem
{
public:
    QString name;
    QString iconUrl;
    bool    checkable = false;
    bool    checked = false;
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

private:
    QVector<CircularMenuItem*> mItems;
};

#endif // CIRCULARMENUMODEL_H
