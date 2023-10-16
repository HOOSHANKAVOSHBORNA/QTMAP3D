#ifndef STATUSNODE_H
#define STATUSNODE_H

#include <QString>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/PlaceNode>
#include "mapItem.h"
#include "serviceManager.h"
#include <QPainter>
#include <osg/AutoTransform>
#include <osg/ref_ptr>


class StatusNode : public osgEarth::Annotation::PlaceNode
{
public:
    StatusNode(MapItem* mapControler);
    ~StatusNode() override;
    void setFieldData(QString title, const std::vector<NodeFieldData> &data);
    StatusNodeData *nodeData() const;
    void setNodeData(StatusNodeData *newNodeData);

private:
    void compile(const std::vector<NodeFieldData> &data);
private:
    QString mTitle;
    QImage *mTitleImage{nullptr};
    MapItem *mMapItem;
    StatusNodeData *mNodeData;
    QFont mFont;
};

#endif // STATUSNODE_H
