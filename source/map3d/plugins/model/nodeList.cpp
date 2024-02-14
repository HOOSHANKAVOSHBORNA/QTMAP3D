#include <QColor>
#include <QDebug>
#include <QPixmap>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QTimer>

#include "nodeList.h"

//--------------------------------NodeList-----------------------
NodeList::NodeList(QQmlEngine *engine, MapControllerItem *mapItem, DataManager *dataManager)
{
    mQmlEngine = engine;
    mMapItem = mapItem;
    mDataManager = dataManager;

    NodeListModel *nodeModel = new NodeListModel(dataManager);

    mProxyModel = new NodeProxyModel(dataManager);
    mProxyModel->setSourceModel(nodeModel);

    CategoryTabbarModel *tabbarModel = new CategoryTabbarModel(dataManager);
    mProxyModel->setTabbarModel(tabbarModel);

    CategoryTagModel *categoryTagsModel = new CategoryTagModel(dataManager);
    mProxyModel->setCategoryTagModel(categoryTagsModel);

    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, nodeModel, &NodeListModel::beginEndResetModel);
    //    connect(timer, &QTimer::timeout, tabbarModel, &CategoryTabbarModel::beginEndResetModel);
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

void NodeList::createQml()
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);

    connect(comp, &QQmlComponent::statusChanged, [&] {
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }

        mQmlItem = qobject_cast<QQuickItem *>(comp->create());
        mQmlItem->setProperty("tableModel", QVariant::fromValue(mProxyModel));
        mQmlItem->setProperty("filterManager", QVariant::fromValue(mDataManager->filterManager()));
    });

    comp->loadUrl(QUrl("qrc:/NodeListItem.qml"));
}

NodeProxyModel *NodeList::proxyModel() const
{
    return mProxyModel;
}

void NodeList::setProxyModel(NodeProxyModel *newProxyModel)
{
    mProxyModel = newProxyModel;
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

//--------------------------------------NodeProxyModel-------------------------------------
NodeProxyModel::NodeProxyModel(DataManager *dataManager)
{
    mDataManager = dataManager;
}

void NodeProxyModel::invalidateRowFilterInvoker()
{
    invalidateRowsFilter();
}

void NodeProxyModel::invalidateColumnFilterInvoker()
{
    invalidateColumnsFilter();
}

bool NodeProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);

    QVariant rightData = sourceModel()->data(right);
    return leftData.toString() < rightData.toString();
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

    DataManager *dataManager = dynamic_cast<NodeListModel *>(sourceModel())->dataManager();
    QString currentColumnName = sourceModel()->headerData(sourceColumn, Qt::Horizontal).toString();
    QString currentColumnCategory = dataManager->columnToCategory().value(currentColumnName,
                                                                          "NotFound");

    return mFilterColumn == currentColumnCategory;
}

bool NodeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    //    DataManager *dataManager = dynamic_cast<NodeListModel *>(sourceModel())->dataManager();
    //    QString categoryTab = dataManager->categoryTagNames().at(ifDataFromQmlIsIndexNotString);
    NodeData nodeData = mDataManager->getNodeAtIndex(sourceRow)->nodeData();
    if (!mDataManager->filterManager()->checkNodeToShow(&nodeData)) {
        return false;
    }

    if (mFilterCategoryTag == "All") {
        return true;
    }

    DataManager *dataManager = dynamic_cast<NodeListModel *>(sourceModel())->dataManager();
    QString rowCategory = dataManager->getNodeAtIndex(sourceRow)->nodeData().category;

    return rowCategory.contains(mFilterCategoryTag, Qt::CaseInsensitive);
}

CategoryTabbarModel *NodeProxyModel::tabbarModel() const
{
    return mTabbarModel;
}

void NodeProxyModel::setTabbarModel(CategoryTabbarModel *newTabbarModel)
{
    mTabbarModel = newTabbarModel;
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

//-----------------------------------------NodeListModel------------------------------------
NodeListModel::NodeListModel(DataManager *dataManager)
{
    mDataManager = dataManager;

    mSelectionModel = new QItemSelectionModel();
}

int NodeListModel::rowCount(const QModelIndex &) const
{
    return mDataManager->nodeCount();
}

int NodeListModel::columnCount(const QModelIndex &) const
{
    return mDataManager->fixedColumnNames().size() + mDataManager->uniqueAddedColumnNames().size()
           + mDataManager->essentialColumnNames().size();
}

// BOOKMARK: maindata
QVariant NodeListModel::data(const QModelIndex &index, int role) const
{
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

void NodeListModel::setDataManager(DataManager *newDataManager)
{
    mDataManager = newDataManager;
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

QHash<int, QByteArray> NodeListModel::roleNames() const
{
    return {{Qt::DisplayRole, "display"},
            {Qt::BackgroundRole, "background"},
            {Qt::DecorationRole, "decorate"},
            {Qt::EditRole, "editRole"}};
}

CategoryTabbarModel::CategoryTabbarModel(DataManager *dataManager)
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

int CategoryTabbarModel::rowCount(const QModelIndex &parent) const
{
    return mDataManager->getUniqueCategoryNames()->size();
}

QVariant CategoryTabbarModel::data(const QModelIndex &index, int role) const
{
    return mDataManager->getUniqueCategoryNames()->at(index.row());
}

void CategoryTabbarModel::beginEndResetModel()
{
    beginResetModel();
    endResetModel();
}

CategoryTagModel::CategoryTagModel(DataManager *dataManager)
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

int CategoryTagModel::rowCount(const QModelIndex &parent) const
{
    return mDataManager->categoryTagNames().size();
}

QVariant CategoryTagModel::data(const QModelIndex &index, int role) const
{
    return mDataManager->categoryTagNames().at(index.row());
}

void CategoryTagModel::beginEndResetModel()
{
    beginResetModel();
    endResetModel();
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
    SimpleModelNode *node = mDataManager->getNodeAtIndex(mapToSource(this->index(index, 0)).row());
    mDataManager->mapItem()->getCameraController()->goToPosition(node->getPosition(), 500);
}

void NodeProxyModel::trackPosition(int index)
{
    SimpleModelNode *node = mDataManager->getNodeAtIndex(mapToSource(this->index(index, 0)).row());
    mDataManager->mapItem()->getCameraController()->setTrackNode(node->getGeoTransform(), 100);
}
