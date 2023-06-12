#include "layersmodel.h"
#include <osgEarth/Map>
#include <osgEarth/ModelLayer>



LayersModel::LayersModel(MapItem *mapItem, QObject *parent) :
    QStandardItemModel(parent)
{
    mMapItem = mapItem;
        updateLayers(mapItem->getMapNode()->getMap());
        connect(mapItem, &MapItem::layerChanged,[this, mapItem](){
            updateLayers(mapItem->getMapNode()->getMap());
        });
        connect(mapItem, &MapItem::mapCleared,[this, mapItem](){
            clear();
            updateLayers(mapItem->getMapNode()->getMap());
        });


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
}

//int LayersModel::columnCount(const QModelIndex &parent) const
//{
//    return 1;
//}

void LayersModel::updateLayers(osgEarth::Map *map)
{
//        beginResetModel();
//        mLayersList.clear();
        this->clear();

        QStandardItem *rootItem = invisibleRootItem();

        osgEarth::LayerVector layers;
        map->getLayers(layers);

        for(const auto& layer : layers) {
//            mLayersList.push_back(layer);
            QStandardItem *lv1Items = new QStandardItem(QString(layer->getName().c_str()));
            rootItem->appendRow(lv1Items);
            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
            if (group) {
                group->getNumChildren();
                for(int i = 0; i < group->getNumChildren(); i++){
                    auto node = group->getChild(i);
                    QStandardItem *lv2Items = new QStandardItem(QString(node->getName().c_str()));
                    lv1Items->appendRow(lv2Items);
                }
            }
        }

//        endResetModel();
}

//void LayersModel::clear()
//{
//        beginResetModel();
//        mLayersList.clear();
//        endResetModel();
//}

void LayersModel::toggleLayerEnabled(int layerIndex)
{
//        qDebug() << layerIndex;
//        if (layerIndex /*< mLayersList.size()*/) {
            auto layer = mMapItem->getMapNode()->getMap()->getLayerAt(layerIndex);
            auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
            if (visibleLayer) {
                visibleLayer->setVisible(!visibleLayer->getVisible());
            } else {
                layer->setEnabled(!layer->getEnabled());
            }
            emit dataChanged(index(layerIndex,0),
                             index(layerIndex,0),
                             {LayerEnabledRole});
//        }
}

//int LayersModel::rowCount(const QModelIndex &parent) const
//{
//        return mLayersList.size();
//}

//QVariant LayersModel::data(const QModelIndex &index, int role) const
//{
//        if (index.row() >= mLayersList.size())
//            return QVariant();

//        switch (role) {
//        case Qt::DisplayRole:
//        {
//            return QVariant::fromValue<QString>(mLayersList[index.row()]->getName().c_str());

//            break;
//        }
//        case LayerIndexRole:
//        {
//            return index.row();
//            break;
//        }

//        case LayerEnabledRole:
//        {
//            auto layer = mLayersList[index.row()];
//            auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
//            if (visibleLayer) {
//                return QVariant::fromValue<bool>(visibleLayer->getVisible());
//            } else {
//                return QVariant::fromValue<bool>(layer->getEnabled());
//            }
//            break;
//        }

//        }

//        return QVariant();
//}

//QHash<int, QByteArray> LayersModel::roleNames() const
//{
//        auto hash = QAbstractListModel::roleNames();
//        hash[LayerIndexRole] = "layer_index";
//        hash[LayerEnabledRole] = "layer_enabled";
//        return hash;
//}
