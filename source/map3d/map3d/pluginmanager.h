
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

class ToolboxItemDescProxy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name      READ name      WRITE setName      NOTIFY nameChanged     )
    Q_PROPERTY(QString category  READ category  WRITE setCategory  NOTIFY categoryChanged )
    Q_PROPERTY(QString iconUrl   READ iconUrl   WRITE setIconUrl   NOTIFY iconUrlChanged  )
    Q_PROPERTY(bool    checkable READ checkable WRITE setCheckable NOTIFY checkableChanged)
    Q_PROPERTY(bool    hasMenu   READ hasMenu   WRITE setHasMenu   NOTIFY hasMenuChanged  )
    Q_PROPERTY(QString menuUrl   READ menuUrl   WRITE setMenuUrl   NOTIFY menuUrlChanged  )


public:
    ToolboxItemDescProxy(const ToolboxItemDesc& desc) :
        _name     (desc.name     ),
        _category (desc.category ),
        _iconUrl  (desc.iconUrl  ),
        _checkable(desc.checkable),
        _hasMenu  (desc.hasMenu  ),
        _menuUrl  (desc.menuUrl  )
    {

    }

    QString name     (){return _name;     }
    QString category (){return _category; }
    QString iconUrl  (){return _iconUrl;  }
    bool    checkable(){return _checkable;}
    bool    hasMenu  (){return _hasMenu;  }
    QString menuUrl  (){return _menuUrl;  }

signals:
    void nameChanged     ();
    void categoryChanged ();
    void iconUrlChanged  ();
    void checkableChanged();
    void hasMenuChanged  ();
    void menuUrlChanged  ();

public slots:
    void setName     (const QString& name) {
        if (name != _name) {
            _name = name;
            emit nameChanged();
        }
    }
    void setCategory (const QString& category) {
        if (category != _category) {
            _category = category;
            emit categoryChanged();
        }
    }
    void setIconUrl  (const QString& iconUrl) {
        if (iconUrl != _iconUrl) {
            _iconUrl = iconUrl;
            emit iconUrlChanged();
        }
    }
    void setCheckable(bool checkable) {
        if (checkable != _checkable) {
            _checkable = checkable;
            emit checkableChanged();
        }
    }
    void setHasMenu  (bool hasMenu) {
        if (hasMenu != _hasMenu) {
            _hasMenu = hasMenu;
            emit hasMenuChanged();
        }
    }
    void setMenuUrl  (const QString& menuUrl) {
        if (menuUrl != _menuUrl) {
            _menuUrl = menuUrl;
            emit menuUrlChanged();
        }
    }

private:
    QString _name;
    QString _category;
    QString _iconUrl;
    bool    _checkable = false;
    bool    _hasMenu   = false;
    QString _menuUrl;
};

struct CrystalPluginInfo
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
    void performPluginsInit3D(MapController *mapController);

    std::list<CrystalPluginInfo>& pluginsInfoList();

public slots:
    void onSideItemCreated(int index, QObject *sideItem);
    void onToolboxItemCreated(const QString& name,
                              const QString& category,
                              QObject *menuItem,
                              PluginInterface *interface);
    void onToolboxItemClicked(const QString& name,
                              const QString& category);

private:
    std::list<CrystalPluginInfo> mPluginsInfoList;
    QMap<QString, QMap<QString, PluginInterface*>> mToolboxItemsMap;
};

#endif // PluginManager_H
