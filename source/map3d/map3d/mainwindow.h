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
#include "toolboxManager.h"
#include "locationManager.h"

class ListWindow;
class LayerModel;

Q_DECLARE_METATYPE(MapItem)
class MainWindow : public QObject
{
    Q_OBJECT

public:
    MainWindow(QWindow *parent = nullptr);
    ~MainWindow();

    void initComponent();
    QQmlEngine *getQmlEngine();
    Q_INVOKABLE MapControllerItem *getMapItem();
    Q_INVOKABLE ToolboxManager *getToolboxManager() const;
    LayerManager *getLayerManager() const;
    BookmarkManager *getBookmarkManager() const;
    Q_INVOKABLE LocationProxyModel *getLocationManager() const;

public:
    void addTabToListWindow(const QString tabTitle, QQuickItem *tabItem);

public slots:
    void showListWindow();
    void setListWindow(ListWindow *listWindow);

private:
    MapControllerItem *mMapItem = nullptr;
    ListWindow *mListWindow = nullptr;
    LocationManager *mLocationManager = nullptr;
    ToolboxManager *mToolboxManager = nullptr;
    //    LayersModel *mLayersModel = nullptr;
    //    BookmarkProxyModel *mBookmark = nullptr;
};

#endif // MainWindow_H
