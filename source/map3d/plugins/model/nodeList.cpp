#include <QColor>
#include <QDebug>
#include <QPixmap>
#include <QQmlComponent>
#include <QQmlEngine>

#include "nodeList.h"

//--------------------------------NodeList-----------------------
NodeList::NodeList(MapControllerItem *mapItem, DataManager *dataManager)
{
    mMapItem = mapItem;
    mDataManager = dataManager;

    NodeListModel *nodeModel = new NodeListModel(dataManager);

    mProxyModel = new NodeProxyModel;
    mProxyModel->setSourceModel(nodeModel);

    CategoryTabbarModel *tabbarModel = new CategoryTabbarModel(dataManager);
    mProxyModel->setTabbarModel(tabbarModel);

    CategoryTagModel *categoryTagsModel = new CategoryTagModel(dataManager);
    mProxyModel->setCategoryTagModel(categoryTagsModel);

    // connections
    connect(mDataManager,
            &DataManager::nodeAppendingStart,
            nodeModel,
            &NodeListModel::beginInsertRows);
    connect(mDataManager, &DataManager::nodeAppendingEnd, nodeModel, &NodeListModel::endInsertRows);
    connect(mDataManager,
            &DataManager::nodeAppendingEnd,
            mProxyModel,
            &NodeProxyModel::invalidateRowFilterInvoker);

    connect(mDataManager,
            &DataManager::nodeRemovingStart,
            nodeModel,
            &NodeListModel::beginRemoveRows);
    connect(mDataManager, &DataManager::nodeRemovingEnd, nodeModel, &NodeListModel::endRemoveRows);
    connect(mDataManager,
            &DataManager::nodeRemovingEnd,
            mProxyModel,
            &NodeProxyModel::invalidateRowFilterInvoker);

    connect(mDataManager, &DataManager::nodeUpdated, nodeModel, &NodeListModel::onNodeUpated);

    connect(mDataManager,
            &DataManager::columnAppendigStart,
            nodeModel,
            &NodeListModel::beginInsertColumns);
    connect(mDataManager,
            &DataManager::columnAppendigEnd,
            nodeModel,
            &NodeListModel::endInsertColumns);
    connect(mDataManager,
            &DataManager::columnAppendigEnd,
            mProxyModel,
            &NodeProxyModel::invalidateColumnFilterInvoker);

    createQml();
}

void NodeList::createQml()
{
    QQmlComponent *comp = new QQmlComponent(qmlEngine(mMapItem));

    connect(comp, &QQmlComponent::statusChanged, [&] {
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }

        mQmlItem = qobject_cast<QQuickItem *>(comp->create());
        mQmlItem->setProperty("tableModel", QVariant::fromValue(mProxyModel));
    });

    //qDebug() << "it is what it is";
    comp->loadUrl(QUrl("qrc:/NodeListItem.qml"));
}

NodeProxyModel *NodeList::proxyModel() const
{
    //qDebug() << "chch";
    return mProxyModel;
}

void NodeList::setProxyModel(NodeProxyModel *newProxyModel)
{
    //qDebug() << "chchch";
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

NodeProxyModel::NodeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{}

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
    //qDebug() << leftData << rightData;
    return leftData.toString() < rightData.toString();
}

void NodeProxyModel::sortTable(int column)
{
    //qDebug() << "call sort" << column;
    if (Asc) {
        qDebug() << "Ascending Sort>>>>>";
        sort(column, Qt::AscendingOrder);
        Asc = false;
    } else if (!Asc) {
        qDebug() << "Descending Sort>>>>>";
        sort(column, Qt::DescendingOrder);
        Asc = true;
    }
    invalidateFilter();
}

bool NodeProxyModel::filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const
{
    // fixed columns
    if (sourceColumn >= 0 && sourceColumn <= 3) {
        return true;
    }

    // icon columns
    if (sourceColumn >= sourceModel()->columnCount() - 3) {
        return true;
    }

    // essential columns
    if (m_filterColumn == "Essential" && sourceColumn >= 4 && sourceColumn <= 9) {
        return true;
    }

    DataManager *dataManager = dynamic_cast<NodeListModel *>(sourceModel())->dataManager();
    QString currentColumnName = sourceModel()->headerData(sourceColumn, Qt::Horizontal).toString();
    QString currentColumnCategory = dataManager->columnToCategory().value(currentColumnName,
                                                                          "NotFound");

    // DEBUG
    //    qDebug() << "debug  " << currentColumnName << ", " << currentColumnCategory;
    // ENDDEBUG

    return m_filterColumn == currentColumnCategory;

    //    if (m_filterColumn == tabList.at(0)) { //Main
    //        //        qDebug()<<sourceModel()->headerData(sourceColumn,Qt::Horizontal);
    //        //        QModelIndex index = sourceModel()->index(0, sourceColumn, sourceParent);
    //        //        QVariant data = sourceModel()->data(index);
    //        QVariant data = sourceModel()->headerData(sourceColumn, Qt::Horizontal);
    //        bool result = false;
    //        for (int i = 0; i < columnName.size(); ++i) {
    //            if (i <= Ecolumn::EMaster || i >= Ecolumn::EBattle) {
    //                //qDebug()<<Ecolumn::EMaster;
    //                result = result || data.toString().contains(columnName.at(i), Qt::CaseInsensitive);
    //            }
    //        }
    //        return result;
    //    } else if (m_filterColumn == tabList.at(1)) { //Location
    //        QVariant data = sourceModel()->headerData(sourceColumn, Qt::Horizontal);
    //        bool result = false;
    //        for (int i = 0; i < columnName.size(); ++i) {
    //            if (i <= Ecolumn::EName || i >= Ecolumn::ELatitude) {
    //                result = result || data.toString().contains(columnName.at(i), Qt::CaseInsensitive);
    //            }
    //        }
    //        return result;
    //    }
    //    return true;
}

bool NodeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    //    DataManager *dataManager = dynamic_cast<NodeListModel *>(sourceModel())->dataManager();
    //    QString categoryTab = dataManager->categoryTagNames().at(ifDataFromQmlIsIndexNotString);

    if (mFilterType == "All") {
        return true;
    }

    DataManager *dataManager = dynamic_cast<NodeListModel *>(sourceModel())->dataManager();
    QString rowCategory = dataManager->getNodeAtIndex(sourceRow)->nodeData().category;
    return rowCategory.contains(mFilterType, Qt::CaseInsensitive);

    //search Tag all of them
    bool resultName = m_filterName.isEmpty();
    bool result1 = TagColor.isEmpty();
    bool result2 = TagFilter1.isEmpty();
    bool result3 = TagFilter2.isEmpty();
    bool result4 = TagFilter3.isEmpty();
    //    if(!TagColor.isEmpty())
    //        result1 = false;
    //    if(!TagFilter1.isEmpty())
    //        result2 = false;
    //    if(!TagFilter2.isEmpty())
    //        result3 = false;
    //    if(!TagFilter3.isEmpty())
    //        result4 = false;
    if (m_search == "filter") {
        for (int iter = 0; iter < columnName.size(); ++iter) {
            QModelIndex index = sourceModel()->index(sourceRow, iter, sourceParent);
            QVariant data = sourceModel()->data(index);
            for (int i = 0; i < TagColor.size(); ++i) {
                if (columnName.at(iter) == TagColor.at(i).name) {
                    result1 = result1
                              || data.toString().contains(TagColor.at(i).value, Qt::CaseInsensitive);
                }
            }
            for (int j = 0; j < TagFilter1.size(); ++j) {
                if (columnName.at(iter) == TagFilter1.at(j).name) {
                    //qDebug()<<TagFilter1.at(j).name;
                    result2 = result2
                              || data.toString().contains(TagFilter1.at(j).value,
                                                          Qt::CaseInsensitive);
                }
            }
            for (int i = 0; i < TagFilter2.size(); ++i) {
                if (columnName.at(iter) == TagFilter2.at(i).name) {
                    //qDebug()<< TagColor.at(i).valueFrom << TagColor.at(i).valueTo;
                    result3 = result3
                              || (data.toInt() >= TagFilter2.at(i).valueFrom
                                  && data.toInt() <= TagFilter2.at(i).valueTo);
                }
            }
            for (int i = 0; i < TagFilter3.size(); ++i) {
                if (columnName.at(iter) == TagFilter3.at(i).name) {
                    //qDebug()<<TagFilter3.at(i).mark ;
                    if (TagFilter3.at(i).mark == "<")
                        result4 = result4 || data.toInt() < TagFilter3.at(i).value;
                    else if (TagFilter3.at(i).mark == "<=")
                        result4 = result4 || data.toInt() <= TagFilter3.at(i).value;
                    else if (TagFilter3.at(i).mark == "=")
                        result4 = result4 || data.toInt() == TagFilter3.at(i).value;
                    else if (TagFilter3.at(i).mark == ">")
                        result4 = result4 || data.toInt() > TagFilter3.at(i).value;
                    else if (TagFilter3.at(i).mark == ">=")
                        result4 = result4 || data.toInt() >= TagFilter3.at(i).value;
                    //else return true;
                }
            }
            if (data.toString().contains(m_filterName, Qt::CaseInsensitive)) {
                resultName = resultName
                             || data.toString().contains(m_filterName, Qt::CaseInsensitive);
            }
        }
        return result1 && result2 && result3 && result4 && resultName;
    }
    //    if (m_search == "attack") {
    //        //qDebug() << attakerList.at(0);
    //        QModelIndex index = sourceModel()->index(sourceRow, Ecolumn::EName, sourceParent);
    //        QVariant data = sourceModel()->data(index);

    //        for (int i = 0; i < attakerList.size(); ++i) {
    //            if (data.toString() == attakerList.at(i)) {
    //                return true;
    //            }
    //        }
    //        return false;
    //    }

    //    if(m_search == "filterAllTable"){
    //        bool resultName = false;
    //        //qDebug()<<m_filterName;
    //        int itrator = NodelistModel->Data.at(0).FieldData.size();
    //        for (int i = 0; i < itrator; ++i) {
    //            QModelIndex index = sourceModel()->index(sourceRow, i, sourceParent);
    //            QVariant data = sourceModel()->data(index);
    //            //qDebug()<<data;
    //            if(data.toString().contains(m_filterName, Qt::CaseInsensitive))
    //                resultName = resultName || data.toString().contains(m_filterName, Qt::CaseInsensitive);
    //        }
    //        return resultName;
    //    }
    //qDebug()<<"cx";
    return true;
}

CategoryTabbarModel *NodeProxyModel::tabbarModel() const
{
    return mTabbarModel;
}

void NodeProxyModel::setTabbarModel(CategoryTabbarModel *newTabbarModel)
{
    mTabbarModel = newTabbarModel;
}

void NodeProxyModel::filterString(QString search, QString value)
{
    m_filterName = value;
    //m_search = search;
    m_search = "filter";
    //qDebug() << value;
    // TEST
    //    beginResetModel();
    //    endResetModel();
    // ENDTEST

    invalidateFilter();
}

void NodeProxyModel::filterStringColumn(QString tabName)
{
    m_filterColumn = tabName;
    invalidateFilter();
}

QList<QString> NodeProxyModel::getDataComboBox()
{
    return columnName;
}

QList<QString> NodeProxyModel::getDataComboBoxInt()
{
    return columnNameInt;
}

void NodeProxyModel::addTagColor(QString name, QString value)
{
    TagColor.append({name, value});
    m_search = "filter";
    invalidateFilter();
}

void NodeProxyModel::addTag1(QString name, QString value)
{
    TagFilter1.append({name, value});
    m_search = "filter";
    invalidateFilter();
}

void NodeProxyModel::addTag2(QString name, int value1, int value2)
{
    TagFilter2.append({name, value1, value2});
    m_search = "filter";
    invalidateFilter();
}

void NodeProxyModel::addTag3(QString name, int value, QString mark)
{
    TagFilter3.append({name, value, mark});
    m_search = "filter";
    invalidateFilter();
}

void NodeProxyModel::removeTag(QString filterSearch, QString name, QString value1)
{
    //qDebug()<<filterSearch<<name<<value1;
    if (filterSearch == "colorFilter") {
        TagColor.removeIf(
            [valueToRemove = value1](const FilterTag &s) { return s.value == valueToRemove; });
        //        TagColor.removeAll([valueToRemove = value1](const FilterTag& s) {
        //            return s.name == nameToRemove;
        //        });
    }
    if (filterSearch == "filter1") {
        TagFilter1.removeIf([valueToRemove = value1, nameToRemove = name](const FilterTag1 &s) {
            return s.value == valueToRemove && s.name == nameToRemove;
        });
    }
    if (filterSearch == "filter2") {
        TagFilter2.removeIf([valueToRemove = value1, nameToRemove = name](const FilterTag2 &s) {
            return s.valueFrom == valueToRemove.toInt() && s.name == nameToRemove;
        });
    }
    if (filterSearch == "filter3") {
        TagFilter3.removeIf([valueToRemove = value1, nameToRemove = name](const FilterTag3 &s) {
            return s.value == valueToRemove.toInt() && s.name == nameToRemove;
        });
    }

    if (TagColor.empty() && TagFilter1.empty() && TagFilter2.empty() && TagFilter3.empty())
        m_search = "";
    //    qDebug()<<TagColor.empty();
    invalidateFilter();
}

void NodeProxyModel::filterCategoryTag(QString name)
{
    mFilterType = name;
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

QStringList NodeProxyModel::filterCombo(QString text, QString nameFilter)
{
    QStringList filteredList;
    if (nameFilter == "String") {
        for (const QString &item : columnName) {
            if (item.contains(text, Qt::CaseInsensitive)) {
                filteredList.append(item);
            }
        }
    } else if (nameFilter == "Int") {
        for (const QString &item : columnNameInt) {
            if (item.contains(text, Qt::CaseInsensitive)) {
                filteredList.append(item);
            }
        }
    }
    return filteredList;
}

//-----------------------------------------NodeListModel------------------------------------
NodeListModel::NodeListModel(DataManager *dataManager)
{
    mDataManager = dataManager;

    selectionModel = new QItemSelectionModel();
}

int NodeListModel::rowCount(const QModelIndex &) const
{
    //    qDebug() <<Data.at(0).FieldData.size();
    //        qDebug() <<Data.size();
    //    if (modelType == "attackerModel")
    //        return DataAttacker->size();
    return mDataManager->nodeCount();
}

int NodeListModel::columnCount(const QModelIndex &) const
{
    return mDataManager->fixedColumnNames().size() + mDataManager->uniqueAddedColumnNames().size()
           + mDataManager->essentialColumnNames().size()
           + mDataManager->columnAttackTargetMore().size();
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

    if (index.column() > 1 && index.column() < columnCount() - 3 && role != Qt::DisplayRole) {
        return QVariant("notType");
    }

    if (index.column() == columnCount() - 3 && role != AttackerButtonRole) {
        return QVariant("notType");
    }

    if (index.column() == columnCount() - 2 && role != TargetButtonRole) {
        return QVariant("notType");
    }

    if (index.column() == columnCount() - 1 && role != MoreButtonRole) {
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
        return QString::number(nodeData.latitude, 'f', 2).toDouble(); //nodeData.latitude;
    } else if (index.column() == 7) {
        return QString::number(nodeData.longitude, 'f', 2).toDouble(); //nodeData.longitude;
    } else if (index.column() == 8) {
        return QString::number(nodeData.altitude, 'f', 2).toDouble(); //nodeData.altitude;
    } else if (index.column() == 9) {
        return nodeData.speed;
    } else if (index.column() > 9 && index.column() < columnCount() - 3) {
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
    } else if (index.column() == columnCount() - 3) {
        return "AttackerRole";
    } else if (index.column() == columnCount() - 2) {
        return "TargetRole";
    } else if (index.column() == columnCount() - 1) {
        return "MoreRole";
    }

    return "BADINDEX";

    //    // color roles
    //    if (role == Qt::BackgroundRole) {
    //        switch (index.column()) {
    //        case NodeListModel::EColumn::Color:
    //            return QColor(nodeData.color);
    //        default:
    //            return QColor("#DEE3E6");
    //        }
    //    }

    //    // image roles
    //    if (role == Qt::DecorationRole) {
    //        switch (index.column()) {
    //        case NodeListModel::EColumn::Icon:
    //            return nodeData.iconInfoUrl;
    //            //        case myEnum:
    //            //            return "qrc:/Resources/battle-icon.jpg";
    //            //        case NodeListModel::EColumn::Speed + 6 + 2:
    //            //            return "qrc:/Resources/target-icon.jpg";
    //            //        case NodeListModel::EColumn::Speed + 6 + 3:
    //            //            return "qrc:/Resources/more-icon.jpg";
    //            //        case NodeListModel::EColumn::Battle:
    //            //            return "qrc:/Resources/battle-icon.jpg";
    //            //        case NodeListModel::EColumn::Target:
    //            //            return "qrc:/Resources/target-icon.jpg";
    //            //        case NodeListModel::EColumn::More:
    //            //            return "qrc:/Resources/more-icon.jpg";
    //        default:
    //            return QVariant("qrc:/Resources/hand.png");
    //        }
    //    }
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
    } else if (section < columnCount() - mDataManager->columnAttackTargetMore().size()) {
        return mDataManager->uniqueAddedColumnNames().at(
            section - mDataManager->fixedColumnNames().size()
            - mDataManager->essentialColumnNames().size());
    } else {
        // icon column header name
        return "";
    }
}

void NodeListModel::selectionRow(int Row, int Column)
{
    //selectionModel->clear();
    //QItemSelectionModel selectionModel;
    //selectionModel->select(index(0,2), QItemSelectionModel::ClearAndSelect);
    selectionModel->select(index(0, 0),
                           QItemSelectionModel::Clear | QItemSelectionModel::SelectCurrent
                               | QItemSelectionModel::Rows);
    // for (int i = 0; i < Column; i++) {
    //     selectionModel->select(index(1, i), QItemSelectionModel::Select);
    // }
}

QItemSelectionModel *NodeListModel::selectModel()
{
    return selectionModel;
}

DataManager *NodeListModel::dataManager() const
{
    return mDataManager;
}

void NodeListModel::setDataManager(DataManager *newDataManager)
{
    mDataManager = newDataManager;
}

void NodeListModel::onNodeUpated(int index)
{
    emit dataChanged(this->index(index, 0), this->index(index, 0));
}

QHash<int, QByteArray> NodeListModel::roleNames() const
{
    return {{Qt::DisplayRole, "display"},
            {Qt::BackgroundRole, "background"},
            {Qt::DecorationRole, "decorate"},
            {Qt::EditRole, "editRole"},
            {AttackerButtonRole, "attackerButton"},
            {TargetButtonRole, "targetButton"},
            {MoreButtonRole, "moreButton"}};
}

//void NodeListModel::attacker(QString name)
//{
//    //qDebug() << "input" << name;
//    DataAttacker->clear();
//    int bound = Data.size();
//    int firstRow = -1;
//    //append first row
//    for (int i = 0; i < bound; ++i) {
//        if (Data.at(i).FieldData.at(2).value.toString() == name) {
//            firstRow = i;
//            DataAttacker->append(Data.at(i));
//        }
//    }
//    int numberOfRow = QRandomGenerator::global()->bounded(1, bound);
//    QSet<int> *randRow = new QSet<int>;
//    for (int iter = 0; iter < numberOfRow; ++iter) {
//        int randomRow = QRandomGenerator::global()->bounded(1, bound);
//        if (firstRow != randomRow)
//            randRow->insert(randomRow);
//    }
//    foreach (int data, *randRow) {
//        DataAttacker->append(Data.at(data));
//    }
//    delete randRow;
//}

//void NodeListModel::setChangeModel(QString checkModel)
//{
//    beginResetModel();
//    modelType = checkModel;
//    qDebug() << "Set Model: " << modelType;
//    endResetModel();
//}

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

    // TEST
    //    mDataManager->getUniqueCategoryNames()->append("akbar");
    //    mDataManager->getUniqueCategoryNames()->append("Abbas");
    // ENDTEST
}

int CategoryTabbarModel::rowCount(const QModelIndex &parent) const
{
    return mDataManager->getUniqueCategoryNames()->size();
}

QVariant CategoryTabbarModel::data(const QModelIndex &index, int role) const
{
    return mDataManager->getUniqueCategoryNames()->at(index.row());
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

CategoryTagModel *NodeProxyModel::categoryTagModel() const
{
    return m_categoryTagModel;
}

void NodeProxyModel::setCategoryTagModel(CategoryTagModel *newCategoryTagModel)
{
    if (m_categoryTagModel == newCategoryTagModel)
        return;
    m_categoryTagModel = newCategoryTagModel;
    emit categoryTagModelChanged();
}
