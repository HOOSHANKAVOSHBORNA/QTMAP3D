#include "loadingPage.h"


LoadingPage::LoadingPage(QObject *parent):QAbstractListModel(parent)
{
//    mTimer = new QTimer(this);

//    connect(mTimer, &QTimer::timeout, [this](){

//        if(mLoadingDataItem.size() && !mLoadingDataItem[0].isError){
//            removeItem(0);
//            mTimer->stop();
//        }
//        updateData(mLoadingDataItem.size() - 1);
//    });

//    mTimer->start(1000);
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


    case acceptionState:
        return !mLoadingDataItem[row].isError;
    default:
        break;
    }

    return QVariant(false);

}

QHash<int, QByteArray> LoadingPage::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[acceptionState] = "acceptionState";
    return hash;
}

void LoadingPage::addItem(const QString &message, bool isError)
{
    qDebug()<< message<<": "<<isError;
    beginInsertRows(QModelIndex(), mLoadingDataItem.size(), mLoadingDataItem.size());
    mLoadingDataItem.push_back({message, isError});
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

//    mTimer->start(1000);
}
