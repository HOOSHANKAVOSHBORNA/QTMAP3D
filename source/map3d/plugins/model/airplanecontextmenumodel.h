#ifndef AIRPLANECONTEXTMENUMODEL_H
#define AIRPLANECONTEXTMENUMODEL_H
#include <QAbstractListModel>
#include <QStringList>

class AirplaneContextMenumodel : public QAbstractListModel
{
    Q_OBJECT
public:
    AirplaneContextMenumodel(QObject * parent = nullptr);


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addRow(QString row) {a_list << row;}
private:
    QStringList a_list;
};

#endif // AIRPLANECONTEXTMENUMODEL_H
