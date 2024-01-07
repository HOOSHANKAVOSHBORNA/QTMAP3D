#include "assignDataManager.h"


AssignDataManager::AssignDataManager(CombatManager *combatManager, Model *model, ServiceManager *service)
    : mCombatManagaer(combatManager),
    mModel(model),
    mService(service)
{

}

void AssignDataManager::onSendAssignRequest(SimpleModelNode *attacker, SimpleModelNode *target)
{
    AssignData data;
    data.attackerID = attacker->nodeData()->id;
    data.targetID = target->nodeData()->id;
    data.command = "ASSIGNREQUEST";

    mService->sendJsonAssignData(data);
}

void AssignDataManager::onSendCancelRequest(SimpleModelNode *attacker, SimpleModelNode *target)
{
    AssignData data;
    data.attackerID = attacker->nodeData()->id;
    data.targetID = target->nodeData()->id;
    data.command = "CANCELREQUEST";

    mService->sendJsonAssignData(data);
}


void AssignDataManager::assignDataReceived(AssignData *assignData)
{
    QMap<int,osg::ref_ptr<SimpleModelNode>> *dataMap = mModel->getDataManager()->getNodeMap();
    SimpleModelNode *attackerNode = dataMap->find(assignData->attackerID).value();
    SimpleModelNode *targetNode = dataMap->find(assignData->targetID).value();


    AssignState state = parseState(assignData->state);

    if(assignData->command == "ADDORUPDATE")
        if(attackerNode && targetNode)
            mCombatManagaer->assign(attackerNode,targetNode,state);

    if(assignData->command == "REMOVE")
        removeAssignment(attackerNode,targetNode);

    if(assignData->command == "REMOVEATTACKER")
        removeAllAssignmentForAttacker(attackerNode);

    if(assignData->command == "REMOVETARGET")
        removeAllAssignmentForTarget(targetNode);

}

AssignState AssignDataManager::parseState(std::string state)
{
    if(state == "PREASSIGN"){
        return AssignState::PREASSIGN;
    }else if(state == "ASSIGNED"){
        return AssignState::ASSIGNED;
    }else if(state == "BUSY"){
        return AssignState::BUSY;
    }else if(state == "SEARCH"){
        return AssignState::SEARCH;
    }else if(state == "LOCK"){
        return AssignState::LOCK;
    }else if(state == "FIRE"){
        return AssignState::FIRE;
    }else if(state == "SUCCEED"){
        return AssignState::SUCCEED;
    }else if(state == "FAILED"){
        return AssignState::FAILED;
    }
    return PREASSIGN;
}

void AssignDataManager::addOrUpdateAssignment(SimpleModelNode* attacker,SimpleModelNode* target, AssignState state)
{
    mCombatManagaer->assign(attacker,target,state);
}

void AssignDataManager::removeAssignment(SimpleModelNode *attacker, SimpleModelNode *target)
{
    mCombatManagaer->removeAssignment(attacker,target);
}

void AssignDataManager::removeAllAssignmentForAttacker(SimpleModelNode *attacker)
{
    mCombatManagaer->removeAttackerAssignments(attacker);
}

void AssignDataManager::removeAllAssignmentForTarget(SimpleModelNode *target)
{
    mCombatManagaer->removeTargetAssignments(target);
}
