#ifndef STATUSNODE_H
#define STATUSNODE_H

#include "qvariant.h"
#include <QString>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgEarthAnnotation/ModelNode>
#include "mapItem.h"


class StatusNode : public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    StatusNode(MapItem* mapControler, QObject *parent = nullptr);
    MapItem *mapItem() const;
    struct Data
    {
        QString name;
        QVariant value;
    };
    void setData(QString title, std::list<Data> dataList);
private:
    QString mTitle;
    std::list<Data> mDataList;
    QImage *mRenderImage{nullptr};
    static constexpr int LABEL_IMAGE_WIDTH = 70;
    void updateStatusData();
    osg::ref_ptr<osg::Image> mStatusImg;
    osg::ref_ptr<osg::Geode> mImgNode;
    MapItem *mMapItem;
};

#endif // STATUSNODE_H
