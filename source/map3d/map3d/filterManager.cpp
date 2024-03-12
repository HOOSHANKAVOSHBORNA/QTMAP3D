#include "filterManager.h"
#include "serviceManager.h"
#include "userManager.h"


FilterManager::FilterManager(QObject *parent) : QObject(parent)
{

    mFilterFieldsColor = new QSortFilterProxyModel(this);
    mFilterFieldsColor->setFilterRole(Qt::DisplayRole);
    mFilterFieldsColor->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mFilterFieldsColorModel = new FilterFieldModel(this);
    mFilterFieldsColor->setSourceModel(mFilterFieldsColorModel);

    mFilterFieldsStr = new QSortFilterProxyModel(this);
    mFilterFieldsStr->setFilterRole(Qt::DisplayRole);
    mFilterFieldsStr->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mFilterFieldsStrModel = new FilterFieldModel(this);
    mFilterFieldsStr->setSourceModel(mFilterFieldsStrModel);

    mFilterFieldsNum = new QSortFilterProxyModel(this);
    mFilterFieldsNum->setFilterRole(Qt::DisplayRole);
    mFilterFieldsNum->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mFilterFieldsNumModel = new FilterFieldModel(this);
    mFilterFieldsNum->setSourceModel(mFilterFieldsNumModel);

    // mFilterSettings = new QSettings("Map3D",UserManager::instance()->userName());
    // mFilterSettings->beginGroup("filter");
    // for (int var = 0; var < mFilterSettings->allKeys().count(); ++var) {
    //     QString key = mFilterSettings->allKeys().at(var);
    //     QVariantList data = mFilterSettings->value(key).toList();
    //     Tag::LogicalOperator op;
    //     if(data[2].toString().startsWith("or")){
    //         op = Tag::Or;
    //     }else{
    //         op = Tag::And;
    //     }
    //     addFilterTag(key,data[0],data[1].toString(),op);
    // }
    // mFilterSettings->endGroup();

}

FilterManager::~FilterManager()
{
    qDebug() << "~FilterManager!";
}


void FilterManager::addColorFilterField(QString fields)
{
    mFilterFieldsColorModel->addFilterField(fields);
}

void FilterManager::addNumFilterField(QString fields)
{
    mFilterFieldsNumModel->addFilterField(fields);
}

void FilterManager::addStringFilterField(QString fields)
{
    mFilterFieldsStrModel->addFilterField(fields);
}

void FilterManager::removeColorFilterField(QString field)
{
    mFilterFieldsColorModel->removeFilterField(field);
}

void FilterManager::removeNumFilterField(QString field)
{
    mFilterFieldsNumModel->removeFilterField(field);
}

void FilterManager::removeStringFilterField(QString field)
{
    mFilterFieldsStrModel->removeFilterField(field);
}

void FilterManager::addFilterField(const NodeData& nodeData)
{
    for (auto &field : nodeData.fieldData){
        // addFilterField(field.name, field.value);
        for (auto &field : nodeData.fieldData) {
            if (field.name.toLower() == "color")
                addColorFilterField(field.value.toString());
            else if (std::strcmp(field.value.typeName(), "double") == 0
                     || std::strcmp(field.value.typeName(), "qlonglong") == 0
                     || std::strcmp(field.value.typeName(), "int") == 0)
                addNumFilterField(field.name);
            else if (std::strcmp(field.value.typeName(), "QString") == 0)
                addStringFilterField(field.name);
        }
    }
}

bool FilterManager::checkNodeToShow(NodeData *nodeData, Tag *tag)
{
    auto it = std::find(nodeData->fieldData.begin(), nodeData->fieldData.end(), tag->field);
    if (it == nodeData->fieldData.end())
        return false;
    if (it != nodeData->fieldData.end()) {
        switch (tag->comparision) {
        case Tag::Comparision::Equal:
            if (it->value.toString().startsWith(tag->value.toString())) {
                return true;
            }
            break;
        case Tag::Comparision::Less:
            if (it->value.toDouble() < tag->value.toDouble()) {
                return true;
            }
            break;
        case Tag::Comparision::LessEqual:
            if (it->value.toDouble() <= tag->value.toDouble()) {
                return true;
            }
            break;
        case Tag::Comparision::Greater:
            if (it->value.toDouble() > tag->value.toDouble()) {
                return true;
            }
            break;
        case Tag::Comparision::GreaterEqual:
            if (it->value.toDouble() >= tag->value.toDouble()) {
                return true;
            }
            break;
        case Tag::Comparision::NotEqual:
            if (it->value.toString() != tag->value.toString()) {
                return true;
            }
            break;
        }
    }
    return false;
}

bool FilterManager::checkNodeToShow(NodeData *nodeData)
{
    if (mFilterTags.isEmpty())
        return true;
    // check first tag to set flag in start
    bool firstTag = false;
    bool flag = checkNodeToShow(nodeData, mFilterTags[0]);
    for (auto &tag: mFilterTags) {
        if (!tag->isEnabled)
            continue;
        if (tag->logicalOperator == Tag::LogicalOperator::And) {
            if (firstTag)
                flag = flag && checkNodeToShow(nodeData, tag);
            else {
                firstTag = true;
                flag = checkNodeToShow(nodeData, tag);
            }
        } else {
            if (firstTag)
                flag = flag || checkNodeToShow(nodeData, tag);
            else {
                firstTag = true;
                flag = checkNodeToShow(nodeData, tag);
            }
        }
    }

    return firstTag ? flag : true;
}

void FilterManager::addFilterTag(QString field, QVariant value, QString comp, Tag::LogicalOperator op)
{
    Tag* tag = new Tag{field, value, Tag::srtingToComparison(comp), op};
    if (!mFilterTags.contains(tag)) {
        mFilterTags.push_back(tag);
        connect(tag, &Tag::tagChanged, this, &FilterManager::filterTagsEdited);
        //----------------------------------------------------
        // if(!mFilterSettings->contains(field)){
        //     mFilterSettings->beginGroup("filter");
        //     QVariantList list;
        //     list.insert(0,value);
        //     list.insert(1,comp);
        //     list.insert(2,op);
        //     mFilterSettings->setValue(field,list);
        //     mFilterSettings->endGroup();
        // }
    }else
        delete tag;
    emit filterTagsEdited();

}

void FilterManager::removeFilterTag(QString field, QVariant value, QString comp, Tag::LogicalOperator op)
{
    Tag *tag = new Tag{field, value,  Tag::srtingToComparison(comp), op};
    auto it = std::find_if(mFilterTags.begin(), mFilterTags.end(), [this, tag](const Tag *t){
        return *tag == t;
    });
    if (it != mFilterTags.end()) {
        mFilterTags.erase(it);
    }
    // if(mFilterSettings->contains(field)){
    //     mFilterSettings->beginGroup("filter");
    //     mFilterSettings->remove(field);
    //     mFilterSettings->endGroup();
    // }
    emit filterTagsEdited();
}

void FilterManager::removeFilterTag(int index)
{
    if (index >= mFilterTags.size())
        return;
    // mFilterSettings->beginGroup("filter");
    // if(mFilterSettings->contains(mFilterTags[index]->field)){
    //     mFilterSettings->remove(mFilterTags[index]->field);
    // }
    // mFilterSettings->endGroup();
    mFilterTags.remove(index);
    emit filterTagsEdited();
}

const QVector<Tag *> FilterManager::getFilterTags() const
{
    return mFilterTags;
}

const Tag* const FilterManager::getFilterTagAt(int index)
{
    return mFilterTags[index];
}

QSortFilterProxyModel *FilterManager::colorFilterFields() const
{
    return mFilterFieldsColor;
}

QSortFilterProxyModel *FilterManager::stringFilterFields() const
{
    return mFilterFieldsStr;
}

QSortFilterProxyModel *FilterManager::numFilterFields() const
{
    return mFilterFieldsNum;
}

FilterFieldModel::FilterFieldModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int FilterFieldModel::rowCount(const QModelIndex &parent) const
{
    return mFilterFields.size();
}

QVariant FilterFieldModel::data(const QModelIndex &index, int role) const
{
    return mFilterFields[index.row()];
}

void FilterFieldModel::addFilterField(QString field)
{
    if (mFilterFields.indexOf(field) != -1)
        return;
    beginInsertRows(QModelIndex(), mFilterFields.size(), mFilterFields.size());
    mFilterFields.push_back(field);
    endInsertRows();
}

void FilterFieldModel::removeFilterField(QString field)
{
    auto it = std::find(mFilterFields.begin(), mFilterFields.end(), field);
    if (it != mFilterFields.end()) {
        beginRemoveRows(QModelIndex(), it - mFilterFields.begin(), it - mFilterFields.begin());
        mFilterFields.erase(it);
        endRemoveRows();
    }
}
