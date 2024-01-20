#include "filterManager.h"

#include "serviceManager.h"
FilterManager::FilterManager(QObject *parent) : QObject(parent){
}

void FilterManager::addFilterField(NodeData *nodeData)
{
    for (auto &field : nodeData->fieldData){
        addFilterField(field.name, field.value);
    }
}

void FilterManager::addFilterField(QString field, QVariant value)
{
    if (field.toLower() == "color")
        mFilterFieldsColor.insert(value.toString());
    else if (std::strcmp(value.typeName(), "double") == 0 || std::strcmp(value.typeName(), "qlonglong") == 0)
        mFilterFieldsNum.insert(field);
    else
        mFilterFieldsStr.insert(field);
    emit filterFieldsChanged();
}

QSet<QString> FilterManager::numFilterFields() const
{
    return mFilterFieldsNum;
}

QSet<QString> FilterManager::colorFilterFields() const
{
    return mFilterFieldsColor;
}

QSet<QString> FilterManager::stringFilterFields() const
{
    return mFilterFieldsStr;
}

bool FilterManager::checkNodeToShow(NodeData *nodeData)
{
//    for (auto tag: mFilterTags.keys()) {
//        bool andTag = false;
//        auto it = std::find(nodeData->fieldData.begin(), nodeData->fieldData.end(), tag);
//        if(it != nodeData->fieldData.end()) {
//            for (auto &field: mFilterTags[tag]) {
//                if (field.equalCheck) {
//                    if (it->value.toString().startsWith(field.value.toString())) {
//                        andTag = true;
//                        break;
//                    }
//                }
//                else {
//                    if (field.comparision == "=") {
//                        if (qFuzzyCompare(it->value.toDouble(), field.value.toDouble())) {
//                            andTag = true;
//                            break;
//                        }
//                    }
//                    if (field.comparision == "<=") {
//                        if (it->value.toDouble() <= field.value.toDouble()) {
//                            andTag = true;
//                            break;
//                        }
//                    }
//                    else if (field.comparision == ">=") {
//                        if (it->value.toDouble() >= field.value.toDouble()) {
//                            andTag = true;
//                            break;
//                        }
//                    }
//                    else if (field.comparision == "<") {
//                        if (it->value.toDouble() < field.value.toDouble()) {
//                            andTag = true;
//                            break;
//                        }
//                    }
//                    else if (field.comparision == ">") {
//                        if (it->value.toDouble() > field.value.toDouble()) {
//                            andTag = true;
//                            break;
//                        }
//                    }
//                    if (field.comparision == "<=>") {
//                        if (it->value.toDouble() <= field.value.toDouble() && it->value.toDouble() >= field.value.toDouble()) {
//                            andTag = true;
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//        if (!andTag)
//            return false;
//    }
    return false;
}

void FilterManager::addFilterTag(QString key, QString value)
{
    if (mFilterTags.contains(key)) {
        mFilterTags[key].push_back(Tag{true, value});
    } else {
        mFilterTags[key] = QVector<Tag>{Tag{true, value}};
    }
    emit filterTagsEdited();
}

void FilterManager::addFilterTag(QString key, double value, QString comp)
{
//    if (mFilterTags.contains(key)) {
//        mFilterTags[key].push_back(Tag{false, value, comp});
//    } else {
//        mFilterTags[key] = QVector<Tag>{Tag{false, value, comp}};
//    }
    emit filterTagsEdited();
}

void FilterManager::addFilterTag(QString key, double value1, double value2, QString comp)
{
//    if (mFilterTags.contains(key)) {
//        mFilterTags[key].push_back(Tag{false, 0, comp, QPair<double, double>{value1, value2}});
//    } else {
//        mFilterTags[key] = QVector<Tag>{Tag{false, 0, comp, QPair<double, double>{value1, value2}}};
//    }
    emit filterTagsEdited();
}

void FilterManager::removeFilterTag(QString key, QString value)
{
    auto it = std::find(mFilterTags[key].begin(), mFilterTags[key].end(), Tag{true, value});
    if (it != mFilterTags[key].end()) {
        mFilterTags[key].erase(it);
    }
    emit filterTagsEdited();
}

void FilterManager::removeFilterTag(QString key, double value, QString comp)
{
//    auto it = std::find(mFilterTags[key].begin(), mFilterTags[key].end(), Tag{false, value, comp});
//    if (it != mFilterTags[key].end()) {
//        mFilterTags[key].erase(it);
//    }
    emit filterTagsEdited();
}

void FilterManager::removeFilterTag(QString key, double value1, double value2, QString comp)
{
//    auto it = std::find(mFilterTags[key].begin(), mFilterTags[key].end(), Tag{false, -999999999, comp, QPair<double, double>{value1, value2}});
//    if (it != mFilterTags[key].end()) {
//        mFilterTags[key].erase(it);
//    }
    emit filterTagsEdited();
}
