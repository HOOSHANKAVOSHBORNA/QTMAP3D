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
    MainWindow(UserManager *userManager, QWindow *parent = nullptr);
    ~MainWindow();

    void initComponent();
    Q_INVOKABLE MapControllerItem *getMapItem();
    Q_INVOKABLE ToolboxManager *getToolboxManager() const;
    Q_INVOKABLE LayerManager *getLayerManager() const;
    Q_INVOKABLE BookmarkManager *getBookmarkManager() const;
    Q_INVOKABLE LocationProxyModel *getLocationManager() const;
    Q_INVOKABLE ListWindow *getListWindow() const;

    void addTabToListWindow(const QString tabTitle, QQuickItem *tabItem);
    void setListWindow(ListWindow *listWindow);
    void clearData();

private:
    MapControllerItem *mMapItem = nullptr;
    ListWindow *mListWindow = nullptr;
    LocationManager *mLocationManager = nullptr;
    ToolboxManager *mToolboxManager = nullptr;
    LayerManager *mLayerManager = nullptr;
    BookmarkManager *mBookmarkManager = nullptr;
};

#endif // MainWindow_H
