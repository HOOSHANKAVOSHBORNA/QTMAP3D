#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QWidget>

class Map3dWidget;
class ToolBarWidget;

class PluginInterface: public QWidget
{
    Q_OBJECT
public:
    PluginInterface(QWidget *parent = nullptr);
    virtual void setUpUI() = 0;
    void setMap3dWidget(Map3dWidget *value);

    void setToolBar(ToolBarWidget *toolBar);

protected:
    Map3dWidget *mMap3dWidget;
    ToolBarWidget* mToolBar;
};

QT_BEGIN_NAMESPACE
#define PluginInterface_iid "io.map3d.PluginInterface"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)
QT_END_NAMESPACE
#endif // PLUGININTERFACE_H
