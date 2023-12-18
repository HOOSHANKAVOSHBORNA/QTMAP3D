#include "combatListModel.h"
#include "qqmlcomponent.h"

CombatListModel::CombatListModel(QObject *parent):
    QAbstractListModel(parent)
{

    // mRootItem = invisibleRootItem();
}

int CombatListModel::rowCount(const QModelIndex &parent) const
{
    return mAssignList.length();
}

QVariant CombatListModel::data(const QModelIndex &index, int role) const
{
    assignmentData asData = mAssignList.at(index.row());
    switch (role) {
    case ID:
        if(mIsAttacker){
            return  QString::number(asData.target->nodeData()->id);
        }else{
            return  QString::number(asData.attacker->nodeData()->id);
        }
    case icon:
        if(mIsAttacker){
            return QUrl::fromLocalFile(QString::fromStdString(asData.target->nodeData()->url2D));
        }else{
            return QUrl::fromLocalFile(QString::fromStdString(asData.attacker->nodeData()->url2D));
        }
    case selection:
        if(selectedItemIndex == index){
            return true;
        }else{
            return false;
        }
    case hover:
        return false;
    case stateColor:
    {
        switch (asData.getState()) {
        case PREASSIGN:
            return "#ff00ff";
            break;
        case ASSIGNED:
            return "#ff0000";
            break;
        case SEARCH:
            return "#000000";
            break;
        case LOCK:
            return "ffff00";
            break;
        case FIRE:
            return "#00ff00";
            break;
        case SUCCEED:
            return "#22ff22";
            break;
        case FAILED:
            return "#ffaa00";
            break;
        default:
            return "#ffffff";
            break;
        }
    }
    default:
        break;
    }

    return QVariant("null");
}

QHash<int, QByteArray> CombatListModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[ID] = "objectID";
    hash[icon] = "objectIcon";
    hash[selection] = "objectSelection";
    hash[stateColor] = "stateColor";
    return hash;
}

void CombatListModel::addData(assignmentData data)
{
    beginResetModel();
    mAssignList.append(data);
    endResetModel();
}

void CombatListModel::objectHover(QModelIndex index, bool isHovered)
{
    data(index,hover) = isHovered;
}

void CombatListModel::objectSelect(QModelIndex index)
{
    for (int var = 0; var < rowCount(); ++var) {
        data(index.siblingAtRow(var),selection) = false;
    }
    data(index,selection) = true;
    selectedItemIndex = index;
}

void CombatListModel::attackClicked() const
{
    if( mAssignList.at(selectedItemIndex.row()).getState() == PREASSIGN){
        mAssignList.at(selectedItemIndex.row()).setState(ASSIGNED);
    }
}

CombatList::CombatList(QQmlEngine *engine, MapControllerItem *map)
{
    mMapItem = map;
    mCombatListModel = new CombatListModel;
    QQmlComponent* comp = new QQmlComponent(engine);

    QObject::connect(comp, &QQmlComponent::statusChanged, [&](QQmlComponent::Status status) {
        if(status == QQmlComponent::Error) {
            qDebug() << "Can not load this: " << comp->errorString();
        }

        if(status == QQmlComponent::Ready) {
            QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
            item->setProperty("combatModel", QVariant::fromValue<CombatListModel*>(mCombatListModel));

            map->setTopMenuItem(item);

        }
    });
    comp->loadUrl(QUrl("qrc:/resources/CombatMenu.qml"));
}

CombatList::~CombatList(){}

CombatListModel *CombatList::getCombatModel() const
{
    return mCombatListModel;
}

void CombatList::setCombatMenuVisible(bool visible)
{
    mMapItem->setTopMenuVisible(visible);
}

void CombatListModel::setTitle(QString title)
{
    mTitle = title;
    emit actorDataUpdated();
}

QString CombatListModel::getTitle()
{
    return mTitle;
}

void CombatListModel::setIconUrl(QUrl iconUrl)
{
    mIconUrl = iconUrl;
    emit actorDataUpdated();
}

QUrl CombatListModel::getIconUrl()
{
    return mIconUrl;
}

void CombatListModel::setIsAttacker(bool isAttacker)
{
    mIsAttacker = isAttacker;
    emit actorDataUpdated();
}

bool CombatListModel::getIsAttacker()
{
    return mIsAttacker;
}

void CombatListModel::setBulletCount(int bulletCount)
{
    mBulletCount = bulletCount;
    emit actorDataUpdated();
}

int CombatListModel::getBulletCount()
{
    return mBulletCount;
}

