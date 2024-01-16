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
{
    //    int DataTableSize = dynamic_cast<NodeListModel*>(sourceModel())->Data.at(0).FieldData.size();
    //    for (int i = 0; i < DataTableSize; i++) {
    //        columnName.append(dynamic_cast<NodeListModel*>(sourceModel())->Data.at(0).FieldData.at(i).name);
    //    }
    //    NodeListModel *nodeModel = new NodeListModel;
    //    for (int i = 0; i < nodeModel->Data.at(0).FieldData.size(); i++) {
    //        columnName.append(nodeModel->Data.at(0).FieldData.at(i).name);
    //    }
    //    //add column Int to columnNameInt
    //    for (int i = 0; i < nodeModel->Data.at(0).FieldData.size(); ++i) {
    //        if (nodeModel->Data.at(0).FieldData.at(i).value.type() == QVariant::Int) {
    //            //qDebug()<<nodeListModel.Data.at(0).FieldData.at(i).name;
    //            columnNameInt.append(nodeModel->Data.at(0).FieldData.at(i).name);
    //        }
    //    }
    //    setSourceModel(nodeModel);

    //    qDebug()<<columnNameInt;
    //    qDebug()<<dynamic_cast<NodeListModel *>(sourceModel())->Data.at(0).FieldData.size();
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
    return true;
    if (m_filterColumn == tabList.at(0)) { //Main
        //        qDebug()<<sourceModel()->headerData(sourceColumn,Qt::Horizontal);
        //        QModelIndex index = sourceModel()->index(0, sourceColumn, sourceParent);
        //        QVariant data = sourceModel()->data(index);
        QVariant data = sourceModel()->headerData(sourceColumn, Qt::Horizontal);
        bool result = false;
        for (int i = 0; i < columnName.size(); ++i) {
            if (i <= Ecolumn::EMaster || i >= Ecolumn::EBattle) {
                //qDebug()<<Ecolumn::EMaster;
                result = result || data.toString().contains(columnName.at(i), Qt::CaseInsensitive);
            }
        }
        return result;
    } else if (m_filterColumn == tabList.at(1)) { //Location
        QVariant data = sourceModel()->headerData(sourceColumn, Qt::Horizontal);
        bool result = false;
        for (int i = 0; i < columnName.size(); ++i) {
            if (i <= Ecolumn::EName || i >= Ecolumn::ELatitude) {
                result = result || data.toString().contains(columnName.at(i), Qt::CaseInsensitive);
            }
        }
        return result;
    }
    return true;
}

bool NodeProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    return true;
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

    if (m_search == "type") {
        QModelIndex index = sourceModel()->index(sourceRow, Ecolumn::EType, sourceParent);
        QVariant data = sourceModel()->data(index);
        if (mFilterType == "All")
            return true;
        else
            return data.toString().contains(mFilterType, Qt::CaseInsensitive);
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

void NodeProxyModel::nodeTypeFilter(QString type)
{
    mFilterType = type;
    m_search = "type";
    invalidateFilter();
}

void NodeProxyModel::filterString(QString search, QString value)
{
    m_filterName = value;
    //m_search = search;
    m_search = "filter";
    //qDebug() << value;
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
        //qDebug()<<"ssss";
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
    connect(mDataManager, &DataManager::nodeDataManagerChanged, this, &NodeListModel::resetTable);

    selectionModel = new QItemSelectionModel();
}

int NodeListModel::rowCount(const QModelIndex &) const
{
    //    qDebug() <<Data.at(0).FieldData.size();
    //        qDebug() <<Data.size();
    //    if (modelType == "attackerModel")
    //        return DataAttacker->size();
    return mDataManager->nodeCount();
    //    qDebug() << "debug: " << mDataManager->nodeCount();
    //    return 10;
}

int NodeListModel::columnCount(const QModelIndex &) const
{
    //qDebug()<<Data.at(0).FieldData.size();
    //    if (modelType == "attackerModel")
    //        return DataAttacker->at(0).FieldData.size();

    // TODO
    return 11;
}

QVariant NodeListModel::data(const QModelIndex &index, int role) const
{
    NodeData nodeData = mDataManager->getNodeAtIndex(index.row())->nodeData();

    //    id= nodeData.id;
    //    name= nodeData.name;
    //    type= nodeData.type;
    //    category = nodeData.category;
    //    url2D= nodeData.url2D;
    //    url3D= nodeData.url3D;
    //    imgInfoUrl = nodeData.imgInfoUrl;
    //    iconInfoUrl = nodeData.iconInfoUrl;
    //    color= nodeData.color;
    //    isAttacker = nodeData.isAttacker;
    //    latitude = nodeData.latitude;
    //    longitude = nodeData.longitude;
    //    altitude = nodeData.altitude;
    //    speed= nodeData.speed;
    //    command= nodeData.command;

    // string or int roles
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case NodeListModel::EColumn::Name:
            return nodeData.name;
        case NodeListModel::EColumn::Type:
            return nodeData.type.toString();
        case NodeListModel::EColumn::Latitude:
            return QString::number(nodeData.latitude, 'l', 2);
        case NodeListModel::EColumn::Longitude:
            return QString::number(nodeData.longitude, 'l', 2);
        case NodeListModel::EColumn::Altitude:
            return QString::number(nodeData.altitude, 'l', 2);
        case NodeListModel::EColumn::Speed:
            return QString::number(nodeData.speed, 'l', 2);
        default:
            return QVariant("defualt");
        }
    }

    // color roles
    if (role == Qt::BackgroundRole) {
        switch (index.column()) {
        case NodeListModel::EColumn::Color:
            return QColor(nodeData.color);
        default:
            return QColor("#DEE3E6");
        }
    }

    // image roles
    if (role == Qt::DecorationRole) {
        switch (index.column()) {
        case NodeListModel::EColumn::Icon:
            return nodeData.iconInfoUrl;
        case NodeListModel::EColumn::Battle:
            return "qrc:/Resources/battle-icon.jpg";
        case NodeListModel::EColumn::Target:
            return "qrc:/Resources/target-icon.jpg";
        case NodeListModel::EColumn::More:
            return "qrc:/Resources/more-icon.jpg";
        default:
            return QVariant("qrc:/Resources/hand.png");
        }
    }
    //    if (role == Qt::BackgroundRole) {
    //        // color of first column
    //        if (index.column() == 0) {
    //            //            if (modelType == "attackerModel") {
    //            //                return QColor(
    //            //                    DataAttacker->at(index.row()).FieldData.at(index.column()).value.toString());
    //            //            }
    //            //            return QColor(Data.at(index.row()).FieldData.at(index.column()).value.toString());
    //            return QColor(nodeData.color);
    //        }
    //        //        if (index.row())   //change background only for cell(1,2)
    //        //        {
    //        QColor Background("#DEE3E6"); //#bac4f5 //#DEE3E6
    //        return Background;
    //    }

    //    if (role == Qt::DecorationRole && index.column() == 1 || index.column() == 15
    //        || index.column() == 16 || index.column() == 17) {
    //        //        if (modelType == "attackerModel") {
    //        //            return DataAttacker->at(index.row()).FieldData.at(index.column()).value.toString();
    //        //        }
    //        //        return Data.at(index.row()).FieldData.at(index.column()).value.toString();
    //        return nodeData.iconInfoUrl;
    //    }

    //    if (role == Qt::DisplayRole) {
    //        //        for (int i = 0; i < Data.size(); i++) {
    //        //            if(index.column() == i){
    //        //                return Data.at(index.row()).FieldData.at(index.column()).value;}
    //        //        }

    //        //        if (modelType == "attackerModel") {
    //        //            //qDebug() << "change Model!!!!!";
    //        //            return DataAttacker->at(index.row()).FieldData.at(index.column()).value;
    //        //        }
    //        //        return Data.at(index.row()).FieldData.at(index.column()).value;
    //        return nodeData.name;
    //    } else {
    //        return QVariant("defualt");
    //    }
}

QVariant NodeListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case NodeListModel::EColumn::Color:
            return columnToName[NodeListModel::EColumn::Color];
        case NodeListModel::EColumn::Icon:
            return columnToName[NodeListModel::EColumn::Icon];
        case NodeListModel::EColumn::Name:
            return columnToName[NodeListModel::EColumn::Name];
        case NodeListModel::EColumn::Type:
            return columnToName[NodeListModel::EColumn::Type];
        case NodeListModel::EColumn::Latitude:
            return columnToName[NodeListModel::EColumn::Latitude];
        case NodeListModel::EColumn::Longitude:
            return columnToName[NodeListModel::EColumn::Longitude];
        case NodeListModel::EColumn::Altitude:
            return columnToName[NodeListModel::EColumn::Altitude];
        case NodeListModel::EColumn::Speed:
            return columnToName[NodeListModel::EColumn::Speed];
        case NodeListModel::EColumn::Battle:
            return columnToName[NodeListModel::EColumn::Battle];
        case NodeListModel::EColumn::Target:
            return columnToName[NodeListModel::EColumn::Target];
        case NodeListModel::EColumn::More:
            return columnToName[NodeListModel::EColumn::More];
        default:
            return QVariant("defualt");
        }
    } else
        return QVariant("defualt");
}

void NodeListModel::selectionRow(int Row, int Column)
{
    qDebug() << "Row select: " << Row << "Column select: " << Column;
    //selectionModel->clear();
    //QItemSelectionModel selectionModel;
    //selectionModel->select(index(0,2), QItemSelectionModel::ClearAndSelect);
    qDebug() << "wtf:" << selectionModel;
    selectionModel->select(index(0, 0),
                           QItemSelectionModel::Clear | QItemSelectionModel::SelectCurrent
                               | QItemSelectionModel::Rows);
    // for (int i = 0; i < Column; i++) {
    //     selectionModel->select(index(1, i), QItemSelectionModel::Select);
    // }
}

QItemSelectionModel *NodeListModel::selectModel()
{
    qDebug() << "selectttt ";
    return selectionModel;
}

void NodeListModel::resetTable()
{
    qDebug() << "debug"
             << "lolo";
    beginResetModel();
    endResetModel();
}

QHash<int, QByteArray> NodeListModel::roleNames() const
{
    return {
        {Qt::DisplayRole, "display"},
        {Qt::BackgroundRole, "background"},
        {Qt::DecorationRole, "decorate"},
        {Qt::EditRole, "editRole"}
        //             {Qt::EditRole, "edit"},
    };
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
