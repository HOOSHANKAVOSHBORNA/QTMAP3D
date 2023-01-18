#include "contextMenu.h"

ContextMenumodel::ContextMenumodel(QObject * parent):QAbstractListModel(parent)
{

}

int ContextMenumodel::rowCount(const QModelIndex &/*parent*/) const {
    return systemsList.size();
}

QVariant ContextMenumodel::data(const QModelIndex &index, int /*role*/) const{
    return systemsList.at(index.row());
}


QHash<int, QByteArray> ContextMenumodel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[TextToShow] = "disp";
    return hash;
}
