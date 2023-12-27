#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <osgEarth/Layer>

#include "mapItem.h"

Q_DECLARE_METATYPE(osgEarth::Layer);

class LayerModel;

class LayerManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QQuickItem *propertyItem READ propertyItem WRITE setPropertyItem NOTIFY
                   propertyItemChanged FINAL)
    Q_PROPERTY(QString propertyItemTitle READ propertyItemTitle WRITE setPropertyItemTitle NOTIFY
                   propertyItemTitleChanged FINAL)

public:
    static LayerManager* createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
    ~LayerManager() override;
    void setMapItem(MapItem *mapItem);
    Q_INVOKABLE LayerModel *layerModel() const;

    QQuickItem *propertyItem() const;
    void setPropertyItem(QQuickItem *newPropertyItem);
    void addPropertyItem(QQuickItem *newPropertyItem, QString title);
    Q_INVOKABLE void removePropertyItem();

    void createProperty();

    QString propertyItemTitle() const;
    void setPropertyItemTitle(const QString &newPropertyItemTitle);

signals:
    void propertyItemChanged();

    void propertyItemTitleChanged();

private:
    explicit LayerManager();

private:
    inline static LayerManager *mInstance;
    LayerModel *mLayerModel;
    QQuickItem *mPropertyItem = nullptr;
    QString mPropertyItemTitle;
};

class LayerModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString NOTIFY filterStringChanged FINAL)
    Q_PROPERTY(QModelIndex dragIndex READ getDragIndex WRITE setDragIndex NOTIFY filterStringChanged FINAL)

    enum Role {
        VisibleRole = Qt::UserRole + 100,
        LayerRole,
        DropRole
    };

public:
    explicit LayerModel();

    void setMapItem(MapItem *mapItem);
    MapItem *getMapItem();

    QHash<int, QByteArray> roleNames() const override;

    QModelIndex getDragIndex();
    QString filterString() const;

public slots:
    void setDragIndex(QModelIndex value);
    void setFilterString(const QString &newFilterString);
    void onVisibleItemClicked(const QModelIndex &current);
    void onItemLeftClicked(const QModelIndex &current);
    void onRemoveItemClicked(const QModelIndex &current);
    void onMoveItem(QModelIndex oldIndex, QModelIndex newIndex);

signals:
    void filterStringChanged();

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private slots:
    void resetModel();
    void onLayerAdded(osgEarth::Layer* layer, osgEarth::Layer *parentLayer, unsigned index);
    void onLayerRemoved(osgEarth::Layer* layer, osgEarth::Layer *parentLayer, unsigned index);
    void setLayerVisible(osgEarth::VisibleLayer *layer);
private:
    void moveItem(QModelIndex from , QModelIndex to);
    void setItemVisible(QStandardItem *item, bool visible);
    bool getLayerVisible(osgEarth::Layer *layer) const;

private:
    MapItem *mMapItem;
    QStandardItemModel *mSourceModel;
    QModelIndex mDragIndex;
    std::map<osgEarth::Layer*, QStandardItem*> mLayerToItemMap;
    QString mFilterString;
};


#endif // LAYERMANAGER_H
