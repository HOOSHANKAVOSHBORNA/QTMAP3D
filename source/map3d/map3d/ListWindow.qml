import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13

import "style"

Window {
    id: rootItem

    property var listWindowCpp: undefined
    property var tabbarModel: listWindowCpp ? listWindowCpp.tabbarModel : undefined

    title: "Node List"
    width: 1000 / Style.monitorRatio
    height: 600 / Style.monitorRatio
    minimumWidth: 1600 / Style.monitorRatio
    //    color: 'transparent'
    //    flags: Qt.FramelessWindowHint
    Rectangle {
        color: Style.backgroundColor
        anchors.fill: parent

        //        radius: 20
        ColumnLayout {
            anchors.fill: parent
            anchors.leftMargin: 25/ Style.monitorRatio
            anchors.rightMargin: 25/ Style.monitorRatio
            anchors.topMargin: 20/ Style.monitorRatio
            anchors.bottomMargin: 25/ Style.monitorRatio

            spacing: 0

            Repeater {
                id: rep
                model: rootItem.tabbarModel ?? 0

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: 'transparent'

                    data: model.item
                }
            }
        }
    }
}
