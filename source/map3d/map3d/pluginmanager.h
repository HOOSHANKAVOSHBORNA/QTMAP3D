
#ifndef PluginManager_H
#define PluginManager_H

#include <QObject>
#include <QQmlEngine>
#include <list>
#include <QMap>
#include <utility>
#include <tuple>

#include "plugininterface.h"

class MapController;
class PluginInterface;

class ItemDescProxy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name      READ name     )
    Q_PROPERTY(QString category  READ category )
    Q_PROPERTY(QString iconUrl   READ iconUrl  )
    Q_PROPERTY(bool    checkable READ checkable)
    Q_PROPERTY(bool    hasMenu   READ hasMenu  )
    Q_PROPERTY(QString menuUrl   READ menuUrl  )
    Q_PROPERTY(PluginInterface* pluginInterface READ pluginInterface)


public:
    ItemDescProxy(const ItemDesc& desc, PluginInterface *interface = nullptr) :
        _name     (desc.name     ),
        _category (desc.category ),
        _iconUrl  (desc.iconUrl  ),
        _checkable(desc.checkable),
        _hasMenu  (desc.hasMenu  ),
        _menuUrl  (desc.menuUrl  ),
        _pluginInterface(interface)
    {

    }

    QString name     (){return _name;     }
    QString category (){return _category; }
    QString iconUrl  (){return _iconUrl;  }
    bool    checkable(){return _checkable;}
    bool    hasMenu  (){return _hasMenu;  }
    QString menuUrl  (){return _menuUrl;  }
    PluginInterface *pluginInterface() { return _pluginInterface; }

private:
    QString _name;
    QString _category;
    QString _iconUrl;
    bool    _checkable = false;
    bool    _hasMenu   = false;
    QString _menuUrl;
    PluginInterface *_pluginInterface = nullptr;
};

struct PluginInfo
{
    PluginInterface *interface = nullptr;
    PluginQMLDesc   *qmlDesc    = nullptr;
    int              sideItemIndex      = -1;
};

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QObject *parent = nullptr);

public:
    void loadPlugins();
    void performPluginsInitQMLDesc(QQmlEngine *qmlEngine);
    void performPluginsSetup(MapItem *mapController);

    std::list<PluginInfo>& pluginsInfoList();

public slots:
    void onSideItemCreated(int index, QObject *sideItem);
    void onToolboxItemCreated(ItemDescProxy *itemProxy);
    void onFileItemCreated(ItemDescProxy *itemProxy);
    void onToolboxItemClicked(const QString& name,
                              const QString& category);
    void onToolboxItemCheckedChanged(const QString& name,
                                     const QString& category,
                                     bool checked);
    void onFileItemClicked(const QString& name,
                           const QString& category);


public:
//    void frameEvent           ();
//    void keyPressEvent        (QKeyEvent* event);
//    void keyReleaseEvent      (QKeyEvent* event);
//    void mousePressEvent      (QMouseEvent* event);
//    void mouseReleaseEvent    (QMouseEvent* event);
//    void mouseDoubleClickEvent(QMouseEvent* event);
//    void mouseMoveEvent       (QMouseEvent* event);
//    void wheelEvent           (QWheelEvent* event);


private:
    std::list<PluginInfo> mPluginsInfoList;
    QMap<QString, QMap<QString, PluginInterface*>> mToolboxItemsMap;
    QMap<QString, QMap<QString, PluginInterface*>> mFileItemsMap;
};

#endif // PluginManager_H
