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

class PluginInfo;
class PluginInterface;
class UIHandle;
class ListWindow;
class LayersModel;

Q_DECLARE_METATYPE(MapItem)
class MainWindow : public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(LayersModel* layersModel READ layersModel WRITE setLayersModel NOTIFY layersModelChanged)
    Q_PROPERTY(ToolboxProxyModel* toolbox READ toolbox WRITE setToolbox NOTIFY toolboxChanged)
public:
    enum class InfoWidgetType {
        Airplane,
        Station,
        System
    };
    enum DockPosition{
        Left = 0x1,
        Right = 0x2,
        Top = 0x3,
        Bottom = 0x4
    };

public:
    MainWindow(QWindow *parent = nullptr);
    ~MainWindow();

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

    void layersModelChanged();
    void toolboxChanged();

public slots:
    QQuickItem* wrapItemWithDockable(QQuickItem* item,const QString& title);
    void setCentralDockItem(QQuickItem* dockItem);
    void attachToCentralDockItem(QQuickItem* dockItem, bool horizontalAttach, bool attachAsFirst, qreal splitScale);
    void showInRightDock(QQuickItem *item);
    void addDockItem(QQuickItem* item, int position = DockPosition::Right);
    void showListWindow();

    void setLayersModel(LayersModel *layersModel);
    void setToolbox(ToolboxProxyModel* toolbox);

public:
    MapItem* getMapItem();
    void setMapItem(MapItem& mapItem);


public slots:
    void setListWindow(ListWindow *listWindow);

protected:
    bool event(QEvent *ev) override;

private:
    MapItem *mMapItem = nullptr;
    UIHandle *mUIHandle = nullptr;
    ListWindow *mListWindow = nullptr;
    LayersModel *mLayersModel = nullptr;
    ToolboxProxyModel *mToolbox = nullptr;
};

#endif // MainWindow_H
