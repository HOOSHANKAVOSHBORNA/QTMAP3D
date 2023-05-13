#include "stationTableModel.h"
#include "stationDataManager.h"
#include <QColor>
#include <QRegularExpression>

StationTableModel::StationTableModel(QObject *parent): QAbstractTableModel(parent)
{

}

int StationTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 9;
}

int StationTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(mStationInfosProxy.size());
}

QVariant StationTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mStationInfosProxy.size()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        switch(index.column()) {
        case  0: return QVariant::fromValue<double>((*mStationInfos)[mStationInfosProxy[index.row()]]->info.Number);
        case  1: return QVariant::fromValue<QString>((*mStationInfos)[mStationInfosProxy[index.row()]]->info.Name);
        case  2: return QVariant::fromValue<QString>((*mStationInfos)[mStationInfosProxy[index.row()]]->info.Type);
        case  3: return QVariant::fromValue<QString>((*mStationInfos)[mStationInfosProxy[index.row()]]->info.PrimSec);
        case  4: return QVariant::fromValue<double>((*mStationInfos)[mStationInfosProxy[index.row()]]->info.Latitude);
        case  5: return QVariant::fromValue<double>((*mStationInfos)[mStationInfosProxy[index.row()]]->info.Longitude);
        case  6: return QVariant::fromValue<double>((*mStationInfos)[mStationInfosProxy[index.row()]]->info.Radius);
        case  7: return QVariant::fromValue<int>((*mStationInfos)[mStationInfosProxy[index.row()]]->info.CycleTime);
        }

        break;
    }

    case BackColorRole:
    {
        return QVariant::fromValue<QColor>(QColor("transparent"));
    }

    case TextColorRole:
    {
        return QVariant::fromValue<QColor>(QColor("white"));
    }

    case HeaderTextRole:
    {
        switch(index.row()) {
        case  0: return QVariant::fromValue<QString>("Number");
        case  1: return QVariant::fromValue<QString>("Name");
        case  2: return QVariant::fromValue<QString>("Type");
        case  3: return QVariant::fromValue<QString>("Primary/Secondry");
        case  4: return QVariant::fromValue<QString>("Latitude");
        case  5: return QVariant::fromValue<QString>("Longitude");
        case  6: return QVariant::fromValue<QString>("Radius");
        case  7: return QVariant::fromValue<QString>("CycleTime");
        }
        break;
    }
    }

    return QVariant();
}

QHash<int, QByteArray> StationTableModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractTableModel::roleNames();
    hash[BackColorRole] = "d_bkcolor";
    hash[TextColorRole] = "d_txtcolor";
    hash[HeaderTextRole] = "d_headerTxt";
    return hash;
}


QString StationTableModel::headerText(int column) const
{
    switch(column) {
    case  0: return QStringLiteral("Number");
    case  1: return QStringLiteral("Name");
    case  2: return QStringLiteral("Type");
    case  3: return QStringLiteral("Primary/Secondary");
    case  4: return QStringLiteral("Latitude");
    case  5: return QStringLiteral("Longitude");
    case  6: return QStringLiteral("Radius");
    case  7: return QStringLiteral("CycleTime");
    }

    return QStringLiteral("");
}

int StationTableModel::getNumber(int row) const
{
    if (row < 0) return -1;

    if (row >= static_cast<int>(mStationInfosProxy.size())) {
        return -1;
    }

    return(*mStationInfos)[mStationInfosProxy[row]]->info.Number;
}

void StationTableModel::setFilterWildcard(const QString &wildcard)
{
    beginResetModel();

    mFilter = wildcard;
    mFilter.remove(QRegularExpression("\\s"));

    mStationInfosProxy.clear();
    for (auto& item : *mStationInfos) {
        if (QString::number(item->info.Number).contains(mFilter))
            mStationInfosProxy.push_back(item->info.Number);
    }

    endResetModel();
}

void StationTableModel::onInfoChanged(int number)
{
//    if (!mStationInfos->contains(number) && mStationInfosProxy.contains(number)) {
//        beginRemoveRows(QModelIndex(), mStationInfosProxy.indexOf(number), mStationInfosProxy.indexOf(number));
//        mStationInfosProxy = mStationInfos->keys();
//        endRemoveRows();
//    }

    if (mStationInfos->contains(number) && !mStationInfosProxy.contains(number)) {
        mStationInfosProxy = mStationInfos->keys();
        setFilterWildcard(mFilter);
    }

    else {
        int row = mStationInfosProxy.indexOf(number);
        emit dataChanged(createIndex(row, 0), createIndex(row, 22));
    }
}

void StationTableModel::setStationInfos(const QMap<int, Station::Data *> &stationInfos)
{
    beginResetModel();
    mStationInfos = &stationInfos;
    mStationInfosProxy = mStationInfos->keys();
    endResetModel();
}

StationTable::StationTable(StationDataManager *stationDataManager, DefenseModelLayer *defenseModelLayer, QObject *parent) :
    QObject(parent),
    mStationDataManager(stationDataManager),
    mDefenseModelLayer(defenseModelLayer)
{
    QQmlComponent *comp2 = new QQmlComponent(mDefenseModelLayer->mQmlEngine);
    QObject::connect(comp2, &QQmlComponent::statusChanged, [this, comp2](){
        if (comp2->status() == QQmlComponent::Ready) {
            QQuickItem *stationTab = static_cast<QQuickItem*>(comp2->create(nullptr));
            mStationTableModel = new StationTableModel;
            mStationTableModel->setStationInfos(mStationDataManager->getStationsData());
            connect(mStationDataManager, &StationDataManager::infoChanged, mStationTableModel, &StationTableModel::onInfoChanged);
            QObject::connect(stationTab,
                             SIGNAL(filterTextChanged(const QString&)),
                             mStationTableModel,
                             SLOT(setFilterWildcard(const QString&)));

            QObject::connect(stationTab,
                             SIGNAL(stationDoubleClicked(const int&)),
                             this,
                             SLOT(onDoubleClicked(const int&)));


            stationTab->setProperty("model", QVariant::fromValue<StationTableModel*>(mStationTableModel));
            mDefenseModelLayer->mUIHandle->lwAddTab("Stations", stationTab);
        }

    });

    comp2->loadUrl(QUrl("qrc:///modelplugin/StationTableView.qml"));
}

void StationTable::onDoubleClicked(const int &number)
{
    if (mStationDataManager->getStationsData().contains(number))
        mStationDataManager->getStationsData()[number]->modelNode->onLeftButtonClicked(true);
}
