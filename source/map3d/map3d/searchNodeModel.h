#ifndef SEARCHNODEMODEL_H
#define SEARCHNODEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <osgEarthAnnotation/AnnotationNode>

#include "mapObject.h"

class SearchNodeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum myRoles {
        displayTextt = Qt::UserRole + 100
    };
    SearchNodeModel(MapObject *mapObject, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    void addNode(osg::Node *node,osgEarth::Layer *layer) ;
    void removeNode( osg::Node *node,osgEarth::Layer *layer) ;


private:
    void init();
private:
    MapObject *mMapObject{nullptr};
    std::vector<osg::ref_ptr<osg::Node>> mNodes;
};

#endif // SEARCHNODEMODEL_H
