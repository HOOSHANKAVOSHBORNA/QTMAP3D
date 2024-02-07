#include "loadingInfo.h"

LoadingInfo::LoadingInfo(QObject *parent):QAbstractListModel(parent)
{
    for(int x = 0 ; x < 60 ; ++x)
        addItem("this is test " + QString::number(x), x%2 );
}

int LoadingInfo::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mLoadingDataItem.size();
}

QVariant LoadingInfo::data(const QModelIndex &index, int role) const
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

QHash<int, QByteArray> LoadingInfo::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[acceptionState] = "acceptionState";
    return hash;
}

void LoadingInfo::addItem( const QString& loadingData, bool acceptionState)
{

    emit beginInsertRows(QModelIndex(), mLoadingDataItem.size(), mLoadingDataItem.size());
    mLoadingDataItem.push_back({loadingData,acceptionState});
    emit endInsertRows();
}

void LoadingInfo::removeItem(int index)
{
    emit beginRemoveRows(QModelIndex(),index,index);
    mLoadingDataItem.erase(mLoadingDataItem.begin() + index);
    emit endRemoveRows();
}
