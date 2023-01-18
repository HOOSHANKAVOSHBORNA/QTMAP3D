
#include "outlinermodel.h"

OutlinerModel::OutlinerModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int OutlinerModel::rowCount(const QModelIndex &parent) const
{
    return mLayerInfoList.count();
}

int OutlinerModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QHash<int, QByteArray> OutlinerModel::roleNames() const
{
    auto hash = QAbstractListModel::roleNames();
    hash[LayerModelRule] = "layer_model";
    return hash;
}

QVariant OutlinerModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row();
    if (row >= mLayerInfoList.size()) return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        break;
    }

    case OutlinerModel::LayerModelRule: {
        return QVariant::fromValue<LayerModel*>(mLayerInfoList[row]->layerModel);
    }
    }
}

LayerModel::LayerModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int LayerModel::rowCount(const QModelIndex &parent) const
{

}

int LayerModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QHash<int, QByteArray> LayerModel::roleNames() const
{

}

QVariant LayerModel::data(const QModelIndex &index, int role) const
{

}
