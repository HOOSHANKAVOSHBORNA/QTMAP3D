#ifndef TARGETMANAGER_H
#define TARGETMANAGER_H


#include "lineNode.h"
#include "simpleModelNode.h"


class TargetManager
{
public:
    TargetManager(MapItem *map ,SimpleModelNode *node);
    void updateAttackerList(QList<SimpleModelNode*> list);
    QList<SimpleModelNode*> getNearAttackers();
    int setNearAttacker(SimpleModelNode* AttackerNode);
    void showNearAttackers();
    void hideNearAttackers();


private:
    MapItem *mMapItem;
    QList<SimpleModelNode*> mNearAttackers;
    osg::ref_ptr<SimpleModelNode> mNode;
    osg::ref_ptr<osg::Group> mHighlightGroup;
};

#endif // TARGETMANAGER_H
