import QtQuick 2.13
import QtQuick.Layouts
import "style"

Item {
    property string title: 'aydefault'
    Layout.fillWidth: true
    Layout.fillHeight: true

    Rectangle {
        color: Style.foregroundColor
        border.width: 1
        border.color: 'grey'
        anchors.fill: parent
        anchors.margins: 5
        radius: 5

        Text {
            anchors.centerIn: parent
            text: title
            color: Style.backgroundColor
        }
    }
}

