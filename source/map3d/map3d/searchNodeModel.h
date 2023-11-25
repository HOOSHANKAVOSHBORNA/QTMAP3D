#ifndef SEARCHNODEMODEL_H
#define SEARCHNODEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <osgEarthAnnotation/AnnotationNode>
#include <QSortFilterProxyModel>
#include "mapItem.h"
Q_DECLARE_METATYPE(QModelIndex)

class TypeListModel;
class SearchNodeModel : public QAbstractListModel
{
    Q_OBJECT
public:

    SearchNodeModel(MapItem *mapItem, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void onNodeClicked(const QModelIndex &current);

public slots:
    void addNode(osg::Node *node,osgEarth::Layer *layer);
    void removeNode( osg::Node *node,osgEarth::Layer *layer);
    TypeListModel *getTypeListModel() const;


private:
    void init();
private:
    MapItem *mMapItem{nullptr};
    std::vector<osg::ref_ptr<osg::Node>> mNodes;
    TypeListModel *mTypeListModel{nullptr};
};

//----------------------------------------------
class TypeListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    TypeListModel(QObject *parent = nullptr);
    void append(QString type);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //    void addNode(osg::Node *node,osgEarth::Layer *layer);


private:
    std::vector<QString> mTypes{};

};


//---------------

class SearchNodeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchNodeProxyModel(QObject *parent = nullptr);
    QString filterString() const;
public slots:
    void setFilterString(const QString &filterString);
    void onNodeClicked(const int current);
    TypeListModel *getTypeListModel() const;
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

signals:
    void filterStringChanged();

private:
    QString mFilterString = "";
    TypeListModel *mTypeListModel{nullptr};

};

// -------------  Manager
//class searchNodeManager : public QObject
//{
//    Q_OBJECT

//    void addNode(osg::Node *node,osgEarth::Layer *layer);

//private:
//    MapItem *mMapItem{nullptr};

//    SearchNodeProxyModel *mSearchNodeProxyModel;
//};





#endif // SEARCHNODEMODEL_H
