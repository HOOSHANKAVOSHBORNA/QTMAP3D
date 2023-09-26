#ifndef STATUSNODE_H
#define STATUSNODE_H

#include "qvariant.h"
#include <QString>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/ModelNode>
#include "mapItem.h"
#include "serviceManager.h"
#include <QPainter>
#include <osg/AutoTransform>
#include <osg/ref_ptr>


class StatusNode : public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    struct Data
    {
        QString name;
        QVariant value;
    };

    StatusNode(MapItem* mapControler, QObject *parent = nullptr);
    void setData(QString title, std::list<Data> *dataList);
    NodeData *nodeData() const;
    void setNodeData(NodeData *newNodeData);

private:
    void createImageNode();
private:
    osg::ref_ptr<osg::AutoTransform> mAutoTransform;
    QString mTitle;
    std::list<Data> *mDataList{nullptr};
    QImage *mRenderImage{nullptr};
////    int LABEL_IMAGE_WIDTH = 70;
//    osg::ref_ptr<osg::Image> mStatusImg;
//    osg::ref_ptr<osg::Geode> mImgNode;
    MapItem *mMapItem;
    QPainter *mPainter;
    NodeData *mNodeData;
};

#endif // STATUSNODE_H
