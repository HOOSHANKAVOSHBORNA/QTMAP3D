#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include "mapItem.h"
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <osgEarth/Layer>

Q_DECLARE_METATYPE(osgEarth::Layer);

class LayerModel;

class LayerManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    static LayerManager* createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);

    Q_INVOKABLE LayerModel *layerModel() const;

private:
    explicit LayerManager();

private:
    inline static LayerManager *mInstance;
    inline static LayerModel *mLayerModel;
};

class LayerModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString NOTIFY filterStringChanged FINAL)
    Q_PROPERTY(QModelIndex dragIndex READ getDragIndex WRITE setDragIndex)

    enum Role {
        VisibleRole = Qt::UserRole + 100,
        LayerRole,
        DropRole
    };

private:
    explicit LayerModel();

public:
    static LayerModel* createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);

    void initialize(MapItem *mapItem);
    void initializeModel(osgEarth::Map *map);
    QHash<int,QByteArray> roleNames() const override;
    QModelIndex getDragIndex();
    void setDragIndex(QModelIndex value);

    QString filterString() const;
    Q_INVOKABLE void setFilterString(const QString &newFilterString);

public slots:
    void onVisibleItemClicked(const QModelIndex &current);
    void onRemoveItemClicked(const QModelIndex &current);
    void onMoveItem(QModelIndex oldIndex, QModelIndex newIndex);

    void onLayerAdded(osgEarth::Layer* layer , osgEarth::Layer *parentLayer,   unsigned index);
    void onLayerRemoved(osgEarth::Layer* layer ,osgEarth::Layer *parentLayer, unsigned index);

signals:
    void filterStringChanged();

private:
    void moveItem(QModelIndex from , QModelIndex to);
    void setItemVisible(QStandardItem *item, bool visible);
    bool getLayerVisible(osgEarth::Layer *layer) const;
    void setLayerVisible(osgEarth::VisibleLayer *layer);

private:
    static LayerModel* mInstance;

//    QStandardItem mLayerList;
    MapItem *mMapItem;
    QStandardItemModel *mSourceModel;
//    QMap<QStandardItem , osgEarth::Layer*> treeLayerMap;
    QModelIndex mDragIndex;
    std::map<osgEarth::Layer*, QStandardItem*> mLayerToItemMap;

    // QSortFilterProxyModel interface
    QString mFilterString;

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};


#endif // LAYERMANAGER_H
