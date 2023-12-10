#ifndef SEARCHNODEMODEL_H
#define SEARCHNODEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <osgEarthAnnotation/AnnotationNode>
#include <QSortFilterProxyModel>
#include <osgEarthAnnotation/GeoPositionNode>
#include "mapItem.h"
#include "serviceManager.h"
Q_DECLARE_METATYPE(QModelIndex)

struct NodeInfo{
    osgEarth::Annotation::GeoPositionNode *node;
    NodeData *data;
};

class TypeListModel;
class SearchNodeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum myRoles{
        iD_ = Qt::UserRole,
        text_,
        type_
    };
    SearchNodeModel(MapItem *mapItem, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void onNodeClicked(const QModelIndex &current);
    QHash<int, QByteArray> roleNames() const override;


public slots:
    void addNode(osg::Node *node, osgEarth::Layer *layer);
    void removeNode( osg::Node *node,osgEarth::Layer *layer);

    TypeListModel *getTypeListModel() const;


private:
//    void init();
private:
    MapItem *mMapItem{nullptr};
    std::vector<osg::ref_ptr<osg::Node>> mNodes;
    std::vector<NodeInfo *> mNodes1;

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

    std::vector<QString> mTypes{};

};


//---------------

class SearchNodeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SearchNodeProxyModel(QObject *parent = nullptr);
    QString filterString() const;
    Q_INVOKABLE void toggleItem(const QString &itemText);


public slots:
    void setFilterString(const QString &filterString);
    void onNodeClicked(const int current);
    TypeListModel *getTypeListModel() const;
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

signals:
    void filterStringChanged();
public:
    std::vector<QString> myVector;
private:
    QString mFilterString = "";
    TypeListModel *mTypeListModel{nullptr};



 //----------------------


};

class SearchNodeManager : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    static SearchNodeManager* createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
    void setMapItem(MapItem *mapItem);
    Q_INVOKABLE SearchNodeProxyModel *searchNodeProxyModel() const;

private:
    explicit SearchNodeManager();


private:
    inline static SearchNodeManager *mInstance;
    SearchNodeProxyModel *mSearchNodeProxyModel;

};


#endif // SEARCHNODEMODEL_H
