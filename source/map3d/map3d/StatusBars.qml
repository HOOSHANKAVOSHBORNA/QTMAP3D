import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
Item {

    property real longitude : 0.0
    property real latitude : 0.0
    property real altitude : 0.0

    property real coordinate1 : 0.0
    property real coordinate2 : 0.0
    property real coordinate3 : 0.0

    property var _font: "Liberation Serif"
    property var _lat_color: "#20bd5f"
    property var _map_color: "#22f2ad"


    Rectangle {
        id: statusbar
        anchors.fill: parent
        color: "#0c55cc"
        GridLayout {
            anchors.leftMargin: 4
            columnSpacing: 0
            anchors.right: parent.right
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            layoutDirection: Qt.RightToLeft


            Repeater {

                id: lon
                model: [latitude, longitude, altitude]

                delegate: Label {
                    Layout.minimumWidth: hiddenn.implicitWidth
                    color: "white"
                    background: Rectangle {
                        id: t
                        color: _lat_color
                    }

                    Text {
                        anchors.centerIn: t
                        text: Number(modelData).toLocaleString(Qt.locale(), 'f', 3)
                        font.family: _font
                    }
                }
            }
            Label {
                Layout.minimumWidth: hiddenn.implicitWidth
                color: "black"
                background: Rectangle {
                    id: t3
                    color: _lat_color
                }

                Text {
                    anchors.centerIn: t3
                    text: "Lat, Long: "
                    font.family: _font
                }
            }

            Rectangle {
                width: 10
            }

            Repeater {
                model: [coordinate1, coordinate2, coordinate3]
                anchors.leftMargin: 10
                Label {
                    Layout.minimumWidth: hiddenn.implicitWidth
                    id: mp
                    color: "black"
                    background: Rectangle {
                        id: t2
                        color: _map_color
                    }

                    Text {
                        anchors.centerIn: t2
                        text: Number(modelData).toLocaleString(Qt.locale(), 'f', 3)
                        font.family: _font
                    }
                }
            }
            Label {
                Layout.minimumWidth: hiddenn.implicitWidth
                color: "black"
                background: Rectangle {
                    id: t4
                    color: _map_color
                }

                Text {
                    anchors.centerIn: t4
                    text: "Coordinate: "
                    font.family: _font
                }
            }

        }
        Rectangle {
            anchors.left: statusbar.left
            anchors.leftMargin: 5
            Label {
                text: "for message"
                color: "white"
                font.family: _font

            }
        }
    }
    Label {
        id: hiddenn
        text: "-1,254.000 "
        visible: false
    }
}
