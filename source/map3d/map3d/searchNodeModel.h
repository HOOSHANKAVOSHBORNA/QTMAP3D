#ifndef SEARCHNODEMODEL_H
#define SEARCHNODEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <osgEarthAnnotation/AnnotationNode>
#include <QSortFilterProxyModel>
#include "mapObject.h"

class SearchNodeModel : public QAbstractListModel
{
    Q_OBJECT
public:
//    enum myRoles {
//        displayTextt = Qt::UserRole + 100
//    };
    SearchNodeModel(MapObject *mapObject, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
//    QHash<int, QByteArray> roleNames() const override;
    void addNode(osg::Node *node,osgEarth::Layer *layer) ;
    void removeNode( osg::Node *node,osgEarth::Layer *layer) ;


private:
    void init();
private:
    MapObject *mMapObject{nullptr};
    std::vector<osg::ref_ptr<osg::Node>> mNodes;
};

//----------------------------------------------



class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortFilterProxyModel(QObject *parent = nullptr);
    QString filterString() const;
public slots:
    void setFilterString(const QString &filterString);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

signals:
    void filterStringChanged();

private:
    QString mFilterString = "";

};

#endif // SEARCHNODEMODEL_H
