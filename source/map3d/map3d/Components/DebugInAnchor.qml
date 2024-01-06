import QtQuick

Rectangle {
    id: rootItem

    property string text: 'Debug Item'

    anchors.fill: parent

    color: 'gold'

    Text {
        text: rootItem.text
        anchors.centerIn: parent
    }
}
