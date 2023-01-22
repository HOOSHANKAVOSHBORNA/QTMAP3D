#include "contextMenu.h"

#include <QQmlComponent>
#include <QQuickItem>

ContextMenumodel::ContextMenumodel(QObject * parent):QAbstractListModel(parent)
{

}

int ContextMenumodel::rowCount(const QModelIndex &/*parent*/) const {
    return systemsList.size();
}

QVariant ContextMenumodel::data(const QModelIndex &index, int /*role*/) const{
    return systemsList.at(index.row());
}


QHash<int, QByteArray> ContextMenumodel::roleNames() const
{
    QHash<int, QByteArray> hash = QAbstractListModel::roleNames();
    hash[TextToShow] = "disp";
    return hash;
}

ContextMenu::ContextMenu(QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent) :
    QObject(parent), mUiHandle(uiHandle), mQmlEngine(qmlEngine)
{
    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){

        if (comp->status() == QQmlComponent::Ready) {
            mNowContextMenu = static_cast<QQuickItem*>(comp->create(nullptr));
            mContextMenuModel = new ContextMenumodel;
            }

    });

    comp->loadUrl(QUrl("qrc:/modelplugin/ContextMenuView.qml"));
}

void ContextMenu::show(int x, int y)
{
    mNowContextMenu->setProperty("model", QVariant::fromValue<ContextMenumodel*>(mContextMenuModel));

    QQmlEngine::setObjectOwnership(mNowContextMenu, QQmlEngine::JavaScriptOwnership);

    mUiHandle->cmShowContextMenu(mNowContextMenu, static_cast<int>(x), static_cast<int>(y));
}



void ContextMenu::hideMenu()
{
    mUiHandle->cmHideContextMenu(mNowContextMenu);
    mNowContextMenu = nullptr;
}

void ContextMenu::updatePosition(int x, int y)
{
    mUiHandle->cmSetContextMenuPosition(mNowContextMenu, x, y);
}
