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
    rootItem->clearData();
    osgEarth::LayerVector layers;
    map->getLayers(layers);
    for(const auto& layer : layers) {
        bool hasParent;
        layer->getUserValue("parent", hasParent);
        if(!hasParent){
            QStandardItem *treeItem = new QStandardItem(QString(layer->getName().c_str()));
            rootItem->appendRow(treeItem);
            osg::Node *node = layer->getNode();
            if(node && node->asGroup())
                addChildItem(node->asGroup(), treeItem);
        }
//        //            mLayersList.push_back(layer);
//        QStandardItem *lv1Items = new QStandardItem(QString(layer->getName().c_str()));
//        rootItem->appendRow(lv1Items);
//        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
//        if (group) {
//            for(int i = 0; i < group->getNumChildren(); i++){
//                auto node = group->getChild(i);
//                if(node->asGroup())
//                {
//                    auto group1 = node->asGroup();
//                    QStandardItem *lv2Items = new QStandardItem(QString(group1->getName().c_str()));
//                    lv1Items->appendRow(lv2Items);

//                    for(int j = 0; j < group1->getNumChildren(); j++){
//                        auto node1 = group1->getChild(j);
//                        QStandardItem *lv3Items = new QStandardItem(QString(node1->getName().c_str()));
//                        lv2Items->appendRow(lv3Items);
//                    }
//                }
//                else{
//                    QStandardItem *lv2Items = new QStandardItem(QString(node->getName().c_str()));
//                    //                        qDebug() << lv2Items;
//                    lv1Items->appendRow(lv2Items);
//                }
//            }
//        }
    }

    //        endResetModel();
}


//int LayersModel::rowCount(const QModelIndex &parent) const
//{
//    return mLayerList.rowCount();
//}





//void LayersModel::clear()
//{
//        beginResetModel();
//        mLayersList.clear();
//        endResetModel();
//}

void LayersModel::toggleLayerEnabled(int layerIndex)
{

    //        qDebug() <<"index:"<<  layerIndex<< "depth" << depth;
    //        if (layerIndex /*< mLayersList.size()*/) {
    //    auto layer = mMapItem->getMapNode()->getMap()->getLayerAt(layerIndex);
    //    osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
    //    auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    //    if (visibleLayer) {
    //        visibleLayer->setVisible(!visibleLayer->getVisible());
    //    } else {
    //        layer->setEnabled(!layer->getEnabled());
    //    }
    //    emit dataChanged(index(layerIndex,0),
    //                     index(layerIndex,0),
    //                     {LayerEnabledRole});
    //        }
}

void LayersModel::clickedItem(QModelIndex itemIndex)
{
    qDebug() << itemIndex.row() << itemIndex<<"name:"<< data(itemIndex);

    auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(data(itemIndex).toString().toStdString());
    auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    if(visibleLayer)
    {
        bool visible = visibleLayer->getVisible();
        visibleLayer->setVisible(!visible);
        auto node = visibleLayer->getNode();
        if(node)
            node->setNodeMask(!visible);
    }
    else{
        auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(data(itemIndex.parent()).toString().toStdString());
        if(layer){
            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
            if(group){
                auto node = group->getChild(itemIndex.row());
                node->setNodeMask(!node->getNodeMask());
            }
        }
    }
//    osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
//    if(itemIndex.parent().row() == -1){
//        layer = mMapItem->getMapNode()->getMap()->getLayerAt(itemIndex.row());
//        auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
//        if (visibleLayer) {
//            visibleLayer->setVisible(!visibleLayer->getVisible());
//        } else {
//            layer->setEnabled(!layer->getEnabled());
//        }
//    } else{
//        layer = mMapItem->getMapNode()->getMap()->getLayerAt(itemIndex.parent().row());
//        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
//        auto node = group->getChild(itemIndex.row());
//        if (node) {
//            node->setNodeMask(!node->getNodeMask());
//        }
//    }
}

void LayersModel::addChildItem(osg::Group *parentGroup, QStandardItem *parentItem)
{
    for(int i = 0; i < parentGroup->getNumChildren(); ++i){
        osg::Node *node = parentGroup->getChild(i);
        QStandardItem *treeItem = new QStandardItem(QString(node->getName().c_str()));
        parentItem->appendRow(treeItem);

        auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(node->getName());
        if(layer && layer->getNode() && layer->getNode()->asGroup())
            addChildItem(layer->getNode()->asGroup(), treeItem);
    }
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
