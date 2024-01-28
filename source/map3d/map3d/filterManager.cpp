#include "filterManager.h"

#include "serviceManager.h"
FilterManager::FilterManager(QObject *parent) : QObject(parent)
{
    qmlRegisterType<Tag>("Crystal", 1, 0, "Tag");
    qmlRegisterType<Tag::Comparision>("Crystal", 1, 0, "TagComparision");
}

void FilterManager::addFilterField(NodeData *nodeData)
{
    for (auto &field : nodeData->fieldData){
        addFilterField(field.name, field.value);
    }
}

void FilterManager::addFilterField(QString field, QVariant value)
{
    if (field.toLower() == "color" && mFilterFieldsColor.indexOf(value.toString()) == -1)
        mFilterFieldsColor.push_back(value.toString());
    else if (mFilterFieldsNum.indexOf(field) == -1 && (std::strcmp(value.typeName(), "double") == 0 || std::strcmp(value.typeName(), "qlonglong") == 0))
        mFilterFieldsNum.push_back(field);
    else if (mFilterFieldsStr.indexOf(field) == -1 && std::strcmp(value.typeName(), "QString") == 0)
        mFilterFieldsStr.push_back(field);

    emit filterFieldsChanged();
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

const QVector<QString> &FilterManager::numFilterFields() const
{
    return mFilterFieldsNum;
}

const QVector<QString> &FilterManager::colorFilterFields() const
{
    return mFilterFieldsColor;
}

const QVector<QString> &FilterManager::stringFilterFields() const
{
    return mFilterFieldsStr;
}

bool FilterManager::checkNodeToShow(NodeData *nodeData)
{
    if (mFilterTags.isEmpty())
        return true;
    // check first tag to set flag in start
    bool flag = checkNodeToShow(nodeData, mFilterTags[0]);
    for (auto &tag: mFilterTags) {
        if (tag->logicalOperator == Tag::LogicalOperator::And) {
            flag = flag && checkNodeToShow(nodeData, tag);
        } else {
            flag = flag || checkNodeToShow(nodeData, tag);
        }
    }

    return flag;
}

void FilterManager::addFilterTag(QString field, QVariant value, Tag::Comparision comp, Tag::LogicalOperator op)
{
    Tag* tag = new Tag{field, value, comp, op};
    if (!mFilterTags.contains(tag)) {
        mFilterTags.push_back(tag);
    }else
        delete tag;
    emit filterTagsEdited();
}

void FilterManager::removeFilterTag(QString field, QVariant value, Tag::Comparision comp, Tag::LogicalOperator op)
{
    Tag *tag = new Tag{field, value, comp, op};
    auto it = std::find(mFilterTags.begin(), mFilterTags.end(), tag);
    if (it != mFilterTags.end()) {
        mFilterTags.erase(it);
    }
    emit filterTagsEdited();
}

const QVector<Tag *> FilterManager::getFilterTags() const
{
    return mFilterTags;
}

// void FilterManager::addFilterTag(QString key, QString value)
// {
// //    if (mFilterTags.contains(key)) {
// //        mFilterTags[key].push_back(Tag{true, value});
// //    } else {
// //        mFilterTags[key] = QVector<Tag>{Tag{true, value}};
// //    }

// }

// void FilterManager::addFilterTag(QString key, double value, QString comp)
// {
// //    if (mFilterTags.contains(key)) {
// //        mFilterTags[key].push_back(Tag{false, value, comp});
// //    } else {
// //        mFilterTags[key] = QVector<Tag>{Tag{false, value, comp}};
// //    }
//     emit filterTagsEdited();

// }

// void FilterManager::addFilterTag(QString key, double value1, double value2, QString comp)
// {
// //    if (mFilterTags.contains(key)) {
// //        mFilterTags[key].push_back(Tag{false, 0, comp, QPair<double, double>{value1, value2}});
// //    } else {
// //        mFilterTags[key] = QVector<Tag>{Tag{false, 0, comp, QPair<double, double>{value1, value2}}};
// //    }
//     emit filterTagsEdited();
// }

// void FilterManager::removeFilterTag(QString key, QString value)
// {
// //    auto it = std::find(mFilterTags[key].begin(), mFilterTags[key].end(), Tag{true, value});
// //    if (it != mFilterTags[key].end()) {
// //        mFilterTags[key].erase(it);
// //    }
//     emit filterTagsEdited();
// }

// void FilterManager::removeFilterTag(QString key, double value, QString comp)
// {
// //    auto it = std::find(mFilterTags[key].begin(), mFilterTags[key].end(), Tag{false, value, comp});
// //    if (it != mFilterTags[key].end()) {
// //        mFilterTags[key].erase(it);
// //    }
//     emit filterTagsEdited();
// }

// void FilterManager::removeFilterTag(QString key, double value1, double value2, QString comp)
// {
// //    auto it = std::find(mFilterTags[key].begin(), mFilterTags[key].end(), Tag{false, -999999999, comp, QPair<double, double>{value1, value2}});
// //    if (it != mFilterTags[key].end()) {
// //        mFilterTags[key].erase(it);
// //    }
//     emit filterTagsEdited();
// }
