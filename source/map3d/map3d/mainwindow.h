#ifndef MainWindow_H
#define MainWindow_H

#include <QQuickWindow>
#include <QQmlComponent>
#include <QVector3D>
#include <QTime>
#include <QPoint>

#include "layerModel.h"
#include "mapItem.h"
#include "toolbox.h"

class ListWindow;
class LayersModel;

Q_DECLARE_METATYPE(MapItem)
class MainWindow : public QQuickWindow
{
    Q_OBJECT
    Q_PROPERTY(LayersModel* layersModel READ layersModel /*WRITE setLayersModel NOTIFY layersModelChanged*/)
    Q_PROPERTY(ToolboxProxyModel* toolbox READ toolbox /*WRITE setToolbox NOTIFY toolboxChanged*/)
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
    void initComponent();
    LayersModel *layersModel() const;
    ToolboxProxyModel *toolbox() const;
    MapItem* getMapItem();
public:
    void addToLeftContainer(QQuickItem *item, QString title);
    void addToRightContainer(QQuickItem *item, QString title);
    void addToCenterCenterContainer(QQuickItem *item);
    void removeFromRightContainer(QQuickItem *item);
    void removeFromLeftContainer(QQuickItem *item);

public slots:
    void showListWindow();
//    void setLayersModel(LayersModel *layersModel);
//    void setToolbox(ToolboxProxyModel* toolbox);
    void setListWindow(ListWindow *listWindow);

signals:
//    void layersModelChanged();
//    void toolboxChanged();

protected:
    bool event(QEvent *ev) override;

private:
    MapItem *mMapItem = nullptr;
    ListWindow *mListWindow = nullptr;
    LayersModel *mLayersModel = nullptr;
    ToolboxProxyModel *mToolbox = nullptr;
};

#endif // MainWindow_H
