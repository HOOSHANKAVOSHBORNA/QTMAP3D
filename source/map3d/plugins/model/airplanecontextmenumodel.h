#ifndef AIRPLANECONTEXTMENUMODEL_H
#define AIRPLANECONTEXTMENUMODEL_H
#include <QAbstractListModel>
#include <QStringList>
class AirplaneContextMenumodel : public QAbstractListModel
{
    Q_OBJECT
public:
    AirplaneContextMenumodel(QObject * parent = nullptr);

    enum MyRoles {
        TextToShow = Qt::UserRole + 200
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addRow(QString row) {a_list << row;}
    QHash<int, QByteArray> roleNames() const;
Q_SIGNALS:
    void returnIndex(int index);
private:
    QStringList a_list;
};

#endif // AIRPLANECONTEXTMENUMODEL_H
