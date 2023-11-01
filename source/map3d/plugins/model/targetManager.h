#ifndef TARGETMANAGER_H
#define TARGETMANAGER_H


#include "simpleModelNode.h"


class TargetManager
{
public:
    TargetManager(QList<SimpleModelNode*> list);
    QList<SimpleModelNode*> getNearInvaders();
    int setNearInvader(SimpleModelNode* invaderNode);
    void setFinalAttacker(SimpleModelNode* attackerNode);
    osg::ref_ptr<SimpleModelNode> getFinalAttacker();


private:
    QList<SimpleModelNode*> mNearInvaders;
    osg::ref_ptr<SimpleModelNode> mFinalAttacker;
};

#endif // TARGETMANAGER_H
