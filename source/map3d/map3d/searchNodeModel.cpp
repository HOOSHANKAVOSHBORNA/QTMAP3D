#include "searchNodeModel.h"
#include "mapObject.h"
#include "serviceManager.h"
#include "filterManager.h"
#include <osgEarth/ModelLayer>
#include <osg/Node>
#include <osgEarthAnnotation/GeoPositionNode>

SearchNodeModel::SearchNodeModel(MapItem *mapItem, QObject *parent):
    QAbstractListModel(parent), mMapItem(mapItem)
{
    //    init();

    connect(mMapItem->getMapObject(), &MapObject::nodeToLayerAdded, this , &SearchNodeModel::addNode);
    connect(mMapItem->getMapObject(), &MapObject::nodeFromLayerRemoved,  this , &SearchNodeModel::removeNode);
    mTypeListModel = new TypeListModel;
    mFilterManager = new FilterManager;

}

int SearchNodeModel::rowCount(const QModelIndex &parent) const
{

    return mNodes1.size();
}

QVariant SearchNodeModel::data(const QModelIndex &index, int role) const
{

    switch (role) {
    case Qt::DisplayRole:
        return QVariant::fromValue<QString>(mNodes1[index.row()]->data->name);
        break;
    case iD_:
        return mNodes1[index.row()]->data->id ;
        break;
    case type_:
        return QVariant::fromValue<QString>(mNodes1[index.row()]->data->type.toString());
    default:
        return QVariant::fromValue<QString>(QString(""));
        break;
    };
}


void SearchNodeModel::addNode(osg::Node *node, osgEarth::Layer *layer) {
    NodeData *nodeData = dynamic_cast<NodeData*>(node->getUserData());
    if (nodeData) {
        QString typeToAdd = nodeData->type.toString();
        auto typeExists = std::find(mTypeListModel->mTypes.begin(), mTypeListModel->mTypes.end(), typeToAdd);
        if (typeExists == mTypeListModel->mTypes.end()) {
            mTypeListModel->append(typeToAdd);
        }
        auto geonode = dynamic_cast<osgEarth::Annotation::GeoPositionNode*>(node);
        if (std::find(mNodes1.begin(), mNodes1.end(), new NodeInfo{geonode, nodeData}) == mNodes1.end()) {
            beginInsertRows(QModelIndex(), mNodes1.size(), mNodes1.size());
            mNodes1.push_back(new NodeInfo{geonode, nodeData});
            endInsertRows();
        }

        // ToDo: optimize it
        for (auto &field : nodeData->fieldData){
            if (field.name.toLower() == "color")
                mFilterManager->addColorFilterField(field.value.toString());
            else if (std::strcmp(field.value.typeName(), "double") == 0 || std::strcmp(field.value.typeName(), "qlonglong") == 0)
                mFilterManager->addNumFilterField(field.name);
            else if (std::strcmp(field.value.typeName(), "QString") == 0)
                mFilterManager->addStringFilterField(field.name);
        }
    }
}

void SearchNodeModel::removeNode(osg::Node *node, osgEarth::Layer *layer) {

    auto iterator = std::remove_if(
        mNodes1.begin(),
        mNodes1.end(),
        [&](NodeInfo *item) {
            return item->node == node;
        }
        );

    if (iterator != mNodes1.end()) {
        int d = std::distance(mNodes1.begin(), iterator);
        beginRemoveRows(QModelIndex(), d, d);
        mNodes1.erase(iterator);
        endRemoveRows();
        emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
    }
}



void SearchNodeModel::onNodeClicked(const QModelIndex &current) {
    // DEBUG
    qDebug() << mNodes1[current.row()]->data->name;
    qDebug() << mNodes1[current.row()]->data->latitude;
    qDebug() << mNodes1[current.row()]->data->longitude;
    qDebug() << mNodes1[current.row()]->data->altitude;
    qDebug() << mNodes1[current.row()]->data->color;
    qDebug() << mNodes1[current.row()]->data->altitude;
    // ENDDEBUG
    if (current.isValid() && current.row() >= 0
        && current.row() < static_cast<int>(mNodes1.size())) {
        osgEarth::Annotation::GeoPositionNode *node = mNodes1[current.row()]->node;
        if (node) {
            mMapItem->getCameraController()->goToPosition(
                node->getPosition(),mMapItem->getCameraController()->getViewpoint().getRange(),0);
        }
    }
}

QHash<int, QByteArray> SearchNodeModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[iD_] = "id_";
    hash[text_]="text_";
    hash[type_]="type_";
    return hash;

}

TypeListModel *SearchNodeModel::getTypeListModel() const
{
    return mTypeListModel;
}

FilterManager *SearchNodeModel::filterManager() const
{
    return mFilterManager;
}

//void SearchNodeModel::init()
//{
//    std::vector<osg::ref_ptr<osgEarth::Layer>> layers;
//    mMapItem->getMapObject()->getLayers(layers);
//    for (auto& layer : layers){
//        ParenticAnnotationLayer* l1 = dynamic_cast<ParenticAnnotationLayer*>(layer.get());
//        osgEarth::Annotation::AnnotationLayer* l2 = dynamic_cast<osgEarth::Annotation::AnnotationLayer*>(layer.get());
//        if (!(l1 || l2)){
//            mNodes.push_back(layer->getNode());
//        }
//        else{
//            if(l1)
//                for (int i = 0; i < l1->getNumberOfNodes(); ++i)
//                    mNodes.push_back(l1->getGroup()->getChild(i));
//            else
//                for (int i = 0; i < l1->getNumberOfNodes(); ++i)
//                    mNodes.push_back(l2->getGroup()->getChild(i));
//        }
//    }
//}



/////////////////////////////////////////////////////////////////ProxyModel///////////////////////////////
SearchNodeProxyModel::SearchNodeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

bool SearchNodeProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{

    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    QString dataString = index.data(SearchNodeModel::type_).toString();
    if (index.data().toString().startsWith(mFilterString, Qt::CaseInsensitive)) {
        if (std::find(myVector.begin(), myVector.end(), dataString) != myVector.end()){
            return true;
        }
        else{
            if(myVector.size()==0){
                return true;
            }
            return false;
        }
    }else if ((index.data(SearchNodeModel::iD_).toString().contains(mFilterString, Qt::CaseInsensitive))){
        if (std::find(myVector.begin(), myVector.end(), dataString) != myVector.end()){
            return true;
        }
        else{
            if(myVector.size()==0){
                return true;
            }
            return false;
        }
    }
    return false;
}

QString SearchNodeProxyModel::filterString() const
{
    return mFilterString;
}

void SearchNodeProxyModel::toggleItem(const QString &itemText) {
    auto it = std::find(myVector.begin(), myVector.end(), itemText);
    if (it == myVector.end()) {
        myVector.push_back(itemText);
    } else {
        myVector.erase(it);
    }
    invalidateFilter();
}


void SearchNodeProxyModel::setFilterString(const QString &filterString)
{
    mFilterString = filterString;
    invalidateFilter();
}

void SearchNodeProxyModel::onNodeClicked(const int current)
{
    QModelIndex mindex = mapToSource(index(current, 0));
    static_cast<SearchNodeModel*>(sourceModel())->onNodeClicked(mindex);
}

TypeListModel *SearchNodeProxyModel::getTypeListModel() const
{
    return dynamic_cast<SearchNodeModel*>(sourceModel())->getTypeListModel();
}



//----------------------------------------------
TypeListModel::TypeListModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

void TypeListModel::append(QString type)
{
    beginInsertRows(QModelIndex(), mTypes.size(), mTypes.size());
    mTypes.push_back(type);
    endInsertRows();

}

QVariant TypeListModel::data(const QModelIndex &index, int role) const
{
    return mTypes[index.row()];
}

int TypeListModel::rowCount(const QModelIndex &parent) const
{
    return mTypes.size();
}
///////////////////////////////////////////////// --------------


SearchNodeManager::SearchNodeManager()
{   
    mSearchNodeProxyModel = new SearchNodeProxyModel();
}

void SearchNodeManager::setMapItem(MapItem *mapItem)
{
    mSearchNodeProxyModel->setSourceModel(new SearchNodeModel(mapItem, this));
}

SearchNodeProxyModel *SearchNodeManager::searchNodeProxyModel() const
{
    return mSearchNodeProxyModel;
}

FilterManager* SearchNodeManager::getFilterManager() const
{
    auto i = dynamic_cast<SearchNodeModel*>(mSearchNodeProxyModel->sourceModel());
    return i->filterManager();
}

SearchNodeManager *SearchNodeManager::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(mInstance == nullptr){ mInstance = new SearchNodeManager(); }
    return mInstance;
}


