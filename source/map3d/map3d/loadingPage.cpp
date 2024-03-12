#include "loadingPage.h"


LoadingPage::LoadingPage(QObject *parent):QObject(parent)
{
}

void LoadingPage::addItem(const QString &message, bool isError)
{
    mLoadingDataItem.insert(0, {message, isError});
    emit itemAdded();
    mPluginFraction = mLoadingDataItem.size()/(mPluginsCount+0.01);
    setpluginFraction(mPluginFraction);
}

void LoadingPage::removeItem(int index)
{
    if(index < mLoadingDataItem.size()){
        mLoadingDataItem.erase(mLoadingDataItem.begin() + index);
        emit pluginFractionChanged();
    }
    else
        return;
}

QString LoadingPage::firstItem()
{
    return mLoadingDataItem[0].text;
}

bool LoadingPage::firstItemErrorStatus()
{
    return mLoadingDataItem[0].isError;
}

float LoadingPage::pluginFraction() const
{
    return mPluginFraction;
}

void LoadingPage::setpluginFraction(float pluginFrac)
{
    mPluginFraction = pluginFrac;
    emit pluginFractionChanged();
}

void LoadingPage::setPluginsCount(int count)
{
    mPluginsCount = count;
}
