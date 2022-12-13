
#include "plugininterface.h"
#include "mainwindow.h"

void InfoWidgetHandle::setReceiverObject(QObject *receiverObject)
{
    mReceiverObject = receiverObject;
}

void InfoWidgetHandle::showInfoWidget(QObject *receiverObject, InfoWidgetHandle::InfoWidgetType infoWidgetType)
{

    if (!mReceiverObject) return;
    if (mReceiverObject != receiverObject) return;

    if (mMainWindow) {
        bool bValidType = false;
        QString itemTypeString = "";
        switch (infoWidgetType) {
        case InfoWidgetHandle::InfoWidgetType::Airplane:
            bValidType = true;
            itemTypeString = "Airplane";
            break;
        case InfoWidgetHandle::InfoWidgetType::Station:
            bValidType = true;
            itemTypeString = "Station";
            break;
        case InfoWidgetHandle::InfoWidgetType::System:
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

void InfoWidgetHandle::updateData(QObject *receiverObject, const QString &infoJSON)
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

void InfoWidgetHandle::onInfoWidget2D3DButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "IW_2D3DButtonClicked");
    }
}

void InfoWidgetHandle::onInfoWidgetRouteButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "IW_RouteButtonClicked");
    }
}

void InfoWidgetHandle::onInfoWidgetFollowButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "IW_FollowButtonClicked");
    }
}

void InfoWidgetHandle::onInfoWidgetMoreButtonClicked()
{
    if (mReceiverObject) {
        QMetaObject::invokeMethod(mReceiverObject,
                                  "IW_MoreButtonClicked");
    }
}
