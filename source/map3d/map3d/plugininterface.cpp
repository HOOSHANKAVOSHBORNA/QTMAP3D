
#include "plugininterface.h"
#include "mainwindow.h"
#include <QQuickItem>
#include <QTimer>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include "application.h"



UIHandle::UIHandle(MainWindow *mainWindow)
{
    mMainWindow = mainWindow;
}


void UIHandle::iwSetReceiverObject(QObject *receiverObject)
{
    mReceiverObject = receiverObject;
}

void UIHandle::iwShow(QObject *receiverObject, UIHandle::InfoWidgetType infoWidgetType)
{

    if (!mReceiverObject) return;
    if (mReceiverObject != receiverObject) return;

    if (mMainWindow) {
        bool bValidType = false;
        QString itemTypeString = "";
        switch (infoWidgetType) {
        case UIHandle::InfoWidgetType::Airplane:
            bValidType = true;
            itemTypeString = "Airplane";
            break;
        case UIHandle::InfoWidgetType::Station:
            bValidType = true;
            itemTypeString = "Station";
            break;
        case UIHandle::InfoWidgetType::System:
            bValidType = true;
            itemTypeString = "System";
            break;
        }

        if (bValidType) {
            QMetaObject::invokeMethod(mMainWindow,
                                      "showInfoItem",
                                      Q_ARG(QVariant, QVariant::fromValue<QString>(itemTypeString))
                                      );
        }
    }
}

void UIHandle::iwUpdateData(QObject *receiverObject, const QString &infoJSON)
{
    if (!mReceiverObject) return;
    if (mReceiverObject != receiverObject) return;

    if (mMainWindow) {
        QMetaObject::invokeMethod(mMainWindow,
                                  "updateInfoWidgetData",
                                  Q_ARG(QVariant, QVariant::fromValue<QString>(infoJSON))
                                  );
    }

}

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

        }
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
