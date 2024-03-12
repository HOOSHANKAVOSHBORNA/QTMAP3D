#include "statusFilter.h"
#include "qqmlcomponent.h"
#include "qquickitem.h"
#include "mainwindow.h"
#include <userManager.h>


StatusFilter::StatusFilter(QQmlEngine *engine, MainWindow *window) {
    mFilterModel = new StatusFieldModel;

    QQmlComponent *comp = new QQmlComponent(engine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [&](QQmlComponent::Status status) {
        if (status == QQmlComponent::Error) {
            qDebug() << "Can not load this: " << comp->errorString();
        }
        if (status == QQmlComponent::Ready) {
            QQuickItem *item = qobject_cast<QQuickItem *>(comp->create());
            item->setProperty("filterModel",
                              QVariant::fromValue<StatusFieldModel *>(mFilterModel));
            window->setSettingsItem(item);
        }
    });
    comp->loadUrl(QUrl("qrc:/StatusSettingItem.qml"));
}

StatusNodeData StatusFilter::filterStatusData(const StatusNodeData &statusNodeData)
{
    mData = statusNodeData;
    mData.fieldData.clear();
    for (int var = 0; var < statusNodeData.fieldData.size(); ++var) {
        mFilterModel->addField(statusNodeData.fieldData[var].name);
    }
    QVector<QString> names = mFilterModel->getStatusActiveFields();
    for (int var = 0; var < statusNodeData.fieldData.size(); ++var) {
        if(names.contains(statusNodeData.fieldData[var].name)){
            NodeFieldData fieldData;
            fieldData.name = statusNodeData.fieldData[var].name;
            fieldData.value = statusNodeData.fieldData[var].value;
            fieldData.category = statusNodeData.fieldData[var].category;
            fieldData.categoryIconUrl = statusNodeData.fieldData[var].categoryIconUrl;
            mData.fieldData.push_back(fieldData);
        }
    }
    return mData;
}


StatusFieldModel::StatusFieldModel(QObject *parent)
{
    mStatusSettings = new QSettings("Map3D",UserManager::instance()->userName());
}

int StatusFieldModel::rowCount(const QModelIndex &parent) const
{
    return mFields.size();
}

QVariant StatusFieldModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
    case fieldName:
        return mFields[index.row()].name;
        break;
    case fieldIsActive:
        return mFields[index.row()].isActive;
        break;
    default:
        return QVariant();
    };
}

QHash<int, QByteArray> StatusFieldModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[fieldName] = "fieldName";
    hash[fieldIsActive] = "fieldIsActive";
    return hash;
}

void StatusFieldModel::toggleCheck(const QModelIndex &index, bool check)
{
    beginResetModel();
    mFields[index.row()].isActive = check;
    endResetModel();
    mStatusSettings->beginGroup("status");
    mStatusSettings->setValue(mFields[index.row()].name,check);
    mStatusSettings->endGroup();
}


void StatusFieldModel::addField(QString name)
{
    // qDebug() << name;
    beginResetModel();
    for (int var = 0; var < mFields.size(); ++var){
        if(mFields[var].name == name){
            mStatusSettings->beginGroup("status");
            if(mStatusSettings->contains(name)){
                mFields[var].isActive = mStatusSettings->value(name).toBool();
            }
            mStatusSettings->endGroup();
            endResetModel();
            return;
        }
    }
    fieldModelData newData;
    newData.name = name;
    mStatusSettings->beginGroup("status");
    if(mStatusSettings->contains(name)){
        newData.isActive = mStatusSettings->value(name).toBool();
    }else{
        newData.isActive = true;
    }
    mStatusSettings->endGroup();
    mFields.append(newData);
    endResetModel();
}

QVector<QString> StatusFieldModel::getStatusActiveFields()
{
    QVector<QString> vec;
    for (int var = 0; var < mFields.size(); ++var) {
        if(mFields[var].isActive){
            vec.append(mFields[var].name);
        }
    }
    return vec;
}

void StatusFieldModel::fakeData()
{
    addField(QDateTime::currentDateTime().toString());
}



