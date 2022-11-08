#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include "map3dlib.h"

class Map3dWidget;
class ToolBarWidget;
class WebSocketClient;

class PluginInterface: public QObject
{
    Q_OBJECT
public:
    PluginInterface(QObject *parent = nullptr);
    ~PluginInterface()override{}
    virtual void setUpUI() = 0;
    void setMainWindow(Map3dlib *mainWindow);
    void setMap3dWidget(Map3dWidget *value);
    void setToolBar(ToolBarWidget *toolBar);

    void setName(const QString &name);
    void createWebSocket();

protected:
    Map3dlib* mMainWindow{nullptr};
    Map3dWidget *mMap3dWidget{nullptr};
    ToolBarWidget* mToolBar{nullptr};
    WebSocketClient* mWebSocket{nullptr};
    QString mName;
};

QT_BEGIN_NAMESPACE
#define PluginInterface_iid "io.map3d.PluginInterface"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)
QT_END_NAMESPACE
#endif // PLUGININTERFACE_H
