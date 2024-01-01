#include "assignDataManager.h"


AssignDataManager::AssignDataManager(CombatManager *combatManager)
    : mCombatManagaer(combatManager)
{

}

void AssignDataManager::assignDataReceived(AssignData *assignData)
{
    if(assignData->command == "ADD"){
        addOrUpdateAssignment(assignData);
    }
    if(assignData->command == "REMOVE"){
        removeAssignment(assignData);
    }
    if(assignData->command == "REMOVEATTACKER"){
        removeAllAssignmentForAttacker(assignData->attackerID);
    }
    if(assignData->command == "REMOVETARGET"){
        removeAllAssignmentForTarget(assignData->targetID);
    }
}

void AssignDataManager::addOrUpdateAssignment(AssignData *assignData)
{

}

void AssignDataManager::removeAssignment(AssignData *assignData)
{

}

void AssignDataManager::removeAllAssignmentForAttacker(std::string attackerID)
{

}

void AssignDataManager::removeAllAssignmentForTarget(std::string targetID)
{

}
