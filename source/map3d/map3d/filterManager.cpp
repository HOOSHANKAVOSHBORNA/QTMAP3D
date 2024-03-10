#include "filterManager.h"

#include "serviceManager.h"
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

}

void FilterManager::addColorFilterField(QSet<QString> &fields)
{
    for (auto &i: fields)
        addColorFilterField(i);
}

void FilterManager::setColorFilterField(QSet<QString> &fields)
{
    mFilterFieldsColorModel->setFilterField(fields);
}

void FilterManager::addColorFilterField(QString fields)
{
    mFilterFieldsColorModel->addFilterField(fields);
}

void FilterManager::addNumFilterField(QSet<QString> &fields)
{
    for (auto &i: fields)
        addNumFilterField(i);
}

void FilterManager::setNumFilterField(QSet<QString> &fields)
{
    mFilterFieldsNumModel->setFilterField(fields);
}

void FilterManager::addNumFilterField(QString fields)
{
    mFilterFieldsNumModel->addFilterField(fields);
}

void FilterManager::addStringFilterField(QSet<QString> &fields)
{
    for (auto &i: fields)
        addStringFilterField(i);
}

void FilterManager::setStringFilterField(QSet<QString> &fields)
{
    mFilterFieldsStrModel->setFilterField(fields);
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

// void FilterManager::addFilterField(NodeData *nodeData)
// {
//     for (auto &field : nodeData->fieldData){
//         addFilterField(field.name, field.value);
//     }
// }

// void FilterManager::addFilterField(QString field, QVariant value)
// {
//     if (field.toLower() == "color" && mFilterFieldsColor.indexOf(value.toString()) == -1)
//         mFilterFieldsColor.push_back(value.toString());
//     else if (mFilterFieldsNum.indexOf(field) == -1 && (std::strcmp(value.typeName(), "double") == 0 || std::strcmp(value.typeName(), "qlonglong") == 0))
//         mFilterFieldsNum.push_back(field);
//     else if (mFilterFieldsStr.indexOf(field) == -1 && std::strcmp(value.typeName(), "QString") == 0)
//         mFilterFieldsStr.push_back(field);

//     emit filterFieldsChanged();
// }

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
    emit filterTagsEdited();
}

void FilterManager::removeFilterTag(int index)
{
    if (index >= mFilterTags.size())
        return;
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

void FilterFieldModel::addFilterField(QSet<QString> &fields)
{
    for (auto &i: fields)
        addFilterField(i);
}

void FilterFieldModel::setFilterField(QSet<QString> &fields)
{
    beginResetModel();
    mFilterFields = fields.values().toVector();
    endResetModel();
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
