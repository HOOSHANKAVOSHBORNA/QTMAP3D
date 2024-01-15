#ifndef ASSIGNDATAMANAGER_H
#define ASSIGNDATAMANAGER_H

#include "combatManager.h"
#include "qobject.h"
#include "model.h"



class AssignDataManager: public QObject
{
    Q_OBJECT
public:
    AssignDataManager(CombatManager *combatManager , Model *model, ServiceManager *service);

public slots:
    void assignDataReceived(const AssignmentData &assignData);
    void onSendAssignRequest(SimpleModelNode *attacker, SimpleModelNode *target);
    void onSendCancelRequest(SimpleModelNode *attacker, SimpleModelNode *target);

private:
    AssignState parseState(std::string state);
    void addOrUpdateAssignment(SimpleModelNode* attacker, SimpleModelNode* target, AssignState state);
    void removeAssignment(SimpleModelNode* attacker,SimpleModelNode* target);
    void removeAllAssignmentForAttacker(SimpleModelNode* attacker);
    void removeAllAssignmentForTarget(SimpleModelNode* target);

private:
    CombatManager *mCombatManagaer;
    ServiceManager *mService;
    Model *mModel;
};

#endif // ASSIGNDATAMANAGER_H
