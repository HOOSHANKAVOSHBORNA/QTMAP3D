#include "aircraftcontextmenumodel.h"

AircraftContextMenumodel::AircraftContextMenumodel(QObject * parent):QAbstractListModel(parent)
{

}

int AircraftContextMenumodel::rowCount(const QModelIndex &/*parent*/) const {
    return systemsList.size();
}

QVariant AircraftContextMenumodel::data(const QModelIndex &index, int /*role*/) const{
    return systemsList.at(index.row());
}


QHash<int, QByteArray> AircraftContextMenumodel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[TextToShow] = "disp";
    return hash;
}
