#include "layerModel.h"
#include <osgEarth/Map>
#include <osgEarth/ModelLayer>
#include <QHeaderView>
#include <QQmlComponent>
#include <QQuickItem>
#include <osgEarthAnnotation/AnnotationNode>
#include <osgEarthAnnotation/AnnotationNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/GeoPositionNode>


LayersModel::LayersModel(MapItem *mapItem, QObject *parent) :
    TreeProxyModel(parent)
{
    mTreeModel = new TreeModel;
    mMapItem = mapItem;

    initializeModel(mapItem->getMapNode()->getMap());
    setSourceModel(mTreeModel);
//    connect(mapItem, &MapItem::layerChanged,[this, mapItem](){
//        mTreeModel->removeRows(0,mTreeModel->rowCount());
//        initializeModel(mapItem->getMapNode()->getMap());
//    });
    connect(mapItem, &MapItem::mapCleared,[this, mapItem](){
        mTreeModel->removeRows(0,mTreeModel->rowCount());
        initializeModel(mapItem->getMapNode()->getMap());
    });
    connect(mapItem->getMapObject(), &MapObject::layerAdded,this ,&LayersModel::onLayerAdded);
    connect(mapItem->getMapObject(), &MapObject::layerRemoved,this ,&LayersModel::onLayerRemoved);
    connect(mapItem->getMapObject(), &MapObject::nodeToLayerAdded,this ,&LayersModel::onNodeToLayerAdded);
    connect(mapItem->getMapObject(), &MapObject::nodeFromLayerRemoved,this ,&LayersModel::onNodeFromLayerRemoved);
    connect(mapItem->getMapObject(), &MapObject::parentLayerChanged,this ,&LayersModel::onParentLayerChanged);
}


void LayersModel::initializeModel(osgEarth::Map *map)
{
    osgEarth::LayerVector layers;
    //    map.
    //    QStandardItem *rootItem = mTreeModel->getRootItem();
    //    osgEarth::Layer rootLayer = mMapItem->getRoot();
    //    QVariant rootLayerVariant;
    //    rootLayerVariant.setValue(rootLayer);
    //    rootItem->setData(rootLayerVariant,layerRole);
    map->getLayers(layers);
    for(const auto& layer : layers) {
        //        auto parentLayer = mMapItem->getMapObject()->getParentLayer(layer);
        onLayerAdded(layer, nullptr, map->getIndexOfLayer(layer));
    }
}


QHash<int, QByteArray> LayersModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[visibleRole] = "isVisible";
    hash[locatableRole] = "isLocatable";
    hash[layerRole] = "layerRole";
    hash[visibleDrop] = "isDropVisible";
    return hash;
}


void LayersModel::onItemClicked(const QModelIndex &current)
{
    QModelIndex indexSource = mapToSource(current);
    //    osgEarth::Layer *layerData = mTreeModel->data(indexSource,layerRole).value<osgEarth::Layer*>();
    //    if(layerData)
    //        qDebug()<<layerData->getName() << " " << layerData->getTypeName();
    //    bool locata = mTreeModel->data(indexSource , locatableRole).toBool();
    //    qDebug() << locata ;
    bool visibleRoleData = mTreeModel->data(indexSource,visibleRole).toBool();
    mTreeModel->updateData(indexSource,!visibleRoleData,visibleRole);
    auto layer = data(current, layerRole).value<osgEarth::Layer*>();
    auto visibleLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    if(visibleLayer)
    {
        setLayerVisible(visibleLayer);
    }
    //    else{
    //        auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(data(current.parent()).toString().toStdString());
    //        if(layer){
    //            osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
    //            if(group){
    //                auto node = group->getChild(current.row());
    //                node->setNodeMask(!node->getNodeMask());
    //            }
    //        }
    //    }
}

void LayersModel::onDeleteLayerClicked(const QModelIndex &current)
{
    QModelIndex indexSource = mapToSource(current);
    QStandardItem *item =  mTreeModel->itemFromIndex(indexSource);
    QStandardItem *parentItem = item->parent();
    if(!parentItem){
        parentItem = mTreeModel->getRootItem();
    }
    osgEarth::Layer *itemLayer = item->data(layerRole).value<osgEarth::Layer*>();
    osgEarth::Layer *parentLayer = parentItem->data(layerRole).value<osgEarth::Layer*>();

    //    QString layerItem = item->text();
    //    QString parentLayerItem = item->parent()->text();
    //    mTreeModel->removeItem(layerItem , parentLayerItem);
    //    auto layer = data(current, layerRole).value<osgEarth::Layer*>();
    ParenticAnnotationLayer *layerParentic = dynamic_cast<ParenticAnnotationLayer*>(itemLayer);
    if (layerParentic){
        auto parentAnoLayer = dynamic_cast<CompositeAnnotationLayer*>(parentLayer);
        auto parenticItemLayer = dynamic_cast<ParenticAnnotationLayer*>(itemLayer);
        parentAnoLayer->removeLayer(parenticItemLayer);
    }else{
        //        auto layer = mMapItem->getMapNode()->getMap()->getLayerByName(data(current).toString().toStdString());
        mMapItem->getMapObject()->removeLayer(itemLayer,parentLayer);
    }


}

void LayersModel::onGoToClicked(const QModelIndex &current)
{
    QModelIndex indexSource = mapToSource(current);
    QStandardItem *item =  mTreeModel->itemFromIndex(indexSource);
    QString layerItem = item->text();
    QString parentLayerItem = item->parent()->text();
    QString grandpaLayerItem = item->parent()->parent()->text();
    auto grandpaLayer = mMapItem->getMapNode()->getMap()->getLayerByName(grandpaLayerItem.toStdString());
    CompositeAnnotationLayer *grandpaCompositeLayer = dynamic_cast<CompositeAnnotationLayer*>(grandpaLayer);
    osgEarth::Annotation::AnnotationLayer *parentAnnotationlayer = dynamic_cast<osgEarth::Annotation::AnnotationLayer*>(grandpaCompositeLayer->getLayerByName(parentLayerItem));
    for(int i=0 ;i < parentAnnotationlayer->getGroup()->getNumChildren(); i++ ){
        if(parentAnnotationlayer->getGroup()->getChild(i)->getName() == layerItem.toStdString()){
            auto layer = parentAnnotationlayer->getGroup()->getChild(i);
            auto anotationNode = dynamic_cast<osgEarth::Annotation::GeoPositionNode*>(layer);
            osgEarth::GeoPoint location(mMapItem->getMapSRS(), 0,0,0);
            if(anotationNode)
                location = anotationNode->getPosition();
            else{
                auto featureNode = dynamic_cast<osgEarth::Annotation::FeatureNode*>(layer);
                location.vec3d() = featureNode->getFeature()->getGeometry()->at(0);
            }
            mMapItem->getCameraController()->goToPosition(location,mMapItem->getCameraController()->getDistance(),1.0);
        }
    }
}



void LayersModel::onLayerAdded(osgEarth::Layer *layer , osgEarth::Layer *parentLayer , unsigned index)
{
    QStandardItem *treeItem = new QStandardItem(QString(layer->getName().c_str()));
    treeItem->setData(getLayerVisible(layer),visibleRole);
    treeItem->setData(false,visibleDrop);

    QVariant layerVariant;
    layerVariant.setValue(layer);
    treeItem->setData(layerVariant,layerRole);

    ParenticAnnotationLayer* checkParentic = dynamic_cast<ParenticAnnotationLayer*>(layer);
    if(checkParentic && !checkParentic->asCompositeAnnotationLayer()){
        treeItem->setData(true,locatableRole);
    }else{
        treeItem->setData(false,locatableRole);
    }
    if(parentLayer){
        QString parentName = parentLayer->getName().c_str();
        mTreeModel->addItem(treeItem,parentName);
    }else{
        mTreeModel->addItem(treeItem);
    }
    //--------------------------------------
    //    CompositeAnnotationLayer* compositeLayer = dynamic_cast<CompositeAnnotationLayer*>(layer);
    //    if (compositeLayer){
    //        for(int i = 0; i < compositeLayer->getNumChildren(); i++)
    //            onLayerAdded(compositeLayer->getChild(i), layer, index);
    //    }
}

void LayersModel::onLayerRemoved(osgEarth::Layer *layer , osgEarth::Layer *parentLayer, unsigned index)
{
    QString treeItem = QString(layer->getName().c_str());
    if(parentLayer){
        QString parentName = parentLayer->getName().c_str();
        mTreeModel->removeItem(treeItem , parentName);
    }else{
        mTreeModel->removeItem(treeItem);
    }
}

void LayersModel::onParentLayerChanged(osgEarth::Layer *layer, osgEarth::Layer *oldParentLayer, osgEarth::Layer *newParentLayer)
{
    QString treeItem = QString(layer->getName().c_str());
    if(oldParentLayer){
        QString oldParent = QString(oldParentLayer->getName().c_str());
        mTreeModel->removeItem(treeItem , oldParent);
    }else{
        mTreeModel->removeItem(treeItem);
    }
    QStandardItem *item = new QStandardItem(treeItem);
    if(newParentLayer){
        QString newParent = QString(newParentLayer->getName().c_str());
        mTreeModel->addItem(item,newParent);
    }else{
        mTreeModel->addItem(item);
    }
}


void LayersModel::onNodeToLayerAdded(osg::Node *node, osgEarth::Layer *layer)
{
    QStandardItem *treeItem = new QStandardItem(QString(node->getName().c_str()));
    treeItem->setData(getLayerVisible(layer),visibleRole);
    QString parentLayer = layer->getName().c_str();
    mTreeModel->addItem(treeItem,parentLayer);
}

void LayersModel::onNodeFromLayerRemoved(osg::Node *node, osgEarth::Layer *layer)
{
    QString treeItem = QString(node->getName().c_str());
    QString parentLayer = QString(layer->getName().c_str());
    mTreeModel->removeItem(treeItem , parentLayer);
}

void LayersModel::setLayerVisible(osgEarth::VisibleLayer *layer)
{
    bool visible = layer->getVisible();
    layer->setVisible(!visible);
}

bool LayersModel::getLayerVisible(osgEarth::Layer *layer) const
{
    auto visLayer = dynamic_cast<osgEarth::VisibleLayer*>(layer);
    bool visible = visLayer->getVisible();
    return visible;
}

QModelIndex LayersModel::getDragIndex()
{
    return mDragIndex;
}

void LayersModel::setDragIndex(QModelIndex value)
{
//    beginResetModel();
    //--set old to false------------------------
    if(mDragIndex.isValid())
    {
        auto itemDrag = mTreeModel->itemFromIndex(mapToSource(mDragIndex));
        QStandardItem *parentItem = itemDrag->parent();
        if(!parentItem){
            parentItem = mTreeModel->getRootItem();
        }
        for(int i = 0; i < parentItem->rowCount(); i++)
        {
            parentItem->child(i, 0)->setData(false, visibleDrop);
        }
    }
    //--set new to true---------------------------
    mDragIndex = value;
    if(mDragIndex.isValid())
    {
        auto itemDrag = mTreeModel->itemFromIndex(mapToSource(mDragIndex));
        QStandardItem *parentItem = itemDrag->parent();
        if(!parentItem){
            parentItem = mTreeModel->getRootItem();
        }
        for(int i = 0; i < parentItem->rowCount(); i++)
        {
            parentItem->child(i, 0)->setData(true, visibleDrop);
        }
    }
//    endResetModel();
}

//QVariant LayersModel::data(const QModelIndex &index, int role) const
//{
////    if(index.row() == -1)
////        return QVariant();

////    if(role == visibleDrop){
////        if(mDragIndex.isValid() && index != mDragIndex){
//////            auto item = mTreeModel->itemFromIndex(index);
////            qDebug()<<" index: "<<index;
//////            qDebug()<<" indexparent: "<<index.parent();
////            qDebug()<<" dragindex: "<<mDragIndex;
//////            qDebug()<<" dragindexparent: "<<mDragIndex.parent();
//////            qDebug()<<"mapdrag: "<<mapToSource(mDragIndex);
//////            auto itemDrag = mTreeModel->itemFromIndex(mDragIndex);
//////            if(item->parent() == itemDrag->parent())
//////                return true;
////        }
////        return false;
////    }
//    return TreeProxyModel::data(index, role);
//}

void LayersModel::onMoveItem(QModelIndex oldIndex, QModelIndex newIndex)
{
    if(!oldIndex.isValid() || !newIndex.isValid())
        return;
    QModelIndex fromIndexSource = mapToSource(oldIndex);
    QModelIndex toIndexSource = mapToSource(newIndex);

    if((fromIndexSource != toIndexSource)){

        QStandardItem *item =  mTreeModel->itemFromIndex(fromIndexSource);
        osgEarth::Layer *itemLayer = item->data(layerRole).value<osgEarth::Layer*>();

        QStandardItem *parentItem = item->parent();
        if(!parentItem){
            mMapItem->getMapObject()->moveLayer(itemLayer,toIndexSource.row());
        }
        else{
            osgEarth::Layer *parentLayer = parentItem->data(layerRole).value<osgEarth::Layer*>();
            ParenticAnnotationLayer *layerParentic = dynamic_cast<ParenticAnnotationLayer*>(itemLayer);
            if (layerParentic){
                auto parentAnoLayer = dynamic_cast<CompositeAnnotationLayer*>(parentLayer);
                parentAnoLayer->moveLayer(layerParentic,toIndexSource.row());
            }
        }

        mTreeModel->moveItem(fromIndexSource,toIndexSource);
    }

}
