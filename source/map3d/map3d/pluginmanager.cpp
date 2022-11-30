
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <QDebug>

#include "pluginmanager.h"
#include "plugininterface.h"

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{
    mToolboxItemsMap["Ali"]["Askari"] = nullptr;
}

void PluginManager::loadPlugins()
{
    QDir pluginsDir(PLUGINS_OUTPUT_DIR);

    for (const QString& fileName : pluginsDir.entryList(QDir::Files)) {

        if ((fileName.split('.').back() == "so") || (fileName.split('.').back() == "dll"))
        {
            qDebug() << fileName;

            const QString filePath = pluginsDir.absoluteFilePath(fileName);
            QPluginLoader pluginLoader(filePath);

            QObject* instance = pluginLoader.instance();

            qDebug() << "11111111111111111111111";
            if (!instance)
                continue;

            PluginInterface *pluginInterface =
                    dynamic_cast<PluginInterface*>(instance);

            qDebug() << "11111111111111111111111";
            if (pluginInterface) {
                qDebug() << "22222222222222222222222";
                CrystalPluginInfo cpi;
                cpi.interface = pluginInterface;
                cpi.qmlDesc    = new PluginQMLDesc;
                cpi.sideItemIndex = -1;
                mPluginsInfoList.push_back(std::move(cpi));
            }
        }
    }
}

void PluginManager::performPluginsInitQMLDesc(QQmlEngine *qmlEngine)
{
    for (const auto& item : mPluginsInfoList) {
        item.interface->initializeQMLDesc(qmlEngine, item.qmlDesc);
    }
}

void PluginManager::performPluginsInit3D(MapController *mapController)
{
    for (const auto& item : mPluginsInfoList) {
        item.interface->initialize3D(mapController);
    }
}

std::list<CrystalPluginInfo> &PluginManager::pluginsInfoList()
{
    return mPluginsInfoList;
}

void PluginManager::onSideItemCreated(int index, QObject *sideItem)
{
    const auto it = std::find_if(mPluginsInfoList.begin(),
                 mPluginsInfoList.end(),
                 [index](const CrystalPluginInfo& item){
        return (item.sideItemIndex == index);
    });

    if (it != mPluginsInfoList.end()) {
        it->interface->onSideItemCreated(index, sideItem);
    }
}

void PluginManager::onToolboxItemCreated(const QString &name,
                                         const QString &category,
                                         QObject *menuItem,
                                         PluginInterface *interface)
{
    mToolboxItemsMap[category][name] = interface;
}

void PluginManager::onToolboxItemClicked(const QString &name, const QString &category)
{
    if (mToolboxItemsMap.contains(category)) {
        if (mToolboxItemsMap[category].contains(name)) {
                PluginInterface* pInterface = mToolboxItemsMap[category][name];
                if (pInterface) {
                    pInterface->onToolboxItemClicked(name, category);
                }
        }
    }
}
