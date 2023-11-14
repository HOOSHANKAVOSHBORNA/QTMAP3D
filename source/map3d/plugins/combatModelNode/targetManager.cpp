#include "targetManager.h"

TargetManager::TargetManager(MapItem *map, SimpleModelNode *node):
    mMapItem(map),
    mNode(node)
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

void TargetManager::showNearAttackers()
{
    mHighlightGroup = new osg::Group;
    for (int var = 0; var < mNearAttackers.length(); ++var) {
        osg::ref_ptr<LineNode> line = new LineNode(mMapItem);
        line->addPoint(mNode->getPosition());
        line->addPoint(mNearAttackers.at(var)->getPosition());
        line->setFillColor(osgEarth::Color::Red);
        mHighlightGroup->addChild(line);
        mNearAttackers.at(var)->selectModel();
    }
    mNode->addChild(mHighlightGroup);
}

void TargetManager::hideNearAttackers()
{
    if(mNode->containsNode(mHighlightGroup)){
        for (int var = 0; var < mNearAttackers.length(); ++var) {
            mNearAttackers.at(var)->selectModel();
        }
        mNode->removeChild(mHighlightGroup);
    }
}

