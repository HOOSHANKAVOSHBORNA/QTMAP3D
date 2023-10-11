#ifndef STATUSNODE_H
#define STATUSNODE_H

#include "qvariant.h"
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
    struct Data
    {
        QString name;
        QVariant value;
    };

    StatusNode(MapItem* mapControler);
    void setData(QString title, std::list<Data> *dataList);
    StatusNodeData *nodeData() const;
    void setNodeData(StatusNodeData *newNodeData);

private:
    void createImageNode();
private:
    QString mTitle;
    std::list<Data> *mDataList{nullptr};
    QImage *mRenderImage{nullptr};
    MapItem *mMapItem;
    QPainter *mPainter;
    StatusNodeData *mNodeData;
    QFont mFont;
};

#endif // STATUSNODE_H
