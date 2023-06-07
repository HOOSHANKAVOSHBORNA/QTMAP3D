
#include "plugininterface.h"
#include "listwindow.h"
#include "mainwindow.h"
#include <QQuickItem>
#include <QTimer>
#include <QQmlComponent>
#include <QQmlApplicationEngine>

UIHandle::UIHandle(MainWindow *mainWindow, QObject *parent) : QObject(parent)
{
    mMainWindow = mainWindow;
}

MapItem *PluginInterface::mMapItem;
UIHandle *PluginInterface::mUiHandle;
DefenseDataManager *PluginInterface::mDefenseDataManager;

PluginInterface::PluginInterface(QObject *parent):
    QObject(parent)
{

}

MapItem *PluginInterface::mapItem() const
{
    return mMapItem;
}
void PluginInterface::setMapItem(MapItem *mapItem)
{
    mMapItem = mapItem;
//    bool isSetHandler = false;
//    for(auto eventHandler :mMapItem->getViewer()->getEventHandlers()){
//        auto pli = dynamic_cast<PluginInterface*>(eventHandler.get());
//        if(pli){
//            isSetHandler = true;
//            break;
//        }
//    }
//    if(!isSetHandler)
//        mMapItem->getViewer()->addEventHandler(this);
}

UIHandle *PluginInterface::uiHandle() const
{
        return mUiHandle;
}

void PluginInterface::setUiHandle(UIHandle *newUiHandle)
{
        mUiHandle = newUiHandle;
}

DefenseDataManager *PluginInterface::defenseDataManager() const
{
        return mDefenseDataManager;
}

void PluginInterface::setDefenseDataManager(DefenseDataManager *defenseDataManager)
{
        mDefenseDataManager = defenseDataManager;
}

//bool PluginInterface::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//{
//    // Get view and event adaptor
//    osgViewer::View *view = dynamic_cast<osgViewer::View *>(&aa);

//    if (!view)
//    {
//        return false;
//    }
//    switch (ea.getEventType())
//    {
//    case osgGA::GUIEventAdapter::FRAME:
//        return frameEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::KEYDOWN):
//        return keyPressEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::KEYUP):
//        return keyReleaseEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::PUSH):
//        return mousePressEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::RELEASE):
//        return mouseReleaseEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::DOUBLECLICK):
//        return mouseDoubleClickEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::MOVE):
//        return mouseMoveEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::SCROLL):
//        return wheelEvent(ea, aa);
//    default:

//        return false;
//    }
//}

QString PluginInterface::name() const
{
    return mName;
}

void PluginInterface::setName(const QString &newName)
{
    mName = newName;
}

void UIHandle::iwSetReceiverObject(QObject *receiverObject)
{
    mReceiverObject = receiverObject;
}

void UIHandle::iwShow(QQuickItem* item, QString title)
{

    //    if (!mReceiverObject) return;
    //    if (mReceiverObject != receiverObject) return;

    //    if (mMainWindow) {
    //        bool bValidType = false;
    //        QString itemTypeString = "";
    //        switch (infoWidgetType) {
    //        case UIHandle::InfoWidgetType::Airplane:
    //            bValidType = true;
    //            itemTypeString = "Airplane";
    //            break;
    //        case UIHandle::InfoWidgetType::Station:
    //            bValidType = true;
    //            itemTypeString = "Station";
    //            break;
    //        case UIHandle::InfoWidgetType::System:
    //            bValidType = true;
    //            itemTypeString = "System";
    //            break;
    //        }

    //        if (bValidType) {
    //            QMetaObject::invokeMethod(mMainWindow,
    //                                      "showInfoItem",
    //                                      Q_ARG(QVariant, QVariant::fromValue<QString>(itemTypeString))
    //                                      );
    //        }
    //    }
    QMetaObject::invokeMethod(mMainWindow,
                              "showInfoView",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item)),
                              Q_ARG(QVariant, QVariant::fromValue<QString>(title))
                              );
}

void UIHandle::iwHide()
{
    QMetaObject::invokeMethod(mMainWindow,
                              "hideInfoView"
                              );
}

void UIHandle::propertiesShow(QQuickItem *item)
{
    QMetaObject::invokeMethod(mMainWindow,
                              "showRightContainer",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item))
                              );
}

void UIHandle::propertiesHide(QQuickItem *item)
{
    QMetaObject::invokeMethod(mMainWindow,
                              "hideRightContainer",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item))
                              );
}


//void UIHandle::iwUpdateData(QObject *receiverObject, const QString &infoJSON)
//{
//    if (!mReceiverObject) return;
//    if (mReceiverObject != receiverObject) return;

//    if (mMainWindow) {
//        QMetaObject::invokeMethod(mMainWindow,
//                                  "updateInfoWidgetData",
//                                  Q_ARG(QVariant, QVariant::fromValue<QString>(infoJSON))
//                                  );
//    }

//}

void UIHandle::sbShowMessage(const QString &message, qreal duration)
{
    if (mMainWindow) {
        QMetaObject::invokeMethod(mMainWindow,
                                  "showStatusMessage",
                                  Q_ARG(QVariant, QVariant::fromValue<QString>(message)),
                                  Q_ARG(QVariant, QVariant::fromValue<int>(int(duration * 1000.0)))
                                  );

    }
}

void UIHandle::cmShowContextMenu(QQuickItem *contextMenu, int x, int y)
{
    if (mMainWindow) {

        if (mCurrentContextMenuItem) {
            cmHideContextMenu(mCurrentContextMenuItem);
        }

        mCurrentContextMenuItem = contextMenu;
        QMetaObject::invokeMethod(mMainWindow,
                                  "addContextmenu",
                                  Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(contextMenu)),
                                  Q_ARG(QVariant, QVariant::fromValue<int>(x)),
                                  Q_ARG(QVariant, QVariant::fromValue<int>(y))
                                  );


    }
}

void UIHandle::cmSetContextMenuPosition(QQuickItem *contextMenu, int x, int y)
{
    if (mMainWindow) {
        if (contextMenu == mCurrentContextMenuItem) {
            QMetaObject::invokeMethod(mMainWindow,
                                      "updateXYContextmenu",
                                      Q_ARG(QVariant, QVariant::fromValue<int>(x)),
                                      Q_ARG(QVariant, QVariant::fromValue<int>(y))
                                      );

        }
    }
}

void UIHandle::cmHideContextMenu(QQuickItem *contextMenu)
{
    if (mMainWindow) {
        if (contextMenu == mCurrentContextMenuItem) {
            QMetaObject::invokeMethod(mMainWindow,
                                      "hideContextmenu"
                                      );
            mCurrentContextMenuItem = nullptr;

        }
    }
}

void UIHandle::lwAddTab(const QString &tabTitle, QQuickItem *tabItem)
{
    if (mListWindow) {
        QMetaObject::invokeMethod(mListWindow,
                                  "addTab",
                                  Q_ARG(QVariant, QVariant::fromValue<QString>(tabTitle)),
                                  Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(tabItem))
                                  );

    }
}

void UIHandle::onInfoWidget2D3DButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "iw2D3DButtonClicked");
    }
}

void UIHandle::onInfoWidgetRouteButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "iwRouteButtonClicked");
    }
}

void UIHandle::onInfoWidgetFollowButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "iwFollowButtonClicked");
    }
}

void UIHandle::onInfoWidgetMoreButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "iwMoreButtonClicked");
    }
}

void UIHandle::setListWindow(ListWindow *listWindow)
{
    mListWindow = listWindow;
    QObject::connect(mListWindow, &ListWindow::tabChanged, this, &UIHandle::listwindowTabChanged);
}


