#include "combatMenu.h"
#include "qqmlcomponent.h"

AssignmentListModel::AssignmentListModel(CombatManager *combatManager, QObject *parent)
    :QAbstractListModel(parent),
    mCombatManager(combatManager)
{
    QObject::connect(combatManager, &CombatManager::dataChanged,[&]{

        beginResetModel();
        if(mIsAttacker)
            mAssignmentList = mCombatManager->getAttackerAssignments(mOperatorNode);
        else
            mAssignmentList = mCombatManager->getTargetAssignments(mOperatorNode);
        endResetModel();
    });
}

int AssignmentListModel::rowCount(const QModelIndex &parent) const
{
    return mAssignmentList.length();
}

QVariant AssignmentListModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Name:
        if(mIsAttacker)
            return  QString::fromStdString(mAssignmentList.at(index.row())->target->nodeData()->name);
        else
            return  QString::fromStdString(mAssignmentList.at(index.row())->attacker->nodeData()->name);
    case Icon:
        if(mIsAttacker)
            return  QUrl(QString::fromStdString(mAssignmentList.at(index.row())->target->nodeData()->iconSrc));
        else
            return  QUrl(QString::fromStdString(mAssignmentList.at(index.row())->attacker->nodeData()->iconSrc));
    case Color:
        if(mIsAttacker)
            return  QString::fromStdString(mAssignmentList.at(index.row())->target->nodeData()->color);
        else
            return  QString::fromStdString(mAssignmentList.at(index.row())->attacker->nodeData()->color);
    case State:
        return mAssignmentList.at(index.row())->state;
    case StateColor:
        return mAssignmentList.at(index.row())->getColor();
    case  Select:
        return mSelectedAssignmentList.contains(mAssignmentList.at(index.row()));
    default:
        break;
    }

    return QVariant("null");
}

QHash<int, QByteArray> AssignmentListModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[Name] = "objectID";
    hash[Icon] = "objectIcon";
    hash[Color] = "objectColor";
    hash[State] = "stateColor";
    hash[StateColor] = "stateColor";
    hash[Select] = "objectSelection";
    return hash;
}

void AssignmentListModel::setOperator(SimpleModelNode *node, bool isAttacker)
{
    mOperatorNode = node;
    beginResetModel();

    mIsAttacker = isAttacker;
    if(mIsAttacker)
        mAssignmentList = mCombatManager->getAttackerAssignments(node);
    else
        mAssignmentList = mCombatManager->getTargetAssignments(node);

    mSelectedAssignmentList.clear();
    endResetModel();
}

void AssignmentListModel::onAddButtonChecked(bool check)
{
    emit addAssignmentChecked(check, mOperatorNode, mIsAttacker);
}

void AssignmentListModel::onRemoveButtonChecked(bool check)
{
    emit removeAssignmentChecked(check, mOperatorNode, mIsAttacker);
}

void AssignmentListModel::onAttackButtonClicked()
{
    for(auto assignment: mSelectedAssignmentList){
        if(mAssignmentList.contains(assignment)){
            assignment->setState(ASSIGNED);
        }

    }
}

void AssignmentListModel::onCloseMenuClicked()
{

}

void AssignmentListModel::onMenuItemSelect(int row)
{
    auto assinment = mAssignmentList.at(row);
    if(!mSelectedAssignmentList.contains(assinment))
        mSelectedAssignmentList.append(mAssignmentList.at(row));
    else
        mSelectedAssignmentList.removeAll(mAssignmentList.at(row));
}
//-------------------------------------------------------------------------------
OperatorListModel::OperatorListModel(AssignmentListModel *assignmentListModel, QObject *parent)
    :QAbstractListModel(parent),
    mAssignmentListModel(assignmentListModel)
{

}

int OperatorListModel::rowCount(const QModelIndex &parent) const
{
    if(mIsAttacker)
        return mAttackerList.count();
    else
        return mTargetList.count();
}

QVariant OperatorListModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Name:
        if(mIsAttacker)
            return  QString::fromStdString(mAttackerList.at(index.row())->nodeData()->name);
        else
            return  QString::fromStdString(mTargetList.at(index.row())->nodeData()->name);
    case Icon:
        if(mIsAttacker)
            return  QUrl(QString::fromStdString(mAttackerList.at(index.row())->nodeData()->iconSrc));
        else
            return  QUrl(QString::fromStdString(mTargetList.at(index.row())->nodeData()->iconSrc));
    case Color:
        if(mIsAttacker)
            return  QString::fromStdString(mAttackerList.at(index.row())->nodeData()->color);
        else
            return  QString::fromStdString(mTargetList.at(index.row())->nodeData()->color);
    case  Select:
        if(mIsAttacker)
            return  mAttackerList.indexOf(mSelectedNode) == index.row();
        else
            return  mTargetList.indexOf(mSelectedNode) == index.row();
    default:
        break;
    }

    return QVariant("null");
}

QHash<int, QByteArray> OperatorListModel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractItemModel::roleNames();
    hash[Name] = "operatorName";
    hash[Icon] = "operatorIcon";
    hash[Color] = "operatorColor";
    hash[Select] = "operatorSelect";
    return hash;
}

void OperatorListModel::addAttacker(SimpleModelNode *node)
{
    beginResetModel();
    if(!mAttackerList.contains(node))
        mAttackerList.append(node);
    mIsAttacker = true;
    endResetModel();

    int row = mAttackerList.indexOf(node);
    select(row);

}

void OperatorListModel::addTarget(SimpleModelNode *node)
{
    beginResetModel();
    if(!mTargetList.contains(node))
        mTargetList.append(node);
     mIsAttacker = false;
    endResetModel();

    int row = mTargetList.indexOf(node);
    select(row);

}

void OperatorListModel::select(int row)
{
    beginResetModel();
    if(mIsAttacker){
        mSelectedNode = mAttackerList.at(row);
        mAssignmentListModel->setOperator(mSelectedNode, true);
    }
    else{
        mSelectedNode = mTargetList.at(row);
        mAssignmentListModel->setOperator(mSelectedNode, false);
    }
//    emit dataChanged(index(row), index(row));
    endResetModel();
}

//-----------------------------------------------------------------------------
CombatMenu::CombatMenu(CombatManager *combatManager, MapControllerItem *map)
{
    QQmlEngine *engine = QQmlEngine::contextForObject(map)->engine();
    mMapItem = map;
    mAssignmentListModel = new AssignmentListModel(combatManager);
    mOperatorListModel = new OperatorListModel(mAssignmentListModel);
    QQmlComponent* comp = new QQmlComponent(engine);

    QObject::connect(comp, &QQmlComponent::statusChanged, [&](QQmlComponent::Status status) {
        if(status == QQmlComponent::Error) {
            qDebug() << "Can not load this: " << comp->errorString();
        }

        if(status == QQmlComponent::Ready) {
            QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
            item->setProperty("assignmentListModel", QVariant::fromValue<AssignmentListModel*>(mAssignmentListModel));
            item->setProperty("operatorListModel", QVariant::fromValue<OperatorListModel*>(mOperatorListModel));

            map->setTopMenuItem(item);
            map->setTopMenuVisible(true);

        }
    });
    comp->loadUrl(QUrl("qrc:/resources/CombatMenu.qml"));
}


CombatMenu::~CombatMenu(){}

void CombatMenu::selectAttacker(SimpleModelNode *node)
{
    mOperatorListModel->addAttacker(node);
}

void CombatMenu::selectTarget(SimpleModelNode *node)
{
    mOperatorListModel->addTarget(node);
}

AssignmentListModel *CombatMenu::assignmentListModel() const
{
    return mAssignmentListModel;
}
