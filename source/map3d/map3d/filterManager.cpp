#include "filterManager.h"

FilterManager::FilterManager(QObject *parent) : QObject(parent){
}

void FilterManager::addFilterField(QString field, QString type)
{
    mFilterFields.insert(field);
    if (type == typeid(5).name())
        mFilterFieldsNum.insert(field);
}

bool FilterManager::checkNodeToShow(NodeData *nodeData)
{
    for (auto tag: mFilterTags.keys()) {
        bool andTag = false;
        auto it = std::find(nodeData->fieldData.begin(), nodeData->fieldData.end(), tag);
        if(it != nodeData->fieldData.end()) {
            for (auto &field: mFilterTags[tag]) {
                if (field.equalCheck) {
                    if (field.value == it->value) {
                        andTag = true;
                        break;
                    }
                }
                else {
                    if (field.comparision == "=") {
                        if (qFuzzyCompare(it->value.toDouble(), field.value.toDouble())) {
                            andTag = true;
                            break;
                        }
                    }
                    if (field.comparision == "<=") {
                        if (it->value.toDouble() <= field.value.toDouble()) {
                            andTag = true;
                            break;
                        }
                    }
                    else if (field.comparision == ">=") {
                        if (it->value.toDouble() >= field.value.toDouble()) {
                            andTag = true;
                            break;
                        }
                    }
                    else if (field.comparision == "<") {
                        if (it->value.toDouble() < field.value.toDouble()) {
                            andTag = true;
                            break;
                        }
                    }
                    else if (field.comparision == ">") {
                        if (it->value.toDouble() > field.value.toDouble()) {
                            andTag = true;
                            break;
                        }
                    }
                    if (field.comparision == "<=>") {
                        if (it->value.toDouble() <= field.value.toDouble() && it->value.toDouble() >= field.value.toDouble()) {
                            andTag = true;
                            break;
                        }
                    }
                }
            }
        }
        if (!andTag)
            return false;
    }
    return false;
}

void FilterManager::addFilterTag(QString key, QString value)
{
    if (mFilterTags.contains(key)) {
        mFilterTags[key].push_back(Tag{true, value});
    } else {
        mFilterTags[key] = QVector<Tag>{Tag{true, value}};
    }
}

void FilterManager::addFilterTag(QString key, double value, QString comp)
{
    if (mFilterTags.contains(key)) {
        mFilterTags[key].push_back(Tag{true, value, comp});
    } else {
        mFilterTags[key] = QVector<Tag>{Tag{true, value, comp}};
    }
}

void FilterManager::addFilterTag(QString key, double value1, double value2, QString comp)
{
    if (mFilterTags.contains(key)) {
        mFilterTags[key].push_back(Tag{true, 0, comp, QPair<double, double>{value1, value2}});
    } else {
        mFilterTags[key] = QVector<Tag>{Tag{true, 0, comp, QPair<double, double>{value1, value2}}};
    }
}

QSet<QString> FilterManager::getAllFilterFields()
{
    return mFilterFields;
}

QSet<QString> FilterManager::getIntFilterFields()
{
    return mFilterFieldsNum;
}
