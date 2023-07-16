#ifndef MainWindow_H
#define MainWindow_H

#include <QQuickWindow>
#include <QQmlComponent>
#include <QVector3D>
#include <QTime>
#include <QPoint>

#include "pluginmanager.h"
#include "layerModel.h"
#include "mapItem.h"
#include "toolbox.h"
class QOpenGLFunctions_2_0;
class PluginInfo;
class PluginInterface;
class UIHandle;
class ListWindow;
class LayersModel;
//class LayersProxyModel;

Q_DECLARE_METATYPE(MapItem)
class MainWindow : public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(qreal fps READ fps WRITE setFps NOTIFY fpsChanged)
    Q_PROPERTY(LayersModel* layersModel READ layersModel WRITE setLayersModel NOTIFY layersModelChanged)
    Q_PROPERTY(ToolboxProxyModel* toolbox READ toolbox WRITE setToolbox NOTIFY layersModelChanged)

    enum class InfoWidgetType {
        Airplane,
        Station,
        System
    };

public:
    MainWindow(QWindow *parent = nullptr);
    ~MainWindow();

    qreal fps() const;

    LayersModel *layersModel() const;
    ToolboxProxyModel *toolbox() const;

    UIHandle *uiHandle() const;

signals:
    void sideItemCreated(int index, QObject *pSideItem);
    void toolboxItemCreated(ItemDescProxy *itemProxy);
    void fileItemCreated(ItemDescProxy *itemProxy);
    void toolboxItemClicked(const QString& itemName, const QString& categoryName);
    void fileItemClicked(const QString& itemName, const QString& categoryName);
    void toolboxItemCheckedChanged(const QString& itemName,
                                   const QString& categoryName,
                                   bool checked);

    void fpsChanged();
    void layersModelChanged();

public slots:
    void initializePluginsUI(std::list<PluginInfo>& pluginsInfoList);

    void setFps(qreal fps);

    void showInfoWidget(InfoWidgetType infoWidgetType);
    void updateInfoWidgetData(const QString& dataJSON);
//    void setStatusBarText(const QString message, int time);

    QQuickItem* wrapItemWithDockable(QQuickItem* item,const QString& title);
    void setCentralDockItem(QQuickItem* dockItem);
    void attachToCentralDockItem(QQuickItem* dockItem, bool horizontalAttach, bool attachAsFirst, qreal splitScale);
    void showInRightDock(QQuickItem *item);
    void showListWindow();

    void setLayersModel(LayersModel *layersModel);
//    void setLayersProxyModel(LayersProxyModel *layerProxyModel);
    void setToolbox(ToolboxProxyModel* toolbox);
    void onFrameSwapped();

public:

    MapItem* getMapItem();
    void setMapItem(MapItem& mapItem);


public slots:
//    void cleanup();
    void frame();
    void tickNavigation(double deltaTime);

    void restoreContext();
    void setListWindow(ListWindow *listWindow);

private:
    void initializeGL();

protected:
    void resizeEvent(QResizeEvent *) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    bool event(QEvent *ev) override;

private:
    QOpenGLFunctions_2_0 *mOGLF = nullptr;

    bool mResized     = false;

    int mViewportWidth = 0;
    int mViewportHeight = 0;

    bool mIsFirstFrame = true;


    QTime mLastMousePressTime = QTime::currentTime();
    QPoint mLastPressPoint = QPoint();

    QOpenGLContext *mContext = nullptr;
    QSurface       *mSurface = nullptr;


    bool mMousePressOusideClickProcess = false;
    bool mInClickProcess = false;

protected:
    MapItem *mMapItem = nullptr;

private:
    qreal mFps = 0.0f;

    UIHandle *mUIHandle = nullptr;
    ListWindow *mListWindow = nullptr;
    LayersModel *mLayersModel = nullptr;
    ToolboxProxyModel *mToolbox = nullptr;
//    LayersProxyModel *mLayersProxyModel = nullptr;
};

#endif // MainWindow_H
