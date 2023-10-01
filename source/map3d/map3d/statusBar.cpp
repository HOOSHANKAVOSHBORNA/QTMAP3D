#include <QAbstractListModel>
#include <QObject>
#include "statusBar.h"

StatusBar::StatusBar(QObject *parent) : QAbstractListModel(parent)
{

}

int StatusBar::rowCount(const QModelIndex &parent) const
{
    return mMessages.size();
}

QVariant StatusBar::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case messageText:
        return mMessages[index.row()]->text;
        break;
    case dateText:
        return mMessages[index.row()]->dateTime.toString("dd/MM/yyyy");
        break;
    case timeText:
        return mMessages[index.row()]->dateTime.toString("h:m ap");
    case textChecked:
        return mMessages[index.row()]->isCheck;
    case isnewMessage:
        return mMessages[index.row()]->isNew;
    default:
        return QVariant();
    };
}


void StatusBar::removeMessage()
{
    for (int i = 0; i < mMessages.size();) {
        if(mMessages[i]->isCheck){
            beginRemoveRows(QModelIndex(),i,i);
            mMessages.erase(mMessages.begin()+i);
            endRemoveRows();
        } else {
            i++;
        }
    }


}



QHash<int, QByteArray> StatusBar::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[messageText] = "messageText";
    hash[dateText] = "dateText";
    hash[timeText] = "timeText";
    hash[textChecked] = "textChecked";
    hash[isnewMessage] = "isnewMessage";

    return hash;
}

void StatusBar::addMessage(Message *m)
{
    mMessages.push_back(m);
}

void StatusBar::isNewMessage(const QModelIndex &index)
{
    mMessages[index.row()]->isNew = false;
    QList<int> roles;
    roles.append(isnewMessage);
    emit dataChanged(index,index, roles);
}

void StatusBar::toggleCheck(const QModelIndex &index, bool check)
{
    mMessages[index.row()]->isCheck = check;
    QList<int> roles;
    roles.append(textChecked);
    emit dataChanged(index,index, roles);
}



//-------------------------------

/////////////////////////////////////////////////////////////////ProxyModel///////////////////////////////
StatusBarSearchModel::StatusBarSearchModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

bool StatusBarSearchModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    if (index.data(StatusBar::myRoles::messageText).toString().contains(mFilterString, Qt::CaseInsensitive))
        return true;
    return false;
}

QString StatusBarSearchModel::filterString() const
{
    return mFilterString;
}

int StatusBarSearchModel::getRange() const
{
    return mRange;
}

double StatusBarSearchModel::getScale() const
{
    return mScale;
}

void StatusBarSearchModel::setRange(const int range)
{
    mRange = range;
    emit rangeChanged();
}

void StatusBarSearchModel::setScale(const double scale)
{
    mScale = scale;
}

void StatusBarSearchModel::addMessage(QString Text, QDateTime time)
{
    Message *m = new Message{Text, time};
    dynamic_cast<StatusBar*>(sourceModel())->addMessage(m);
}

void StatusBarSearchModel::removeMessage()
{
    dynamic_cast<StatusBar*>(sourceModel())->removeMessage();

}

void StatusBarSearchModel::toggleCheck(const QModelIndex &index, bool check)
{

    dynamic_cast<StatusBar*>(sourceModel())->toggleCheck(mapToSource(index), check);
}

void StatusBarSearchModel::selectAllMessages(bool check)
{
    auto model = dynamic_cast<StatusBar*>(sourceModel());
    for(int i = 0; i < rowCount(); i++)
    {
        auto filterIndex = index(i, 0);
        auto sourceIndex = mapToSource(filterIndex);
        model->toggleCheck(sourceIndex, check);
    }

}

void StatusBarSearchModel::isNewMessage(const QModelIndex &index)
{
    dynamic_cast<StatusBar*>(sourceModel())->isNewMessage(mapToSource(index));
}

void StatusBarSearchModel::setFilterString(const QString &filterString)
{
    selectAllMessages(false);
    mFilterString = filterString;
    invalidateFilter();
}



