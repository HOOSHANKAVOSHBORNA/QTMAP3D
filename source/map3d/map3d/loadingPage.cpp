#include "loadingPage.h"


LoadingPage::LoadingPage(QObject *parent):QAbstractListModel(parent)
{
    for(int x = 0 ; x < 10 ; ++x)
        addItem("this is test " + QString::number(x), false );

        mTimer = new QTimer(this);

    connect(mTimer, &QTimer::timeout, [=](){
        while(mLoadingDataItem[0].acceptionState){
            removeItem(0);
            mTimer->stop();
        }
//        changeAcceptionState(0,true);
    });
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
        return mLoadingDataItem[row].acceptionState;
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

void LoadingPage::addItem( const QString& loadingData, bool acceptionState)
{
    emit beginInsertRows(QModelIndex(), mLoadingDataItem.size(), mLoadingDataItem.size());
    mLoadingDataItem.push_back({loadingData,acceptionState});
    emit endInsertRows();
}

void LoadingPage::removeItem(int index)
{
    if(index < mLoadingDataItem.size()){
        emit beginRemoveRows(QModelIndex(),index,index);
        mLoadingDataItem.erase(mLoadingDataItem.begin() + index);
        emit endRemoveRows();
    }
    else
        return;
}

void LoadingPage::swapItem(int sourceIndex, int destinationIndex)
{
    if(sourceIndex < mLoadingDataItem.size() && destinationIndex < mLoadingDataItem.size()){
        emit beginMoveRows(QModelIndex(),sourceIndex,sourceIndex,QModelIndex(),destinationIndex);
        std::swap(mLoadingDataItem[sourceIndex],mLoadingDataItem[destinationIndex]);
        emit endMoveRows();
    }
    else
        return;

}

void LoadingPage::changeAcceptionState(int index, bool state)
{
    if(index < mLoadingDataItem.size()){
        mLoadingDataItem[index].acceptionState = state;
        QModelIndex modelIndex = createIndex(index, 0);
        emit dataChanged(modelIndex,modelIndex);
        updateData(index);
    }
    else
        return;

}

void LoadingPage::updateData(int index)
{
    for(int m=index;m>0;m--)
        if(mLoadingDataItem[m].acceptionState && !mLoadingDataItem[m-1].acceptionState)
            swapItem(m , m - 1);

    mTimer->start(1000);
}
