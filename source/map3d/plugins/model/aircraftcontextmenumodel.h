#ifndef AircraftContextMenumodel_H
#define AircraftContextMenumodel_H
#include <QAbstractListModel>
#include <QStringList>
class AircraftContextMenumodel : public QAbstractListModel
{
    Q_OBJECT
public:
    AircraftContextMenumodel(QObject * parent = nullptr);

    enum MyRoles {
        TextToShow = Qt::UserRole + 200
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addRow(QString row) {systemsList << row;}
    QHash<int, QByteArray> roleNames() const;
Q_SIGNALS:
    void itemClicked(int index, QString systemName);
private:
    QStringList systemsList;
};

#endif // AircraftContextMenumodel_H
