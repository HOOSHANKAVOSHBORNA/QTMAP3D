
#ifndef LAYERSMODEL_H
#define LAYERSMODEL_H

#include <QAbstractItemModel>
#include <QHash>
#include <QByteArray>
#include <QList>
#include <osgEarth/Layer>

class LayerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    LayerModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    osgEarth::Layer *mOsgEarthLayer = nullptr;
};

struct LayerInfo
{
    QString     layerName;
    LayerModel *layerModel = nullptr;
};

class OutlinerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum OutlinerModelRules {
        LayerModelRule = Qt::UserRole + 300
    };


public:
    OutlinerModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<LayerInfo*> mLayerInfoList;
};

#endif // LAYERSMODEL_H
