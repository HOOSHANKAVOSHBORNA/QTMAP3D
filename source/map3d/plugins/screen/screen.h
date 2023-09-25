#ifndef SCREEN_H
#define SCREEN_H

#include "plugininterface.h"
#include <QObject>
#include "snapShot.h"
#include "snipTool.h"

#define CATEGORY "Screen"
#define SNAPSHOT "Snap Shot"
#define SNIP     "Snip Tool"


class Screen: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "screen.json")
    Q_INTERFACES(PluginInterface)

public:
    Screen(QWidget *parent = nullptr);
    ~Screen()override{}
    bool setup() override;

private slots:
    void onSnapShotClicked();
    void onSnipToolClicked();


private:
    SnapShot *mSnapShot{nullptr};
    SnipToolInterface *mSnipTool{nullptr};
//    QQmlEngine *mEngine;

};



#endif // SCREEN_H
