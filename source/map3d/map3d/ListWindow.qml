import QtQuick 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.13
import Crystal 1.0
import QtQuick.Window 2.13
import "style"

CListWindow {
    id: rootItem
    width: 1000
    height: 600

    //    color: 'transparent'
    //    flags: Qt.FramelessWindowHint
    Rectangle {
        color: Style.backgroundColor
        anchors.fill: parent

        //        radius: 20
        ColumnLayout {
            anchors.fill: parent
            anchors.leftMargin: 25
            anchors.rightMargin: 25
            anchors.topMargin: 20
            anchors.bottomMargin: 25

            spacing: 0

            Repeater {
                id: rep
                model: tabbarModel

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
