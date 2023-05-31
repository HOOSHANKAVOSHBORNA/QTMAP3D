
#ifndef PluginInterface_H
#define PluginInterface_H


#include <QObject>
#include <QString>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <osgGA/GUIEventHandler>

class QQmlEngine;
class QQmlComponent;
class MapItem;
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
    void iwShow(QQuickItem* item, QString title);
    void iwHide();
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

class PluginInterface : public QObject, public osgGA::GUIEventHandler
{
    friend class PluginManager;
    Q_OBJECT

public:
    PluginInterface(QObject *parent = nullptr);
    virtual ~PluginInterface() { }
    MapItem *mapItem() const;
    void setMapItem(MapItem *mapItem);
    UIHandle *uiHandle() const;
    void setUiHandle(UIHandle *newUiHandle);

    virtual bool  handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) override;
    virtual bool initializeQMLDesc(QQmlEngine *engine, PluginQMLDesc *desc) {return false;}
    virtual void onSideItemCreated(int index, QObject *sideItem) {}
    virtual void onToolboxItemClicked(const QString& name,
                                      const QString& category) {}
    virtual void onToolboxItemCheckedChanged(const QString& name,
                                             const QString& category,
                                             bool checked) {}
    virtual void onFileItemClicked(const QString& name,
                                      const QString& category) {}

    virtual bool setup(MapItem *mapController,
                       UIHandle *uiHandle) {return false;}
    virtual void setDefenseDataManager(DefenseDataManager* defenseDataManager){}

    QString name() const;
    void setName(const QString &newName);



protected:
    virtual bool frameEvent           (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool keyPressEvent        (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool keyReleaseEvent      (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool mousePressEvent      (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {qDebug()<<"click:"<<mName; return false;}
    virtual bool mouseReleaseEvent    (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool mouseDoubleClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool mouseMoveEvent       (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool wheelEvent           (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
private:
    MapItem *mMapItem;
    UIHandle *mUiHandle;
    QString mName;
};


QT_BEGIN_NAMESPACE
#define PluginInterface_iid "map3d.hooshan.ir"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)
QT_END_NAMESPACE

#endif // PluginInterface_H
