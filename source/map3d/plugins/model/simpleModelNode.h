#ifndef SIMPLEMODELNODE_H
#define SIMPLEMODELNODE_H
#include "mapItem.h"
#include <QObject>

#include <osgEarthAnnotation/ModelNode>

class simpleModelNode : public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    simpleModelNode(MapItem* mapControler, const std::string& modelUrl, QObject *parent = nullptr);
    virtual simpleModelNode* getNewModel();
protected:
    std::string mModelUrl;
    MapItem *mMapItem;
private:


signals:

};

#endif // SIMPLEMODELNODE_H
