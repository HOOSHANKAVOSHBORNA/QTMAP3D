
#include "plugininterface.h"
#include "mainwindow.h"

void UIHandle::iw_setReceiverObject(QObject *receiverObject)
{
    mReceiverObject = receiverObject;
}

void UIHandle::iw_show(QObject *receiverObject, UIHandle::InfoWidgetType infoWidgetType)
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

void UIHandle::iw_updateData(QObject *receiverObject, const QString &infoJSON)
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

void UIHandle::sb_showMessage(const QString &message, qreal duration)
{

}

void UIHandle::onInfoWidget2D3DButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "iw_2D3DButtonClicked");
    }
}

void UIHandle::onInfoWidgetRouteButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "iw_routeButtonClicked");
    }
}

void UIHandle::onInfoWidgetFollowButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "iw_followButtonClicked");
    }
}

void UIHandle::onInfoWidgetMoreButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "iw_moreButtonClicked");
    }
}
