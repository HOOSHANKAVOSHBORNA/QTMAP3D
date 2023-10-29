#ifndef MainWindow_H
#define MainWindow_H

#include <QQuickWindow>
#include <QQmlComponent>
#include <QVector3D>
#include <QTime>
#include <QPoint>

#include "layerManager.h"
#include "mapItem.h"
#include "mapControllerItem.h"
#include "bookmark.h"
#include "toolbox.h"
#include "locationManager.h"

class ListWindow;
class LayerModel;

Q_DECLARE_METATYPE(MapItem)
class MainWindow : public QQuickWindow
{
    Q_OBJECT
//    Q_PROPERTY(BookmarkProxyModel* bookmark READ bookmark /*WRITE setLayersModel NOTIFY layersModelChanged*/)

public:
    enum class InfoWidgetType {
        Airplane,
        Station,
        System
    };

    enum DockPosition{
        Left = 0x1,
        Right,
        Top,
        Bottom
    };

public:
    MainWindow(QWindow *parent = nullptr);
    ~MainWindow();
    void initComponent();
    QQmlEngine *getQmlEngine();
    MapItem *getMapItem();
    ToolboxProxyModel *getToolboxManager() const;
    LayerManager *getLayerManager() const;
    BookmarkProxyModel *getBookmarkManager() const;
    LocationProxyModel *getLocationManager() const;
public:
    void addToLeftContainer(QQuickItem *item, QString title);
    void addToRightContainer(QQuickItem *item, QString title);
    void addToCenterCenterContainer(QQuickItem *item);
    void removeFromRightContainer(QQuickItem *item);
    void removeFromLeftContainer(QQuickItem *item);

    void showInfoItem(QQuickItem* item, QString title);
    void hideInfoItem(QQuickItem* item);
    void hideProperty(QQuickItem* item);
    void addTabToListWindow(const QString tabTitle, QQuickItem *tabItem);

public slots:
    void showListWindow();
    void setListWindow(ListWindow *listWindow);

signals:

protected:
    bool event(QEvent *ev) override;

private:
    MapControllerItem *mMapItem = nullptr;
    ListWindow *mListWindow = nullptr;
//    LayersModel *mLayersModel = nullptr;
//    BookmarkProxyModel *mBookmark = nullptr;
};

#endif // MainWindow_H
