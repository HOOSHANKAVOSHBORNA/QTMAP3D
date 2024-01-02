
#ifndef PluginInterface_H
#define PluginInterface_H


#include "serviceManager.h"
#include "toolboxManager.h"
#include "mapControllerItem.h"

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
class BookmarkManager;
namespace osgViewer {
    class Viewer;
};

class PluginInterface : public QObject
{
    Q_OBJECT

public:
    PluginInterface(QObject *parent = nullptr);
    virtual ~PluginInterface() { }

    static void setMainWindow(MainWindow* mainWindow);
    MainWindow *mainWindow() const;
    MapControllerItem *mapItem() const;
    QQmlEngine *qmlEngine() const;
    Toolbox *toolbox() const;
    BookmarkManager *bookmarkManager() const;

    DefenseDataManager *defenseDataManager() const;
    static void setDefenseDataManager(DefenseDataManager* defenseDataManager);

    virtual bool setup() {return false;}

    QString name() const;
    void setName(const QString &newName);

    ServiceManager *serviceManager() const;
    static void setServiceManager(ServiceManager *newServiceManager);
    static void setPluginsMap(const QMap<QString, PluginInterface *> &newPluginsMap);
    PluginInterface *getPlugin(QString name);

public:
    virtual bool frameEvent           (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool keyPressEvent        (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool keyReleaseEvent      (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool mousePressEvent      (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool mouseReleaseEvent    (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool mouseClickEvent      (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool mouseDragEvent       (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool mouseDoubleClickEvent(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool mouseMoveEvent       (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}
    virtual bool wheelEvent           (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {return false;}

private:
    static inline MapControllerItem *mMapItem;
    static inline QQmlEngine *mQmlEngine;
    static inline DefenseDataManager *mDefenseDataManager;
    static inline MainWindow *mMainWindow;
    static inline Toolbox *mToolbox;
    static inline ServiceManager *mServiceManager;
    static inline BookmarkManager *mBookmarkManager;
    static inline QMap<QString, PluginInterface*> mPluginsMap;
    QString mName;

};


QT_BEGIN_NAMESPACE
#define PluginInterface_iid "map3d.hooshan.ir"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)
QT_END_NAMESPACE

#endif // PluginInterface_H
