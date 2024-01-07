import QtQuick
import QtQuick.Layouts

Rectangle {
    id: rootItem

    property string text: 'Debug Item'

    Layout.fillWidth: true
    Layout.fillHeight: true

    color: 'gold'

    Text {
        text: rootItem.text
        anchors.centerIn: parent
    }
}
