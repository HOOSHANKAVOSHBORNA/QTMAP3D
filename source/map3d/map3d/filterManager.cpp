#include "filterManager.h"

#include "serviceManager.h"
FilterManager::FilterManager(QObject *parent) : QObject(parent)
{
    qmlRegisterType<Tag>("Crystal", 1, 0, "Tag");
}

void FilterManager::addColorFilterField(QSet<QString> &fields)
{
    for (auto &i: fields)
        addColorFilterField(i);
}

void FilterManager::setColorFilterField(QSet<QString> &fields)
{
    mFilterFieldsColor = fields;
    emit filterFieldsChanged();
}

void FilterManager::addColorFilterField(QString fields)
{
    mFilterFieldsColor.insert(fields);
    emit filterFieldsChanged();
}

void FilterManager::addNumFilterField(QSet<QString> &fields)
{
    for (auto &i: fields)
        addNumFilterField(i);
}

void FilterManager::setNumFilterField(QSet<QString> &fields)
{
    mFilterFieldsNum = fields;
    emit filterFieldsChanged();
}

void FilterManager::addNumFilterField(QString fields)
{
    mFilterFieldsNum.insert(fields);
    emit filterFieldsChanged();
}

void FilterManager::addStringFilterField(QSet<QString> &fields)
{
    for (auto &i: fields)
        addStringFilterField(i);
}

void FilterManager::setStringFilterField(QSet<QString> &fields)
{
    mFilterFieldsStr = fields;
    emit filterFieldsChanged();
}

void FilterManager::addStringFilterField(QString fields)
{
    mFilterFieldsStr.insert(fields);
    emit filterFieldsChanged();
}

void FilterManager::removeColorFilterField(QString field)
{
    mFilterFieldsColor.remove(field);
    emit filterFieldsChanged();
}

void FilterManager::removeNumFilterField(QString field)
{
    mFilterFieldsNum.remove(field);
    emit filterFieldsChanged();
}

void FilterManager::removeStringFilterField(QString field)
{
    mFilterFieldsStr.remove(field);
    emit filterFieldsChanged();
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

const QSet<QString> &FilterManager::numFilterFields() const
{
    // qDebug() << "num:" << mFilterFieldsNum;
    return mFilterFieldsNum;
}

const QSet<QString> &FilterManager::colorFilterFields() const
{
    // qDebug() << "color:" << mFilterFieldsColor;
    return mFilterFieldsColor;
}

const QSet<QString> &FilterManager::stringFilterFields() const
{
    // qDebug() << "string:" << mFilterFieldsStr;
    return mFilterFieldsStr;
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

void FilterManager::addFilterTag(QString field, QVariant value, Tag::Comparision comp, Tag::LogicalOperator op)
{
    Tag* tag = new Tag{field, value, comp, op};
    if (!mFilterTags.contains(tag)) {
        mFilterTags.push_back(tag);
        connect(tag, &Tag::tagChanged, this, &FilterManager::filterTagsEdited);
    }else
        delete tag;
    emit filterTagsEdited();
}

void FilterManager::removeFilterTag(QString field, QVariant value, Tag::Comparision comp, Tag::LogicalOperator op)
{
    Tag *tag = new Tag{field, value, comp, op};
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
