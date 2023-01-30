
#include "layersmodel.h"
#include <osgEarth/Map>


LayersModel::LayersModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

void LayersModel::updateLayers(osgEarth::Map *map)
{
    beginResetModel();

    mLayersList.clear();

    osgEarth::LayerVector layers;
    map->getLayers(layers);

    for(auto layer : layers) {
        mLayersList.push_back(layer);
    }

    endResetModel();
}

void LayersModel::toggleLayerEnabled(int layerIndex)
{
    if (layerIndex < mLayersList.size()) {
        mLayersList[layerIndex]->setEnabled(!mLayersList[layerIndex]->getEnabled());
        emit dataChanged(index(layerIndex),
                         index(layerIndex),
                         {LayerEnabledRole});
    }
}

int LayersModel::rowCount(const QModelIndex &parent) const
{
    return mLayersList.size();
}

QVariant LayersModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= mLayersList.size())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        return QVariant::fromValue<QString>(mLayersList[index.row()]->getName().c_str());

        break;
    }

    case LayerIndexRole:
    {
        return index.row();
        break;
    }

    case LayerEnabledRole:
    {
        return QVariant::fromValue<bool>(mLayersList[index.row()]->getEnabled());
        break;
    }

    }

    return QVariant();
}

QHash<int, QByteArray> LayersModel::roleNames() const
{
    auto hash = QAbstractListModel::roleNames();
    hash[LayerIndexRole] = "layer_index";
    hash[LayerEnabledRole] = "layer_enabled";
    return hash;
}
