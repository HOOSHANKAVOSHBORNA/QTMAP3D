
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QDebug>

#include "pluginmanager.h"
#include "plugininterface.h"
#include "application.h"
#include "mainwindow.h"

EventHandler::EventHandler(PluginManager *pluginManager)
{
    mPluginManager = pluginManager;
}

bool EventHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    osgViewer::View *view = dynamic_cast<osgViewer::View *>(&aa);

    if (!view)
    {
        return false;
    }
    switch (ea.getEventType())
    {
    case osgGA::GUIEventAdapter::FRAME:
        for (auto& item : mPluginManager->mPluginsMap) {
            bool res = item->frameEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::KEYDOWN):
        for (auto& item : mPluginManager->mPluginsMap) {
            bool res = item->keyPressEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::KEYUP):
        for (auto& item : mPluginManager->mPluginsMap) {
            bool res = item->keyReleaseEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::PUSH):
        for (auto& item : mPluginManager->mPluginsMap) {
            bool res = item->mousePressEvent(ea, aa);
            mCurrentPressPoint.setX(ea.getX());
            mCurrentPressPoint.setY(ea.getY());
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::DRAG):
        for (auto& item : mPluginManager->mPluginsMap) {
            bool res = item->mouseDragEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::RELEASE):
        for (auto& item : mPluginManager->mPluginsMap) {
            bool res = item->mouseReleaseEvent(ea, aa);
            if((ea.getX() - 2) < mCurrentPressPoint.x() &&  mCurrentPressPoint.x() < (ea.getX() + 2)
                && (ea.getY() - 2) < mCurrentPressPoint.y() && mCurrentPressPoint.y() < (ea.getY() + 2))
                item->mouseClickEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::DOUBLECLICK):
        for (auto& item : mPluginManager->mPluginsMap) {
            bool res = item->mouseDoubleClickEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::MOVE):
        for (auto& item : mPluginManager->mPluginsMap) {
            bool res = item->mouseMoveEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::SCROLL):
        for (auto& item : mPluginManager->mPluginsMap) {
            bool res = item->wheelEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    default:

        return false;
    }
}

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{
    mPluginLoader = new QPluginLoader(this);
    mPluginsDir = QCoreApplication::applicationDirPath();
    mPluginsDir.cd("../plugins/bin");
    mPluginFileNameList = mPluginsDir.entryList(QDir::Files);
}

PluginManager::~PluginManager()
{
    qDebug() << "~PluginManager";
    unLoadPlugins();
    for (auto &i : mPluginsMap)
        delete i;
    mPluginsMap.clear();
    Application::instance()->mainWindow()->getMapItem()->getViewer()->removeEventHandler(mEventHandler);
    delete mPluginLoader;
}

void PluginManager::loadPlugins()
{
    mPluginTimer = new QTimer(this);
    connect(mPluginTimer, &QTimer::timeout, this, [&](){
        parsePlugin(mPluginFileNameList[mIndex], mPluginsDir);
        if (mIndex == mPluginFileNameList.size() - 1) {
            mPluginTimer->stop();
//            emit pluginsLoaded();
            setup();
        }
        mIndex++;
    });
    mPluginTimer->start(100);
}

void PluginManager::unLoadPlugins()
{
    for (auto &i: mLoadedPlugins) {
        mPluginLoader->setFileName(i);
        mPluginLoader->unload();
    }
}

QStringList PluginManager::pluginFileNameList() const
{
    return mPluginFileNameList;
}

void PluginManager::setup()
{
    PluginInterface::setQmlEngine(Application::instance()->qmlEngine());
    auto mainWindow = Application::instance()->mainWindow();
    PluginInterface::setMainWindow(mainWindow);
    auto mapItem = mainWindow->getMapItem();
    mEventHandler = new EventHandler(this);
    mapItem->getViewer()->addEventHandler(mEventHandler);
    //----------------------------------
    PluginInterface::setPluginsMap(mPluginsMap);
    //--------------------------------
    ServiceManager *serviceManager = Application::instance()->serviceManager();
    PluginInterface::setServiceManager(serviceManager);
    //-------------------------------------
    for (auto item : qAsConst(mPluginsMap)) {
        item->setup();
    }
    emit setupFinished();
}

void PluginManager::parsePlugin(const QString &pluginFileName, const QDir &pluginsDir)
{
    try
    {
        if(mLoadedPluginList.contains(pluginFileName))
            return;
        // Get plugin info
        QString        path = pluginsDir.absoluteFilePath(pluginFileName);
        QPluginLoader  pluginLoader(path);
//        bool           debug      = pluginLoader.metaData()["debug"].toBool();
        QJsonObject    metaData   = pluginLoader.metaData()["MetaData"].toObject();
//        QString        pluginName = metaData["Name"].toString();

//        if (debug)
//        {
//            pluginName += 'd';
//        }

        // Resolve dependencies
        QJsonValue  deps = metaData["Dependencies"];
        if (deps.isArray())
        {
            for (const QJsonValue& plugin : deps.toArray())
            {
                // Register info for parent
                QString  dependName = plugin.toObject()["Name"].toString();
                dependName = "lib" + dependName+".so";
                int index = mPluginFileNameList.indexOf(dependName);
                if(index != -1){
                    QString dependFileName = mPluginFileNameList[index];
                    parsePlugin(dependFileName, pluginsDir);
                }
            }
        }
        loadPlugin(pluginFileName, pluginsDir);
        mLoadedPluginList.append(pluginFileName);
    }
    catch (...)
    {
        qWarning() << "Plugin meta not valid: " << pluginFileName;
    }
}

void PluginManager::loadPlugin(const QString &pluginFileName, const QDir &pluginsDir)
{
    if ((pluginFileName.split('.').back() == "so") || (pluginFileName.split('.').back() == "dll"))
    {
        qDebug() << pluginFileName;
        emit pluginMessage(pluginFileName, false);

        const QString filePath = pluginsDir.absoluteFilePath(pluginFileName);
        mPluginLoader->setFileName(filePath);
        mPluginLoader->load();
        QObject* instance = mPluginLoader->instance();

        if (!instance)
        {
            qWarning() << "Plugin loading failed: [" << pluginFileName
                       << "] " << mPluginLoader->errorString();
            emit pluginMessage(mPluginLoader->errorString(), true);
            return;
        }

        PluginInterface *pluginInterface =
            dynamic_cast<PluginInterface*>(instance);

        if (pluginInterface) {
            pluginInterface->setName(pluginFileName);
            mPluginsMap.insert(pluginFileName, pluginInterface);
            mLoadedPlugins.push_back(pluginFileName);
        }
    }
}

QMap<QString, PluginInterface *> PluginManager::pluginsMap() const
{
    return mPluginsMap;
}

