
#ifndef PluginInterface_H
#define PluginInterface_H


#include <QObject>
#include <QString>

class QQmlEngine;
class QQmlComponent;
class MapController;
class NetworkManager;

namespace osgViewer {
    class Viewer;
};

class InfoWidgetHandle
{
    enum class InfoWidgetType {
        Airplane,
        Station,
        System
    };

public:
    InfoWidgetHandle() { }
    virtual ~InfoWidgetHandle() { }

public:
    void showInfoWidget(InfoWidgetType infoWidgetType);
    void updateData(const QString& infoJSON);
};

struct ItemDesc
{
    ItemDesc(
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

    QList<ItemDesc*> toolboxItemsList;
    QList<ItemDesc*> fileItemsList;
};

class PluginInterface : public QObject
{
    Q_OBJECT

public:
    PluginInterface(QObject *parent = nullptr) : QObject(parent) { }
    virtual ~PluginInterface() { }

    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) {}
    virtual void onSideItemCreated(int index, QObject *sideItem) {}
    virtual void onToolboxItemClicked(const QString& name,
                                      const QString& category) {}
    virtual void onToolboxItemCheckedChanged(const QString& name,
                                             const QString& category,
                                             bool checked) {}
    virtual void onFileItemClicked(const QString& name,
                                      const QString& category) {}

    virtual bool setup(MapController *mapController,
                       NetworkManager * networkManager,
                       InfoWidgetHandle *infoWidgetHandle) {}

};


QT_BEGIN_NAMESPACE
#define PluginInterface_iid "map3d.hooshan.ir"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)
QT_END_NAMESPACE

#endif // PluginInterface_H
