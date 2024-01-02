#ifndef ASSIGNDATAMANAGER_H
#define ASSIGNDATAMANAGER_H

#include "combatManager.h"
#include "model.h"
#include "qobject.h"

class AssignDataManager: public QObject
{
    Q_OBJECT
public:
    AssignDataManager(CombatManager *combatManager , Model *model);
    ~AssignDataManager();

public slots:
    void assignDataReceived(AssignData *assignData);

private:
    AssignState parseState(std::string state);
    void addOrUpdateAssignment(SimpleModelNode* attacker, SimpleModelNode* target, AssignState state);
    void removeAssignment(SimpleModelNode* attacker,SimpleModelNode* target);
    void removeAllAssignmentForAttacker(SimpleModelNode* attacker);
    void removeAllAssignmentForTarget(SimpleModelNode* target);

private:
    CombatManager *mCombatManagaer;
    Model *mModel;
    AssignData* mAssignData;
    static inline int mCount = 0;
};

#endif // ASSIGNDATAMANAGER_H
