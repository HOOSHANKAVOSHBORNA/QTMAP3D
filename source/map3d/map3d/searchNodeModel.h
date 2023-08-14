#ifndef SEARCHNODEMODEL_H
#define SEARCHNODEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <osgEarthAnnotation/AnnotationNode>
#include <QSortFilterProxyModel>
#include "mapItem.h"
Q_DECLARE_METATYPE(QModelIndex)
class SearchNodeModel : public QAbstractListModel
{
    Q_OBJECT
public:
//    enum myRoles {
//        displayTextt = Qt::UserRole + 100
//    };
    SearchNodeModel(MapItem *mapItem, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
//    QHash<int, QByteArray> roleNames() const override;
    void addNode(osg::Node *node,osgEarth::Layer *layer);
    void removeNode( osg::Node *node,osgEarth::Layer *layer);
    void onNodeClicked(const QModelIndex &current);


private:
    void init();
private:
    MapItem *mMapItem{nullptr};
    std::vector<osg::ref_ptr<osg::Node>> mNodes;
};

//----------------------------------------------



class SearchNodeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchNodeProxyModel(QObject *parent = nullptr);
    QString filterString() const;
public slots:
    void setFilterString(const QString &filterString);
    void onNodeClicked(const int current);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

signals:
    void filterStringChanged();

private:
    QString mFilterString = "";

};

#endif // SEARCHNODEMODEL_H
