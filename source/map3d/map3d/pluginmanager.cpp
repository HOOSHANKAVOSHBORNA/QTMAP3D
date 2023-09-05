
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <QDebug>

#include "pluginmanager.h"
#include "plugininterface.h"
#include "application.h"
#include "mainwindow.h"
#include "qqmlapplicationengine.h"

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
        for (auto& item : mPluginManager->mPluginsInfoList) {
            bool res = item.interface->frameEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::KEYDOWN):
        for (auto& item : mPluginManager->mPluginsInfoList) {
            bool res = item.interface->keyPressEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::KEYUP):
        for (auto& item : mPluginManager->mPluginsInfoList) {
            bool res = item.interface->keyReleaseEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::PUSH):
        for (auto& item : mPluginManager->mPluginsInfoList) {
            bool res = item.interface->mousePressEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::DRAG):
        for (auto& item : mPluginManager->mPluginsInfoList) {
            bool res = item.interface->mouseDragEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::RELEASE):
        for (auto& item : mPluginManager->mPluginsInfoList) {
            bool res = item.interface->mouseReleaseEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::DOUBLECLICK):
        for (auto& item : mPluginManager->mPluginsInfoList) {
            bool res = item.interface->mouseDoubleClickEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::MOVE):
        for (auto& item : mPluginManager->mPluginsInfoList) {
            bool res = item.interface->mouseMoveEvent(ea, aa);
            if(res)
                return true;
        }
        return false;
    case (osgGA::GUIEventAdapter::SCROLL):
        for (auto& item : mPluginManager->mPluginsInfoList) {
            bool res = item.interface->wheelEvent(ea, aa);
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
}

void PluginManager::loadPlugins()
{
    QDir pluginsDir = QCoreApplication::applicationDirPath();
    pluginsDir.cd("../plugins/bin");
    mPluginFileNameList = pluginsDir.entryList(QDir::Files);
    for (const QString& pluginFileName : mPluginFileNameList) {
            parsePlugin(pluginFileName, pluginsDir);
    }
}

void PluginManager::setup()
{
    auto mainWindow = Application::instance()->mainWindow();
    PluginInterface::setMainWindow(mainWindow);
    auto mapItem = Application::instance()->mainWindow()->getMapItem();
    PluginInterface::setMapItem(mapItem);
    mapItem->getViewer()->addEventHandler(new EventHandler(this));
    //--------------------------------
    QQmlEngine *qmlEngine = dynamic_cast<QQmlEngine*>(Application::instance()->qmlEngine());
    PluginInterface::setQmlEngine(qmlEngine);

//    for (const auto& item : mPluginsInfoList) {
//        item.interface->initializeQMLDesc(qmlEngine, item.qmlDesc);
//    }
    //-----------------------------------
    DefenseDataManager* defenseDataManager = Application::instance()->defenseDataManager();
    PluginInterface::setDefenseDataManager(defenseDataManager);

//    UIHandle * const uiHandle = Application::instance()->mainWindow()->uiHandle();
//    PluginInterface::setUiHandle(uiHandle);
    auto toolbox = Application::instance()->mainWindow()->toolbox();
    PluginInterface::setToolbox(static_cast<Toolbox*>(toolbox->sourceModel()));

//    ServiceManager * const serviceManager = Application::instance()->mainWindow()->serviceManager();
//    PluginInterface::setServiceManager(serviceManager);
    //-------------------------------------
    for (const auto& item : mPluginsInfoList) {
        item.interface->setup();
    }
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

        if (deps.isArray() && !deps.toArray().isEmpty())
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

        const QString filePath = pluginsDir.absoluteFilePath(pluginFileName);
        QPluginLoader pluginLoader(filePath);

        QObject* instance = pluginLoader.instance();

        if (!instance)
        {
            //                QString errStr = pluginLoader.errorString();
            qWarning() << "Plugin loading failed: [" << pluginFileName
                       << "] " << pluginLoader.errorString();
            return;
        }

        PluginInterface *pluginInterface =
            dynamic_cast<PluginInterface*>(instance);

        if (pluginInterface) {
            pluginInterface->setName(pluginFileName);
            PluginInfo cpi;
            cpi.interface = pluginInterface;
            //                cpi.qmlDesc    = new PluginQMLDesc;
            cpi.sideItemIndex = -1;
            mPluginsInfoList.push_back(std::move(cpi));
        }
    }
}

//void PluginManager::setQmlEngine(QQmlEngine *qmlEngine)
//{
//    for (const auto& item : mPluginsInfoList) {
//        item.interface->initializeQMLDesc(qmlEngine, item.qmlDesc);
//    }
//}

//void PluginManager::setMapItem(MapItem *mapItem)
//{
//    PluginInterface::setMapItem(mapItem);
//    mapItem->getViewer()->addEventHandler(new EventHandler(this));
////    DefenseDataManager* defenseDataManager = Application::instance()->defenseDataManager();
////    UIHandle * const uiHandle = Application::instance()->mainWindow()->uiHandle();
////    for (const auto& item : mPluginsInfoList) {
////        item.interface->setup(mapItem, uiHandle);
////        item.interface->setMapItem(mapItem);
////        item.interface->setDefenseDataManager(defenseDataManager);
////    }
//}

//void PluginManager::setDefenseDataManager(DefenseDataManager *defenseDataManager)
//{

//}

//std::list<PluginInfo> &PluginManager::pluginsInfoList()
//{
//    return mPluginsInfoList;
//}

//void PluginManager::onSideItemCreated(int index, QObject *sideItem)
//{
//    const auto it = std::find_if(mPluginsInfoList.begin(),
//                                 mPluginsInfoList.end(),
//                                 [index](const PluginInfo& item){
//        return (item.sideItemIndex == index);
//    });

//    if (it != mPluginsInfoList.end()) {
//        it->interface->onSideItemCreated(index, sideItem);
//    }
//}

//void PluginManager::onToolboxItemCreated(ItemDescProxy *itemProxy)
//{
//    mToolboxItemsMap[itemProxy->category()][itemProxy->name()] = itemProxy->pluginInterface();
//}

//void PluginManager::onFileItemCreated(ItemDescProxy *itemProxy)
//{
//    mFileItemsMap[itemProxy->category()][itemProxy->name()] = itemProxy->pluginInterface();
//}

//void PluginManager::onToolboxItemClicked(const QString &name, const QString &category)
//{
//    if (mToolboxItemsMap.contains(category)) {
//        if (mToolboxItemsMap[category].contains(name)) {
//            PluginInterface* pInterface = mToolboxItemsMap[category][name];
//            if (pInterface) {
//                pInterface->onToolboxItemClicked(name, category);
//            }
//        }
//    }
//}

//void PluginManager::onToolboxItemCheckedChanged(const QString &name, const QString &category, bool checked)
//{
//    if (mToolboxItemsMap.contains(category)) {
//        if (mToolboxItemsMap[category].contains(name)) {
//            PluginInterface* pInterface = mToolboxItemsMap[category][name];
//            if (pInterface) {
//                pInterface->onToolboxItemCheckedChanged(name, category, checked);
//            }
//        }
//    }

//}

//void PluginManager::onFileItemClicked(const QString &name, const QString &category)
//{
//    if (mFileItemsMap.contains(category)) {
//        if (mFileItemsMap[category].contains(name)) {
//            PluginInterface* pInterface = mFileItemsMap[category][name];
//            if (pInterface) {
//                pInterface->onFileItemClicked(name, category);
//            }
//        }
//    }
//}

//void PluginManager::frameEvent()
//{
//    for (auto& item : mPluginsInfoList) {
//        item.interface->frameEvent();
//    }
//}

//void PluginManager::keyPressEvent(QKeyEvent *event)
//{
//    for (auto& item : mPluginsInfoList) {
//        item.interface->keyPressEvent(event);
//    }
//}

//void PluginManager::keyReleaseEvent(QKeyEvent *event)
//{
//    for (auto& item : mPluginsInfoList) {
//        item.interface->keyReleaseEvent(event);
//    }
//}

//void PluginManager::mousePressEvent(QMouseEvent *event)
//{
//    for (auto& item : mPluginsInfoList) {
//        item.interface->mousePressEvent(event);
//    }
//}

//void PluginManager::mouseReleaseEvent(QMouseEvent *event)
//{
//    for (auto& item : mPluginsInfoList) {
//        item.interface->mouseReleaseEvent(event);
//    }
//}

//void PluginManager::mouseDoubleClickEvent(QMouseEvent *event)
//{
//    for (auto& item : mPluginsInfoList) {
//        item.interface->mouseDoubleClickEvent(event);
//    }
//}

//void PluginManager::mouseMoveEvent(QMouseEvent *event)
//{
//    for (auto& item : mPluginsInfoList) {
//        item.interface->mouseMoveEvent(event);
//    }
//}

//void PluginManager::wheelEvent(QWheelEvent *event)
//{
//    for (auto& item : mPluginsInfoList) {
//        item.interface->wheelEvent(event);
//    }
//}

