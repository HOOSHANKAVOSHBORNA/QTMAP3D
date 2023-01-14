import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Crystal 1.0

Item {
    id: root
    anchors.fill: parent
    property AircraftContextMenumodel model: AircraftContextMenumodel {

    }

    ScrollView {
        anchors.fill: parent
//        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        ScrollBar.horizontal.interactive: true
        ListView {
        id: items
        height: 200
        model: root.model
        delegate: Button {
                Text {
                    anchors.centerIn: parent
                    text: disp
                    color: "white"
                }
                background: Rectangle {
                    color: hovered ? "#606060" : "#404040"
                    border.width: 1
                    border.color: "#303030"
                }
                implicitWidth: 200
                implicitHeight: 25
                onClicked: root.model.itemClicked(index, disp)
            }


    }}


}
