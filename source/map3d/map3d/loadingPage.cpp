#include "loadingPage.h"


LoadingPage::LoadingPage(QObject *parent):QAbstractListModel(parent)
{
//    for(int x = 0 ; x < 20 ; ++x)
//        addItem("this is test " + QString::number(x), true );
        addItem("this is test 1", true );
        addItem("this is test 2", true );
        addItem("this is test 3", false );
        addItem("this is test 4", true );
        addItem("this is test 5", true );
        addItem("this is test 6", true );
        addItem("this is test 7", false );
        addItem("this is test 8", false );
        addItem("this is test 9", false );
        addItem("this is test 10", false );

        mTimer = new QTimer(this);

    connect(mTimer, &QTimer::timeout, [=](){

        if(mLoadingDataItem[0].acceptionState && mLoadingDataItem.size()){
            removeItem(0);
            mTimer->stop();
        }
        updateData(mLoadingDataItem.size() - 1);
    });

    mTimer->start(5000);
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

void LoadingPage::updateData(int index)
{
    for(int m=index;m>0;m--)
        if(mLoadingDataItem[m].acceptionState && !mLoadingDataItem[m-1].acceptionState)
            swapItem(m , m - 1);

    mTimer->start(1000);
}
