
#ifndef PluginInterface_H
#define PluginInterface_H


#include <QObject>
#include <QString>

class QQmlEngine;
class QQmlComponent;
class MapController;
class NetworkManager;
class MainWindow;
class QQuickItem;

namespace osgViewer {
    class Viewer;
};

class UIHandle
{
    friend class MainWindow;
public:
    UIHandle(MainWindow *mainWindow) { mMainWindow = mainWindow; }
    virtual ~UIHandle() { }

public:
    enum class InfoWidgetType {
        Airplane,
        Station,
        System
    };

    void iwSetReceiverObject(QObject *receiverObject);
    void iwShow(QObject* receiverObject, InfoWidgetType infoWidgetType);
    void iwUpdateData(QObject *receiverObject, const QString& infoJSON);

public:
    void sbShowMessage(const QString& message, qreal duration);

public:
    void cmShowContextMenu(QQuickItem* contextMenu, int x, int y);
    void cmSetContextMenuPosition(QQuickItem* contextMenu, int x, int y);
    void cmHideContextMenu(QQuickItem* contextMenu);

private:
    void onInfoWidget2D3DButtonClicked();
    void onInfoWidgetRouteButtonClicked();
    void onInfoWidgetFollowButtonClicked();
    void onInfoWidgetMoreButtonClicked();



private:
    MainWindow *mMainWindow = nullptr;
    QObject *mReceiverObject = nullptr;
    QQuickItem *mCurrentContextMenuItem = nullptr;
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
                       UIHandle *uiHandle) {}

};


QT_BEGIN_NAMESPACE
#define PluginInterface_iid "map3d.hooshan.ir"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)
QT_END_NAMESPACE

#endif // PluginInterface_H
