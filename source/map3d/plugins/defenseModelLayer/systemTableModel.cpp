#include "systemTableModel.h"
#include "systemDataManager.h"
#include "aircraftDataManager.h"

#include <QDebug>
#include <QColor>
#include <QRegularExpression>

SystemTableModel::SystemTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int SystemTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 22;
}

int SystemTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(mSystemInfosProxy.size());
}

QVariant SystemTableModel::data(const QModelIndex &index, int role) const
{
    if (index.row() > int(mSystemInfosProxy.size()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    {
        switch(index.column()) {
        case  0: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Number);
        case  1: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Name);
        case  2: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Type);
        case  3: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Terminal);
        case  4: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.TN);
        case  5: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Latitude);
        case  6: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Longitude);
        case  7: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.Altitude);
        case  8: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.ViewRange);
        case  9: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemInfo.MezRange);
        case 10: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.ReceiveTime);
        case 11: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.Simulation);
        case 12: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.BCCStatusToString());
        case 13: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.radarSearchStatusToString());
        case 14: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.operationalToString());
        case 15: return QVariant::fromValue<int>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.MissileCount);
        case 16: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemStatusInfo.RadarMode);
        case 17: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.Acceptance);
        case 18: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.phaseToString());
        case 19: return QVariant::fromValue<double>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.Antenna);
        case 20: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.ChanelNo);
        case 21: return QVariant::fromValue<QString>((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.Inrange);

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


    case SystemColor:
    {
        if (index.row() >= mSystemInfosProxy.size())
            return QVariant::fromValue<QColor>("white");
        return index.column() == 18 ? QVariant::fromValue<QColor>(QColor((*mSystemInfos)[mSystemInfosProxy[index.row()]]->information->systemCombatInfo.phaseToColor())):
            QVariant::fromValue<QColor>(QColor("white"));
    }

    }
    return QVariant();
}

QHash<int, QByteArray> SystemTableModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractTableModel::roleNames();
    hash[BackColorRole] = "d_bkcolor";
    hash[TextColorRole] = "d_txtcolor";
    hash[HeaderTextRole] = "d_headerTxt";
    hash[AssignColor] = "AssignColor";
    hash[SystemColor] = "SystemColor";
    return hash;
}

QString SystemTableModel::headerText(int column) const
{
    switch (column) {
    case  0: return QStringLiteral("Number");
    case  1: return QStringLiteral("Name");
    case  2: return QStringLiteral("Type");
    case  3: return QStringLiteral("Terminal");
    case  4: return QStringLiteral("TN");
    case  5: return QStringLiteral("Latitude");
    case  6: return QStringLiteral("Longitude");
    case  7: return QStringLiteral("Altitude");
    case  8: return QStringLiteral("ViewRange");
    case  9: return QStringLiteral("MezRange");
    case 10: return QStringLiteral("ReceiveTime");
    case 11: return QStringLiteral("Simulation");
    case 12: return QStringLiteral("BCCStatus");
    case 13: return QStringLiteral("RadarSearchStatus");
    case 14: return QStringLiteral("Operational");
    case 15: return QStringLiteral("MissileCount");
    case 16: return QStringLiteral("RadarMode");
    case 17: return QStringLiteral("Acceptance");
    case 18: return QStringLiteral("Phase");
    case 19: return QStringLiteral("Antenna");
    case 20: return QStringLiteral("ChanelNo");
    case 21: return QStringLiteral("Inrange");
    default: return QStringLiteral("");
    }
}

int SystemTableModel::getNumber(int row) const
{
    if (row < 0) return -1;

    if (row >= static_cast<int>(mSystemInfosProxy.size())) {
        return -1;
    }

    return (*mSystemInfos)[mSystemInfosProxy[row]]->information->systemInfo.Number;
}

void SystemTableModel::setFilterWildcard(const QString &wildcard)
{
    beginResetModel();

    mFilter = wildcard;
    mFilter.remove(QRegularExpression("\\s"));
    if (mTN == -1){
    mSystemInfosProxy.clear();
        for (auto& item : *mSystemInfos) {
            if (QString::number(item->information->systemInfo.Number).contains(mFilter))
                mSystemInfosProxy.push_back(item->information->systemInfo.Number);
        }
    }
    else
        updateAssignment(mTN);

    endResetModel();
}

void SystemTableModel::refresh()
{
    beginResetModel();
    mTN = -1;
    mSystemInfosProxy.clear();
    mSystemInfosProxy = mSystemInfos->keys();
    endResetModel();
}


void SystemTableModel::setSystemInfos(const QMap<int, System::Data*> &systems)
{
    beginResetModel();
    mSystemInfos = &systems;
    mSystemInfosProxy = mSystemInfos->keys();
    endResetModel();
}

void SystemTableModel::setAircraftInfos(const QMap<int, Aircraft::Data *> &info)
{
    mAircraftInfos = &info;
}

void SystemTableModel::updateAssignment(int /*tn*/)
{
    if (mTN != -1 && mAircraftInfos->contains(mTN)){
        beginResetModel();
        mSystemInfosProxy.clear();
        for (auto &system: (*mAircraftInfos)[mTN]->assignments){
            mSystemInfosProxy.append(system->info->systemInfo.Number);
        }
        endResetModel();
    }
}

void SystemTableModel::onInfoChanged(int number)
{
    if (mSystemInfos->contains(number) && !mSystemInfosProxy.contains(number)) {
        mSystemInfosProxy = mSystemInfos->keys();
        setFilterWildcard(mFilter);
    }

    else {
        int row = mSystemInfosProxy.indexOf(number);
        emit dataChanged(createIndex(row, 0), createIndex(row, 22));
    }
}

void SystemTableModel::onRemoveData(int number)
{
    if (!mSystemInfos->contains(number) && mSystemInfosProxy.contains(number)) {
        beginRemoveRows(QModelIndex(), mSystemInfosProxy.indexOf(number), mSystemInfosProxy.indexOf(number));
        mSystemInfosProxy = mSystemInfos->keys();
        endRemoveRows();
    }
}

void SystemTableModel::onSystemClicked(const int &number)
{
    emit systemClicked(number);
}

void SystemTableModel::onAircraftClicked(const int &tn)
{
    mTN = tn;
    updateAssignment(mTN);
}

SystemTable::SystemTable(SystemDataManager *systemDataManger, DefenseModelLayer *defenseModelLayer, QObject *parent) :
    QObject(parent),
    mSystemDataManger(systemDataManger),
    mDefenseModelLayer(defenseModelLayer)
{
    QQmlComponent *comp3 = new QQmlComponent(mDefenseModelLayer->qmlEngine());
    QObject::connect(comp3, &QQmlComponent::statusChanged, [this, comp3](){
        if (comp3->status() == QQmlComponent::Ready) {
            QQuickItem *systemTab = static_cast<QQuickItem*>(comp3->create(nullptr));
            mSystemTableModel = new SystemTableModel;
            mSystemTableModel->setSystemInfos(mSystemDataManger->getSystemsData());

            connect(mSystemDataManger, &SystemDataManager::infoChanged, mSystemTableModel, &SystemTableModel::onInfoChanged);
            QObject::connect(systemTab,
                             SIGNAL(filterTextChanged(const QString&)),
                             mSystemTableModel,
                             SLOT(setFilterWildcard(const QString&)));

            QObject::connect(systemTab,
                             SIGNAL(systemDoubleClicked(const int&)),
                             this,
                             SLOT(onDoubleClicked(const int&)));

            systemTab->setProperty("model", QVariant::fromValue<SystemTableModel*>(mSystemTableModel));
//            mDefenseModelLayer->uiHandle()->lwAddTab("Systems", systemTab);
        }

    });

    comp3->loadUrl(QUrl("qrc:///modelplugin/SystemTableView.qml"));
}

SystemTableModel *SystemTable::getModel() const
{
    return mSystemTableModel;
}

void SystemTable::onDoubleClicked(int number)
{
    if (mSystemDataManger->getSystemsData().contains(number))
		mDefenseModelLayer->selectModelNode(mSystemDataManger->getSystemsData()[number]->systemModelNode);
//        mSystemDataManger->getSystemsData()[number]->systemModelNode->onLeftButtonClicked(true);
}
