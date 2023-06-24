#include "layersmodel.h"
#include <osgEarth/Map>
#include <osgEarth/ModelLayer>
#include <QHeaderView>
#include <QQmlComponent>
#include <QQuickItem>


//TreeItem::TreeItem(const QList<QVariant> &data, ToolboxItem *toolbox, TreeItem *parent)
//    : m_itemData(data), m_parentItem(parent), mToolboxItem(toolbox)
//{}

//TreeItem::~TreeItem()
//{
//    qDeleteAll(m_childItems);
//}

//void TreeItem::appendChild(TreeItem *item)
//{
//    m_childItems.append(item);
//}

//TreeItem *TreeItem::child(int row)
//{
//    if (row < 0 || row >= m_childItems.size())
//        return nullptr;
//    return m_childItems.at(row);
//}

//int TreeItem::childCount() const
//{
//    return m_childItems.count();
//}

//int TreeItem::columnCount() const
//{
//    return m_itemData.count();
//}



///////////////////////////////////////////////////////////////////////////////////////////


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
        std::string parent;
        layer->getUserValue("parent", parent);
        //        qDebug()<<layer->getName()<<", "<< hasParent;
        if(parent.empty()){
            addChildItem(layer, rootItem);
            //            QStandardItem *treeItem = new QStandardItem(QString(layer->getName().c_str()));
            //            rootItem->appendRow(treeItem);
            //            osg::Node *node = layer->getNode();
            //            if(node && node->asGroup())
            //                addChildItem(node->asGroup(), treeItem);
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
        setLayerVisible(visibleLayer);
        //        auto node = visibleLayer->getNode();
        //        if(node)
        //            node->setNodeMask(!visible);
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

void LayersModel::setLayerVisible(osgEarth::VisibleLayer *layer)
{
    qDebug()<<layer->getName();
    bool visible = layer->getVisible();
    layer->setVisible(!visible);
    auto containerData = layer->getUserDataContainer();
    for(int i = 0; i < containerData->getNumUserObjects(); ++i){
        auto userObject = containerData->getUserObject(i);
        osgEarth::VisibleLayer *childLayer = dynamic_cast<osgEarth::VisibleLayer*>(userObject);
        if(childLayer){
            qDebug()<<childLayer->getName();
            setLayerVisible(childLayer);
        }
    }
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

void LayersModel::addChildItem(osgEarth::Layer *layer, QStandardItem *parentItem)
{
    QStandardItem *treeItem = new QStandardItem(QString(layer->getName().c_str()));
    parentItem->appendRow(treeItem);
    auto containerData = layer->getUserDataContainer();

//    qDebug()<<layer->getName();
//    qDebug()<<containerData->getNumUserObjects();

    for(int i = 0; i < containerData->getNumUserObjects(); ++i){
        auto userObject = containerData->getUserObject(i);
        osgEarth::Layer *childLayer = dynamic_cast<osgEarth::Layer*>(userObject);
        if(childLayer)
            addChildItem(childLayer, treeItem);
    }
    if(layer->getNode() && layer->getNode()->asGroup()){
        auto group = layer->getNode()->asGroup();
        for (int i = 0; i < group->getNumChildren(); ++i) {
            auto child = group->getChild(i);
            QStandardItem *treeItemChild = new QStandardItem(QString(child->getName().c_str()));
            treeItem->appendRow(treeItemChild);
        }

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




LayersProxyModel::LayersProxyModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

bool LayersProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    // check if item name or its children contain filter string (case insensitive)
    if (index.data().toString().contains(mFilterString, Qt::CaseInsensitive))
        return true;
    for (int i = 0; i < sourceModel()->rowCount(index); ++i) {
        if (filterAcceptsRow(i, index))
            return true;
    }
    // show its children if its name contains filter string
    if (source_parent.isValid() && source_parent.data().toString().contains(mFilterString, Qt::CaseInsensitive))
        return true;
    return false;
}

QString LayersProxyModel::filterString() const
{
    return mFilterString;
}

void LayersProxyModel::setFilterString(const QString &filterString)
{
    qDebug() << mFilterString;
    mFilterString = filterString;

    invalidateFilter();
}

void LayersProxyModel::onItemClicked(const QModelIndex &current)
{
    // call mapped item in toolbox model
    QModelIndex index = mapToSource(current);
    static_cast<LayersModel*>(sourceModel())->clickedItem(index);
}
