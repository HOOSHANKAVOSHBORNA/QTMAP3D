#ifndef SEARCHNODEMODEL_H
#define SEARCHNODEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <osgEarthAnnotation/AnnotationNode>

class MapObject;

class SearchNodeModel : public QAbstractListModel
{
public:
    SearchNodeModel(MapObject *mapObject, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    void init();
private:
    MapObject *mMapObject{nullptr};
    std::vector<osg::ref_ptr<osg::Node>> mNodes;
};

#endif // SEARCHNODEMODEL_H
