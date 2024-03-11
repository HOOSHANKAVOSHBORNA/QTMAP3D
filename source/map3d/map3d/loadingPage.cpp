#include "loadingPage.h"


LoadingPage::LoadingPage(QObject *parent):QAbstractListModel(parent)
{
}

int LoadingPage::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mLoadingDataItem.size();
}

QVariant LoadingPage::data(const QModelIndex &index, int role) const
{
    const int row = index.row();

    switch(role) {
    case Qt::DisplayRole:
        return mLoadingDataItem[row].text;


    case errorStatus:
        return !mLoadingDataItem[row].isError;
    default:
        break;
    }

    return QVariant(false);

}

QHash<int, QByteArray> LoadingPage::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[errorStatus] = "errorStatus";
    return hash;
}

void LoadingPage::addItem(const QString &message, bool isError)
{
    beginInsertRows(QModelIndex(), 0, 0);
    mLoadingDataItem.insert(0, {message, isError});
    endInsertRows();
}

void LoadingPage::removeItem(int index)
{
    if(index < mLoadingDataItem.size()){
        beginRemoveRows(QModelIndex(),index,index);
        mLoadingDataItem.erase(mLoadingDataItem.begin() + index);
        endRemoveRows();
    }
    else
        return;
}

void LoadingPage::swapItem(int sourceIndex, int destinationIndex)
{
    if(sourceIndex < mLoadingDataItem.size() && destinationIndex < mLoadingDataItem.size()){
        beginMoveRows(QModelIndex(),sourceIndex,sourceIndex,QModelIndex(),destinationIndex);
        std::swap(mLoadingDataItem[sourceIndex], mLoadingDataItem[destinationIndex]);
        endMoveRows();
    }
    else
        return;

}

void LoadingPage::updateData(int index)
{
    for(int m = index; m > 0; m--)
        if(!mLoadingDataItem[m].isError && mLoadingDataItem[m-1].isError)
            swapItem(m , m - 1);
}

int LoadingPage::pluginCounter() const
{
    return mPluginCounter;
}

void LoadingPage::setPluginCounter(int pluginCounter)
{
    if (mPluginCounter == pluginCounter)
        return;
    mPluginCounter = pluginCounter;
    emit pluginCounterChanged();
}
