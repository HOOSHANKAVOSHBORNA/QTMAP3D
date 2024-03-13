#include "searchNodeModel.h"
#include "filterManager.h"
#include "mapObject.h"
#include "serviceManager.h"
#include "userManager.h"
#include <osg/Node>
#include <osgEarth/ModelLayer>
#include <osgEarthAnnotation/GeoPositionNode>

SearchNodeModel::SearchNodeModel(MapItem *mapItem, QObject *parent)
    : QAbstractListModel(parent)
    , mMapItem(mapItem)
{
    //    init();
    connect(mMapItem->getMapObject(), &MapObject::nodeToLayerAdded, this, &SearchNodeModel::addNode);
    connect(mMapItem->getMapObject(),
            &MapObject::nodeFromLayerRemoved,
            this,
            &SearchNodeModel::removeNode);
    mTypeListModel = new TypeListModel(this);
    mFilterManager = new FilterManager(this);
    connect(mFilterManager , &FilterManager::filterTagsEdited , this , &SearchNodeModel::onTagEdited);
    revokeSettings();
}

SearchNodeModel::~SearchNodeModel()
{
    qDebug() << "~SearchNodeModel()";
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
        return mNodes1[index.row()]->data->id;
        break;
    case type_:
        return QVariant::fromValue<QString>(mNodes1[index.row()]->data->type.toString());
    default:
        return QVariant::fromValue<QString>(QString(""));
        break;
    };
}

void SearchNodeModel::addNode(osg::Node *node, osgEarth::Layer *layer)
{
    NodeData *nodeData = dynamic_cast<NodeData *>(node->getUserData());
    if (nodeData) {
        QString typeToAdd = nodeData->type.toString();
        auto typeExists = std::find(mTypeListModel->mTypes.begin(),
                                    mTypeListModel->mTypes.end(),
                                    typeToAdd);
        if (typeExists == mTypeListModel->mTypes.end()) {
            mTypeListModel->append(typeToAdd);
        }
        auto geonode = dynamic_cast<osgEarth::Annotation::GeoPositionNode *>(node);
        if (std::find(mNodes1.begin(), mNodes1.end(), new NodeInfo{geonode, nodeData})
            == mNodes1.end()) {
            beginInsertRows(QModelIndex(), mNodes1.size(), mNodes1.size());
            mNodes1.push_back(new NodeInfo{geonode, nodeData});
            endInsertRows();
        }

        // ToDo: optimize it
        mFilterManager->addFilterField(*nodeData);
    }
}

void SearchNodeModel::removeNode(osg::Node *node, osgEarth::Layer *layer)
{
    auto iterator = std::remove_if(mNodes1.begin(), mNodes1.end(), [&](NodeInfo *item) {
        return item->node == node;
    });

    if (iterator != mNodes1.end()) {
        int d = std::distance(mNodes1.begin(), iterator);
        beginRemoveRows(QModelIndex(), d, d);
        mNodes1.erase(iterator);
        endRemoveRows();
        emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0));
    }
}

void SearchNodeModel::onTagEdited()
{
    QVector<Tag *> tags = mFilterManager->getFilterTags();
    mFilterSettings->remove("filter/");
    for (int var = 0; var < tags.count(); ++var) {
        QVariantList list;
        list.insert(0,tags[var]->value);
        list.insert(1,tags[var]->comparision);
        list.insert(2,tags[var]->logicalOperator);
        mFilterSettings->setValue("filter/" + tags[var]->field,list);
    }
}

void SearchNodeModel::onNodeClicked(const QModelIndex &current)
{
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
            mMapItem->getCameraController()
                ->goToPosition(node->getPosition(),
                               mMapItem->getCameraController()->getViewpoint().getRange(),
                               0);
        }
    }
}

QHash<int, QByteArray> SearchNodeModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[iD_] = "id_";
    hash[text_] = "text_";
    hash[type_] = "type_";
    return hash;
}

TypeListModel *SearchNodeModel::getTypeListModel() const
{
    return mTypeListModel;
}

void SearchNodeModel::revokeSettings()
{
    mFilterSettings = new QSettings("Map3D",UserManager::instance()->userName());
    mFilterSettings->beginGroup("filter");
    int groupKeysCount = mFilterSettings->childKeys().count();
    for (int var = 0; var < groupKeysCount; ++var) {
        QString key = mFilterSettings->childKeys().at(var);
        QVariantList data = mFilterSettings->value(key).toList();
        Tag::LogicalOperator op;
        if(data[2] == 1){
            op = Tag::Or;
        }else{
            op = Tag::And;
        }
        mFilterManager->addFilterTag(key,data[0],data[1].toString(),op);
        mFilterManager->getFilterTagAt(var)->isEnabled = false;
    }
    mFilterSettings->endGroup();
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

SearchNodeProxyModel::~SearchNodeProxyModel()
{
    qDebug() << "~SearchNodeProxyModel()";
}

bool SearchNodeProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    QString dataString = index.data(SearchNodeModel::type_).toString();
    if (index.data().toString().startsWith(mFilterString, Qt::CaseInsensitive)) {
        if (std::find(myVector.begin(), myVector.end(), dataString) != myVector.end()) {
            return true;
        } else {
            if (myVector.size() == 0) {
                return true;
            }
            return false;
        }
    } else if ((index.data(SearchNodeModel::iD_)
                    .toString()
                    .contains(mFilterString, Qt::CaseInsensitive))) {
        if (std::find(myVector.begin(), myVector.end(), dataString) != myVector.end()) {
            return true;
        } else {
            if (myVector.size() == 0) {
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

void SearchNodeProxyModel::toggleItem(const QString &itemText)
{
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
    static_cast<SearchNodeModel *>(sourceModel())->onNodeClicked(mindex);
}

TypeListModel *SearchNodeProxyModel::getTypeListModel() const
{
    return dynamic_cast<SearchNodeModel *>(sourceModel())->getTypeListModel();
}

//----------------------------------------------
TypeListModel::TypeListModel(QObject *parent)
    : QAbstractListModel(parent)
{}

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

// -------------------------------------------------------------------------- manager
SearchNodeManager::SearchNodeManager(MapItem *mapItem)
    : QObject(mapItem)
{
    mSearchNodeProxyModel = new SearchNodeProxyModel(this);
    setMapItem(mapItem);
}

SearchNodeManager::~SearchNodeManager()
{
    qDebug() << "~SearchNodeManager()";
}

void SearchNodeManager::setMapItem(MapItem *mapItem)
{
    mSearchNodeProxyModel->setSourceModel(new SearchNodeModel(mapItem, this));
}

SearchNodeProxyModel *SearchNodeManager::searchNodeProxyModel() const
{
    return mSearchNodeProxyModel;
}

FilterManager *SearchNodeManager::getFilterManager() const
{
    auto i = dynamic_cast<SearchNodeModel *>(mSearchNodeProxyModel->sourceModel());
    return i->filterManager();
}
