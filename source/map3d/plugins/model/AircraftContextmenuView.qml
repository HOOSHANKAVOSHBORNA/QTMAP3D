import QtQuick 2.13
import QtQuick.Controls 2.13
import Crystal 1.0

Item {
    id: root
    anchors.fill: parent
    property AirplaneContextMenumodel model: AirplaneContextMenumodel {

    }

    ListView {
        id: items
        height: childrenRect.height > parent.height /2 ? parent.height : childrenRect.height
        model: root.model
        delegate: Button {
                Text {
                    anchors.centerIn: parent
                    text: display
                    color: "white"
                }
                background: Rectangle {
                    color: hovered ? "#606060" : "#404040"
                    border.width: 1
                    border.color: "#303030"
                }
                implicitWidth: 200
                implicitHeight: 30
            }

        }

}
