#include "targetManager.h"

TargetManager::TargetManager(QList<SimpleModelNode*> list):
    mNearInvaders(list)
{

}

QList<SimpleModelNode *> TargetManager::getNearInvaders()
{
    return mNearInvaders;
}

int TargetManager::setNearInvader(SimpleModelNode *invaderNode)
{
    mNearInvaders.push_back(invaderNode);
    int invaderID = mNearInvaders.indexOf(invaderNode);
    return invaderID;
}

void TargetManager::setFinalAttacker(SimpleModelNode *attackerNode)
{
    mFinalAttacker = attackerNode;
}

osg::ref_ptr<SimpleModelNode> TargetManager::getFinalAttacker()
{
    return mFinalAttacker;
}




