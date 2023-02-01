
#include "layersmodel.h"
#include <osgEarth/Map>
#include <osgEarth/ModelLayer>


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
        auto layer = mLayersList[layerIndex];
        auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
        if (visibleLayer) {
            visibleLayer->setVisible(!visibleLayer->getVisible());
        } else {
            layer->setEnabled(!layer->getEnabled());
        }
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
        auto layer = mLayersList[index.row()];
        auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
        if (visibleLayer) {
            return QVariant::fromValue<bool>(visibleLayer->getVisible());
        } else {
            return QVariant::fromValue<bool>(layer->getEnabled());
        }
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
