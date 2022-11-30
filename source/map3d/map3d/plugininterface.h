
#ifndef PluginInterface_H
#define PluginInterface_H


#include <QObject>
#include <QString>

class QQmlEngine;
class QQmlComponent;
class MapController;

namespace osgViewer {
    class Viewer;
};

struct ToolboxItemDesc
{
    ToolboxItemDesc(
            QString _name      = QString(),
            QString _category  = QString(),
            QString _iconUrl   = QString(),
            bool    _checkable = false,
            bool    _hasMenu   = false,
            QString _menuUrl   = QString()) :
        name     (_name     ),
        category (_category ),
        iconUrl  (_iconUrl  ),
        checkable(_checkable),
        hasMenu  (_hasMenu  ),
        menuUrl  (_menuUrl  )
    {

    }

    QString name;
    QString category;
    QString iconUrl;
    bool    checkable = false;
    bool    hasMenu   = false;
    QString menuUrl;
};


struct PluginQMLDesc
{
    bool           pluginHasSideItem = false;
    QString        sideItemMenuBarTitle;
    QString        sideItemMenuBarIconUrl;
    QString        sideItemUrl;

    QList<ToolboxItemDesc*> toolboxItemsList;
};

class PluginInterface
{
public:
    virtual ~PluginInterface() { }

    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) = 0;
    virtual void onSideItemCreated(int index, QObject *sideItem) = 0;
    virtual void onToolboxItemClicked(const QString& name,
                                      const QString& category) = 0;

    virtual bool initialize3D(MapController *mapController) = 0;

};


QT_BEGIN_NAMESPACE
#define PluginInterface_iid "map3d.hooshan.ir"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)
QT_END_NAMESPACE

#endif // PluginInterface_H
