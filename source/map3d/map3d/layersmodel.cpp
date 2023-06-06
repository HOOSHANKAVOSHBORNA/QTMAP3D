#include "layersmodel.h"
#include <osgEarth/Map>
#include <osgEarth/ModelLayer>



LayersModel::LayersModel(MapItem *mapItem, QObject *parent) :
    QAbstractListModel(parent)
{

        updateLayers(mapItem->getMapNode()->getMap());
        connect(mapItem, &MapItem::layerChanged,[this, mapItem](){
            updateLayers(mapItem->getMapNode()->getMap());
        });
        connect(mapItem, &MapItem::mapCleared,[this, mapItem](){
            clear();
            updateLayers(mapItem->getMapNode()->getMap());
        });


//    QStandardItem *rootItem = invisibleRootItem();
//    QStandardItem *group1 = new QStandardItem;
//    QStandardItem *group2 = new QStandardItem;
//    QStandardItem *group3 = new QStandardItem;

//    QStandardItem *value1 = new QStandardItem;
//    QStandardItem *value2 = new QStandardItem;
//    QStandardItem *value3 = new QStandardItem;
//    QStandardItem *value4 = new QStandardItem;
//    QStandardItem *value5 = new QStandardItem;
//    QStandardItem *value6 = new QStandardItem;
//    QStandardItem *value7 = new QStandardItem;
//    QStandardItem *value8 = new QStandardItem;
//    QStandardItem *value9 = new QStandardItem;

//    rootItem->appendRow(group1);
//    rootItem->appendRow(group2);
//    rootItem->appendRow(group3);

//    group1->appendRow(value1);
//    group1->appendRow(value2);
//    group1->appendRow(value3);
//    group2->appendRow(value4);
//    group2->appendRow(value5);
//    group2->appendRow(value6);
//    group3->appendRow(value7);
//    group3->appendRow(value8);
//    group3->appendRow(value9);

//    group1->setText("Branch1");
//    group2->setText("Branch2");
//    group3->setText("Branch3");

//    value1->setText("value1");
//    value2->setText("value2");
//    value3->setText("value3");
//    value4->setText("value4");
//    value5->setText("value5");
//    value6->setText("value6");
//    value7->setText("value7");
//    value8->setText("value8");
//    value9->setText("value9");
//}

//int LayersModel::columnCount(const QModelIndex &parent) const
//{
//    return 1;
//}


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

void LayersModel::clear()
{
        beginResetModel();
        mLayersList.clear();
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
