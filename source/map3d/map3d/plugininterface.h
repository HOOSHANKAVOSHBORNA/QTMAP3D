
#ifndef PluginInterface_H
#define PluginInterface_H


#include <QObject>
#include <QString>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

class QQmlEngine;
class QQmlComponent;
class MapController;
class MainWindow;
class ListWindow;
class QQuickItem;
class DefenseDataManager;

namespace osgViewer {
    class Viewer;
};

class UIHandle : public QObject
{
    Q_OBJECT
    friend class MainWindow;
public:
    UIHandle(MainWindow *mainWindow, QObject *parent = nullptr);
    virtual ~UIHandle() { }

public:
    enum class InfoWidgetType {
        Airplane,
        Station,
        System
    };

    void iwSetReceiverObject(QObject *receiverObject);
    void iwShow(QQuickItem* item);
    void iwUpdateData(QObject *receiverObject, const QString& infoJSON);


public:
    void propertiesShow(QQuickItem* item);
    void propertiesHide(QQuickItem* item);

public:
    void sbShowMessage(const QString& message, qreal duration);

public:
    void cmShowContextMenu(QQuickItem* contextMenu, int x, int y);
    void cmSetContextMenuPosition(QQuickItem* contextMenu, int x, int y);
    void cmHideContextMenu(QQuickItem* contextMenu);

public:
    void lwAddTab(const QString& tabTitle, QQuickItem *tabItem);
signals:
    void listwindowTabChanged(int indx);
private:
    void onInfoWidget2D3DButtonClicked();
    void onInfoWidgetRouteButtonClicked();
    void onInfoWidgetFollowButtonClicked();
    void onInfoWidgetMoreButtonClicked();

    void setListWindow(ListWindow *listWindow);

private:
    MainWindow *mMainWindow = nullptr;
    ListWindow *mListWindow = nullptr;
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
    friend class PluginManager;
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
                       UIHandle *uiHandle) {}
    virtual void setDefenseDataManager(DefenseDataManager* defenseDataManager){}

protected:
    virtual void frameEvent           () {}
    virtual void keyPressEvent        (QKeyEvent* event) {}
    virtual void keyReleaseEvent      (QKeyEvent* event) {}
    virtual void mousePressEvent      (QMouseEvent* event) {}
    virtual void mouseReleaseEvent    (QMouseEvent* event) {}
    virtual void mouseDoubleClickEvent(QMouseEvent* event) {}
    virtual void mouseMoveEvent       (QMouseEvent* event) {}
    virtual void wheelEvent           (QWheelEvent* event) {}
};


QT_BEGIN_NAMESPACE
#define PluginInterface_iid "map3d.hooshan.ir"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)
QT_END_NAMESPACE

#endif // PluginInterface_H
