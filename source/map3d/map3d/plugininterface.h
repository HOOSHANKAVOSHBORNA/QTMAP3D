
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

class ToolboxItemDesc : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name      READ name      WRITE setName      NOTIFY nameChanged)
    Q_PROPERTY(QString category  READ category  WRITE setCategory  NOTIFY categoryChanged)
    Q_PROPERTY(QString iconUrl   READ iconUrl   WRITE setIconUrl   NOTIFY iconUrlChanged)
    Q_PROPERTY(bool    checkable READ checkable WRITE setCheckable NOTIFY checkableChanged)
    Q_PROPERTY(bool    hasMenu   READ hasMenu   WRITE setHasMenu   NOTIFY hasMenuChanged)
    Q_PROPERTY(QString menuUrl   READ menuUrl   WRITE setMenuUrl   NOTIFY menuUrlChanged)

public:
    ToolboxItemDesc(
            QString name      = QString(),
            QString category  = QString(),
            QString iconUrl   = QString(),
            bool    checkable = false,
            bool    hasMenu   = false,
            QString menuUrl   = QString()
            ) :
        mName     (name)     ,
        mCategory (category) ,
        mIconUrl  (iconUrl)  ,
        mCheckable(checkable),
        mHasMenu  (hasMenu)  ,
        mMenuUrl  (menuUrl)
    {}
    ~ToolboxItemDesc() {}

public:
    QString name() const
    {
        return mName;
    }

    QString category() const
    {
        return mCategory;
    }

    QString iconUrl() const
    {
        return mIconUrl;
    }

    bool checkable() const
    {
        return mCheckable;
    }

    bool hasMenu() const
    {
        return mHasMenu;
    }

    QString menuUrl() const
    {
        return mMenuUrl;
    }

public:
    void setName(const QString& name) {}
    void setCategory(const QString& category) {}
    void setIconUrl(const QString& iconUrl) {}
    void setCheckable(bool checkable) {}
    void setHasMenu(bool hasMenu) {}
    void setMenuUrl(const QString& setMenuUrl) {}


signals:
    void nameChanged();
    void categoryChanged();
    void iconUrlChanged();
    void checkableChanged();
    void hasMenuChanged();
    void menuUrlChanged();

private:
    QString mName;
    QString mCategory;
    QString mIconUrl;
    bool    mCheckable = false;
    bool    mHasMenu   = false;
    QString mMenuUrl;
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
