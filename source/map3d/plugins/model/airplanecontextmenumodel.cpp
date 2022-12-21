#include "airplanecontextmenumodel.h"

AirplaneContextMenumodel::AirplaneContextMenumodel(QObject * parent):QAbstractListModel(parent)
{

}

int AirplaneContextMenumodel::rowCount(const QModelIndex &parent) const {
    return a_list.size();
}

QVariant AirplaneContextMenumodel::data(const QModelIndex &index, int role) const{
    return a_list.at(index.row());
}
