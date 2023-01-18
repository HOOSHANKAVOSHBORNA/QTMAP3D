#ifndef ContextMenumodel_H
#define ContextMenumodel_H
#include <QAbstractListModel>
#include <QStringList>
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
Q_SIGNALS:
    void itemClicked(int index, QString systemName);
private:
    QStringList systemsList;
};

#endif // ContextMenumodel_H
