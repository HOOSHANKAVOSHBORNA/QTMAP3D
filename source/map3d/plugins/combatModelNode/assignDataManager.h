#ifndef ASSIGNDATAMANAGER_H
#define ASSIGNDATAMANAGER_H

#include "combatManager.h"
#include "mainwindow.h"
#include "qobject.h"

class AssignDataManager: public QObject
{
    Q_OBJECT
public:
    AssignDataManager(CombatManager *combatManager);
    ~AssignDataManager();

public slots:
    void assignDataReceived(AssignData *assignData);


private:
    void addOrUpdateAssignment(AssignData *assignData);
    void removeAssignment(AssignData *assignData);
    void removeAllAssignmentForAttacker(int attackerID);
    void removeAllAssignmentForTarget(int targetID);

private:
    CombatManager *mCombatManagaer;
    AssignData* mAssignData;
    static inline int mCount = 0;
};

#endif // ASSIGNDATAMANAGER_H
