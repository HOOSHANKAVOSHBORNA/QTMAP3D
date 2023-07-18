#include "simpleModelNode.h"
#include "model.h"
#include "modelAutoScaler.h"
//osg::ref_ptr<osg::Node> simpleNode;
simpleModelNode::simpleModelNode(MapItem *mapControler, const std::string &modelUrl, QObject *parent)
    : QObject{parent},
      osgEarth::Annotation::ModelNode(mapControler->getMapNode(), model::getDefaultStyle()),
    mModelUrl(modelUrl),
    mMapItem(mapControler)

{
    osg::ref_ptr<osg::Node> simpleNode = osgDB::readRefNodeFile(modelUrl);
    setCullingActive(false);
    addCullCallback(new ModelAutoScaler(2.5, 1, 1000));
    osgEarth::Symbology::Style  modelStyle;
    modelStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(simpleNode);
    setStyle(modelStyle);
}

simpleModelNode *simpleModelNode::getNewModel()
{
    return new simpleModelNode(mMapItem, mModelUrl);
}
