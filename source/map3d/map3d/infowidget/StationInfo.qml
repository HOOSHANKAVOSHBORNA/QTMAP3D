import QtQuick 2.13
import QtQuick.Controls 2.13

Item {

    Rectangle {
        anchors.fill: parent
        color: "green"

        Text {
            anchors.centerIn: parent
            id: name
            text: qsTr("STATION")
            color:"white"
            font.pointSize: 30
        }
    }

}
