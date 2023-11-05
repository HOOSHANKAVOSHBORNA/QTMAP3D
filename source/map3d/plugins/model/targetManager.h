#ifndef TARGETMANAGER_H
#define TARGETMANAGER_H


#include "simpleModelNode.h"


class TargetManager
{
public:
    TargetManager();
    void updateAttackerList(QList<SimpleModelNode*> list);
    QList<SimpleModelNode*> getNearAttackers();
    int setNearAttacker(SimpleModelNode* AttackerNode);

//    void setFinalAttacker(SimpleModelNode* attackerNode);
//    osg::ref_ptr<SimpleModelNode> getFinalAttacker();


private:
    QList<SimpleModelNode*> mNearAttackers;
//    osg::ref_ptr<SimpleModelNode> mFinalAttacker;
};

#endif // TARGETMANAGER_H
