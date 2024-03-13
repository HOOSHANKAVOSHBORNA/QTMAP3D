#include <QColor>
#include <QDebug>
#include <QPixmap>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QTimer>

#include "nodeList.h"


CategoryTagModel::CategoryTagModel(DataManager *dataManager, QObject *parent)
    :QAbstractListModel(parent)
{
    mDataManager = dataManager;

    connect(dataManager,
            &DataManager::categoryTagAppendingStart,
            this,
            &CategoryTagModel::beginInsertRows);
    connect(dataManager,
            &DataManager::categoryTagAppendingEnd,
            this,
            &CategoryTagModel::endInsertRows);
}

CategoryTagModel::~CategoryTagModel()
{
    qDebug()<<"~CategoryTagModel";
}

int CategoryTagModel::rowCount(const QModelIndex &parent) const
{
    return mDataManager ? mDataManager->categoryTagNames().size(): 0;
}

QVariant CategoryTagModel::data(const QModelIndex &index, int role) const
{
    return mDataManager ? mDataManager->categoryTagNames().at(index.row()): QVariant();
}

void CategoryTagModel::beginEndResetModel()
{
    beginResetModel();
    endResetModel();
}

//--CategoryTabbarModel------------------------------------------------------
CategoryTabbarModel::CategoryTabbarModel(DataManager *dataManager, QObject *parent)
    :QAbstractListModel(parent)
{
    mDataManager = dataManager;

    connect(dataManager,
            &DataManager::tabNameAppendingStart,
            this,
            &CategoryTabbarModel::beginInsertRows);
    connect(dataManager,
            &DataManager::tabNameAppendingEnd,
            this,
            &CategoryTabbarModel::endInsertRows);
}

CategoryTabbarModel::~CategoryTabbarModel()
{
    qDebug()<<"~CategoryTabbarModel";
}

int CategoryTabbarModel::rowCount(const QModelIndex &parent) const
{
    return mDataManager ? mDataManager->getUniqueCategoryNames()->size(): 0;
}

QVariant CategoryTabbarModel::data(const QModelIndex &index, int role) const
{
    return mDataManager ? mDataManager->getUniqueCategoryNames()->at(index.row()): QVariant();
}

void CategoryTabbarModel::beginEndResetModel()
{
    beginResetModel();
    endResetModel();
}

//-----------------------------------------NodeListModel------------------------------------
NodeListModel::NodeListModel(DataManager *dataManager, QObject *parent)
    :QAbstractTableModel(parent)
{
    mDataManager = dataManager;

    mSelectionModel = new QItemSelectionModel(this);
}

NodeListModel::~NodeListModel()
{
    qDebug()<<"~NodeListModel";
}

int NodeListModel::rowCount(const QModelIndex &) const
{
    return mDataManager ? mDataManager->nodeCount(): 0;
}

int NodeListModel::columnCount(const QModelIndex &) const
{
    return mDataManager ? (mDataManager->fixedColumnNames().size() + mDataManager->uniqueAddedColumnNames().size()
                           + mDataManager->essentialColumnNames().size()): 0;
}

// BOOKMARK: maindata
QVariant NodeListModel::data(const QModelIndex &index, int role) const
{
    if(!mDataManager)
        return QVariant();

    NodeData nodeData = mDataManager->getNodeAtIndex(index.row())->nodeData();

    QString columnName;

    if (index.column() == 0 && role != Qt::BackgroundRole) {
        return QVariant("notType");
    }

    if (index.column() == 1 && role != Qt::DecorationRole) {
        return QVariant("notType");
    }

    if (index.column() > 1 && role != Qt::DisplayRole) {
        return QVariant("notType");
    }

    if (index.column() == 0) {
        return nodeData.color;
    } else if (index.column() == 1) {
        return nodeData.iconInfoUrl;
    } else if (index.column() == 2) {
        return nodeData.id;
    } else if (index.column() == 3) {
        return nodeData.name;
    } else if (index.column() == 4) {
        return nodeData.type.toString();
    } else if (index.column() == 5) {
        return nodeData.isAttacker;
    } else if (index.column() == 6) {
        return QString::number(nodeData.latitude, 'f', 2).toDouble();
    } else if (index.column() == 7) {
        return QString::number(nodeData.longitude, 'f', 2).toDouble();
    } else if (index.column() == 8) {
        return QString::number(nodeData.altitude, 'f', 2).toDouble();
    } else if (index.column() == 9) {
        return nodeData.speed;
    } else if (index.column() > 9) {
        columnName = mDataManager->uniqueAddedColumnNames().at(
            index.column() - mDataManager->fixedColumnNames().size()
            - mDataManager->essentialColumnNames().size());

        // TODO; use from find_if not for loop
        int foundIndex = -1;
        for (int i = 0; i < nodeData.fieldData.size(); ++i) {
            if (nodeData.fieldData.at(i).name == columnName) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex == -1) {
            return "NaN";
        } else {
            QString type = nodeData.fieldData.at(foundIndex).value.typeName();
            if (type == "double") {
                return QString::number(nodeData.fieldData.at(foundIndex).value.toDouble(), 'f', 2)
                    .toDouble();
            }
            return nodeData.fieldData.at(foundIndex).value;
        }
    }

    return "BADINDEX";
}

QVariant NodeListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(!mDataManager)
        return QVariant();
    //    return "SomeColumn";
    if (section < mDataManager->fixedColumnNames().size()) {
        return mDataManager->fixedColumnNames().at(section);
    } else if (section < mDataManager->fixedColumnNames().size()
                             + mDataManager->essentialColumnNames().size()) {
        return mDataManager->essentialColumnNames().at(section
                                                       - mDataManager->fixedColumnNames().size());
    } else {
        return mDataManager->uniqueAddedColumnNames().at(
            section - mDataManager->fixedColumnNames().size()
            - mDataManager->essentialColumnNames().size());
    }
}

QHash<int, QByteArray> NodeListModel::roleNames() const
{
    return {{Qt::DisplayRole, "display"},
            {Qt::BackgroundRole, "background"},
            {Qt::DecorationRole, "decorate"},
            {Qt::EditRole, "editRole"}};
}

void NodeListModel::selectionRow(int Row, int Column)
{
    //selectionModel->clear();
    //QItemSelectionModel selectionModel;
    //selectionModel->select(index(0,2), QItemSelectionModel::ClearAndSelect);
    mSelectionModel->select(index(0, 0),
                            QItemSelectionModel::Clear | QItemSelectionModel::SelectCurrent
                                | QItemSelectionModel::Rows);
    // for (int i = 0; i < Column; i++) {
    //     selectionModel->select(index(1, i), QItemSelectionModel::Select);
    // }
}

QItemSelectionModel *NodeListModel::selectModel()
{
    return mSelectionModel;
}

DataManager *NodeListModel::dataManager() const
{
    return mDataManager;
}

void NodeListModel::onNodeUpdated(int index)
{
    emit dataChanged(this->index(index, 0), this->index(index, 0));
}

void NodeListModel::beginEndResetModel()
{
    beginResetModel();
    endResetModel();
}

//--------------------------------------NodeProxyModel-------------------------------------
NodeProxyModel::NodeProxyModel(DataManager *dataManager, QObject *parent)
    :QSortFilterProxyModel(parent)
{
    mDataManager = dataManager;
    connect(mDataManager->filterManager(),&FilterManager::filterTagsEdited,this,&NodeProxyModel::onTagsEdited);
    revokeSettings();
}

NodeProxyModel::~NodeProxyModel()
{
    qDebug()<<"~NodeProxyModel";
}

void NodeProxyModel::invalidateRowFilterInvoker()
{
    invalidateRowsFilter();
}

void NodeProxyModel::invalidateColumnFilterInvoker()
{
    invalidateColumnsFilter();
}

void NodeProxyModel::sortTable(int column)
{
    if (mAscending) {
        sort(column, Qt::AscendingOrder);
        mAscending = false;
    } else if (!mAscending) {
        sort(column, Qt::DescendingOrder);
        mAscending = true;
    }
    invalidateFilter();
}

void NodeProxyModel::setFilterColumn(QString tabName)
{
    mFilterColumn = tabName;
    invalidateFilter();
}

void NodeProxyModel::setFilterCategoryTag(QString name)
{
    mFilterCategoryTag = name;
    invalidateFilter();
}

void NodeProxyModel::selectionRow(int Row, int Column)
{
    dynamic_cast<NodeListModel *>(sourceModel())->selectionRow(Row, Column);
}

QItemSelectionModel *NodeProxyModel::selectModel()
{
    return dynamic_cast<NodeListModel *>(sourceModel())->selectModel();
}

CategoryTabbarModel *NodeProxyModel::tabbarModel() const
{
    return mTabbarModel;
}

void NodeProxyModel::setTabbarModel(CategoryTabbarModel *newTabbarModel)
{
    mTabbarModel = newTabbarModel;
}

CategoryTagModel *NodeProxyModel::categoryTagModel() const
{
    return mCategoryTagModel;
}

void NodeProxyModel::setCategoryTagModel(CategoryTagModel *newCategoryTagModel)
{
    if (mCategoryTagModel == newCategoryTagModel)
        return;
    mCategoryTagModel = newCategoryTagModel;
    emit categoryTagModelChanged();
}

void NodeProxyModel::goToPosition(int index)
{
    if(!mDataManager)
        return;

    SimpleModelNode *node = mDataManager->getNodeAtIndex(mapToSource(this->index(index, 0)).row());
    mDataManager->mapItem()->getCameraController()->goToPosition(node->getPosition(), 800);
}

void NodeProxyModel::trackPosition(int index)
{
    if(!mDataManager)
        return;

    SimpleModelNode *node = mDataManager->getNodeAtIndex(mapToSource(this->index(index, 0)).row());
    mDataManager->mapItem()->getCameraController()->setTrackNode(node->getGeoTransform(), 100);
}

bool NodeProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);

    QVariant rightData = sourceModel()->data(right);
    return leftData.toString() < rightData.toString();
}

bool NodeProxyModel::filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const
{
    // fixed columns
    if (sourceColumn >= 0 && sourceColumn <= 3) {
        return true;
    }

    // essential columns
    if (mFilterColumn == "Essential" && sourceColumn >= 4 && sourceColumn <= 9) {
        return true;
    }

//    DataManager *dataManager = dynamic_cast<NodeListModel *>(sourceModel())->dataManager();
    QString currentColumnName = sourceModel()->headerData(sourceColumn, Qt::Horizontal).toString();
    QString currentColumnCategory = mDataManager->columnToCategory().value(currentColumnName,
                                                                          "NotFound");

    return mFilterColumn == currentColumnCategory;
}

bool NodeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    NodeData nodeData = mDataManager->getNodeAtIndex(sourceRow)->nodeData();

    bool categoryFlag = (mFilterCategoryTag == "All") ? true:(nodeData.category.contains(mFilterCategoryTag, Qt::CaseInsensitive));
    bool filterFlag = mDataManager->filterManager()->checkNodeToShow(&nodeData);
    bool searchFlag = (mFilterSearch == "") ? true: nodeData.name.contains(mFilterSearch, Qt::CaseInsensitive);

    if(categoryFlag && filterFlag && searchFlag)
        return true;

    return false;
}

void NodeProxyModel::revokeSettings()
{
    mlistSettings = new QSettings("Map3D",UserManager::instance()->userName());
    mlistSettings->beginGroup("list");
    int groupKeysCount = mlistSettings->childKeys().count();
    for (int var = 0; var < groupKeysCount; ++var) {
        QString key = mlistSettings->childKeys().at(var);
        QVariantList data = mlistSettings->value(key).toList();
        Tag::LogicalOperator op;
        if(data[2] == 1){
            op = Tag::Or;
        }else{
            op = Tag::And;
        }
        mDataManager->filterManager()->addFilterTag(key,data[0],data[1].toString(),op);
        mDataManager->filterManager()->getFilterTagAt(var)->isEnabled = false;
    }
    mlistSettings->endGroup();
}

QString NodeProxyModel::filterSearch() const
{
    return mFilterSearch;
}

void NodeProxyModel::setFilterSearch(const QString &newFilterSearch)
{
    mFilterSearch = newFilterSearch;
}

void NodeProxyModel::onTagsEdited()
{
    QVector<Tag *> tags = mDataManager->filterManager()->getFilterTags();
    mlistSettings->remove("list/");
    for (int var = 0; var < tags.count(); ++var) {
        QVariantList list;
        list.insert(0,tags[var]->value);
        list.insert(1,tags[var]->comparision);
        list.insert(2,tags[var]->logicalOperator);
        mlistSettings->setValue("list/" + tags[var]->field,list);
    }
}



//--------------------------------NodeList-----------------------
NodeList::NodeList(QQmlEngine *engine, MapControllerItem *mapItem, DataManager *dataManager, QObject *parent)
    :QObject(parent)
{
    mQmlEngine = engine;
    mMapItem = mapItem;
    mDataManager = dataManager;

    NodeListModel *nodeModel = new NodeListModel(dataManager, this);

    mProxyModel = new NodeProxyModel(dataManager, this);
    mProxyModel->setSourceModel(nodeModel);

    CategoryTabbarModel *tabbarModel = new CategoryTabbarModel(dataManager, this);
    mProxyModel->setTabbarModel(tabbarModel);

    CategoryTagModel *categoryTagsModel = new CategoryTagModel(dataManager, this);
    mProxyModel->setCategoryTagModel(categoryTagsModel);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, nodeModel, &NodeListModel::beginEndResetModel);
    connect(timer, &QTimer::timeout, tabbarModel, &CategoryTabbarModel::beginEndResetModel);
    connect(timer, &QTimer::timeout, categoryTagsModel, &CategoryTagModel::beginEndResetModel);
    connect(timer, &QTimer::timeout, mProxyModel, &NodeProxyModel::invalidateRowFilterInvoker);
    timer->start(1000);

    // connections
    //    connect(mDataManager,
    //            &DataManager::nodeAppendingStart,
    //            nodeModel,
    //            &NodeListModel::beginInsertRows);
    //    connect(mDataManager, &DataManager::nodeAppendingEnd, nodeModel, &NodeListModel::endInsertRows);
    //    connect(mDataManager,
    //            &DataManager::nodeAppendingEnd,
    //            mProxyModel,
    //            &NodeProxyModel::invalidateRowFilterInvoker);

    //    connect(mDataManager,
    //            &DataManager::nodeRemovingStart,
    //            nodeModel,
    //            &NodeListModel::beginRemoveRows);
    //    connect(mDataManager, &DataManager::nodeRemovingEnd, nodeModel, &NodeListModel::endRemoveRows);
    //    connect(mDataManager,
    //            &DataManager::nodeRemovingEnd,
    //            mProxyModel,
    //            &NodeProxyModel::invalidateRowFilterInvoker);

    //    connect(mDataManager, &DataManager::nodeUpdated, nodeModel, &NodeListModel::onNodeUpdated);

    //    connect(mDataManager,
    //            &DataManager::columnAppendigStart,
    //            nodeModel,
    //            &NodeListModel::beginInsertColumns);
    //    connect(mDataManager,
    //            &DataManager::columnAppendigEnd,
    //            nodeModel,
    //            &NodeListModel::endInsertColumns);
    //    connect(mDataManager,
    //            &DataManager::columnAppendigEnd,
    //            mProxyModel,
    //            &NodeProxyModel::invalidateColumnFilterInvoker);

    createQml();
}

QQuickItem *NodeList::qmlItem() const
{
    return mQmlItem;
}

void NodeList::setQmlItem(QQuickItem *newQmlItem)
{
    if (mQmlItem == newQmlItem)
        return;
    mQmlItem = newQmlItem;
    emit qmlItemChanged();
}

NodeProxyModel *NodeList::proxyModel() const
{
    return mProxyModel;
}

void NodeList::setProxyModel(NodeProxyModel *newProxyModel)
{
    mProxyModel = newProxyModel;
}

void NodeList::createQml()
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine, this);

    connect(comp, &QQmlComponent::statusChanged, this, [&] {
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }

        mQmlItem = qobject_cast<QQuickItem *>(comp->create());
        mQmlItem->setProperty("tableModel", QVariant::fromValue(mProxyModel));
        mQmlItem->setProperty("filterManager", QVariant::fromValue(mDataManager->filterManager()));
    });

    comp->loadUrl(QUrl("qrc:/NodeListItem.qml"));
}
