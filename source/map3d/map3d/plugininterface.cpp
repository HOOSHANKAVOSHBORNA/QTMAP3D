
#include "plugininterface.h"
#include "mainwindow.h"

void InfoWidgetHandle::showInfoWidget(InfoWidgetHandle::InfoWidgetType infoWidgetType)
{
    if (mMainWindow) {
        bool bValidType = false;
        QString itemTypeString = "";
        switch (infoWidgetType) {
        case InfoWidgetHandle::InfoWidgetType::Airplane:
            bValidType = true;
            itemTypeString = "AIRPLANE";
            break;
        case InfoWidgetHandle::InfoWidgetType::Station:
            bValidType = true;
            itemTypeString = "STATION";
            break;
        case InfoWidgetHandle::InfoWidgetType::System:
            bValidType = true;
            itemTypeString = "SYSTEM";
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

void InfoWidgetHandle::updateData(const QString &infoJSON)
{
    if (mMainWindow) {
        QMetaObject::invokeMethod(mMainWindow,
                                  "updateInfoWidgetData",
                                  Q_ARG(QVariant, QVariant::fromValue<QString>(infoJSON))
                                  );
    }
}
