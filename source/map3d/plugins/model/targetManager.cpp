#include "targetManager.h"

TargetManager::TargetManager()
{

}

void TargetManager::updateAttackerList(QList<SimpleModelNode *> list)
{
    mNearAttackers = list;

}

QList<SimpleModelNode *> TargetManager::getNearAttackers()
{
    return mNearAttackers;
}

int TargetManager::setNearAttacker(SimpleModelNode *AttackerNode)
{
    mNearAttackers.push_back(AttackerNode);
    int AttackerID = mNearAttackers.indexOf(AttackerNode);
    return AttackerID;
}

