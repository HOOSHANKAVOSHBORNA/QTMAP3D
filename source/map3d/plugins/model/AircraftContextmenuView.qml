import QtQuick 2.0
import Crystal 1.0

Item {
    id: root
    anchors.fill: parent
    property AirplaneContextMenumodel model: AirplaneContextMenumodel {

    }

    ListView {
        anchors.fill: parent

        model: root.model
        delegate: Rectangle {
            color: "red"
            implicitWidth:200
            implicitHeight:  30
            Text {
                anchors.centerIn: parent
                text: display
                color: "white"
            }
        }
    }
}
