import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
Item {

    property real longitude : 0.0
    property real latitude : 0.0
    property real altitude : 0.0

    property real cordinate1 : 0.0
    property real cordinate2 : 0.0
    property real cordinate3 : 0.0

    property var names: ["Long: ", "Lat: ", "Alt: "]
    Rectangle {
        id: statusbar
        anchors.fill: parent
        color: "#404040"
        RowLayout {
            anchors.leftMargin: 4
            anchors.right: parent.right
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            layoutDirection: Qt.RightToLeft
            Repeater {

                model: [longitude, latitude, altitude]
                Label {
                    Layout.minimumWidth: hiddenn.implicitWidth
                    id: lon
                    color: "white"
                    background: Rectangle {
                        id: t
                        border.color: "black"
                        border.width: 2
                        color: "red"
                    }

                    Text {
                        anchors.centerIn: t
                        text: Number(modelData).toLocaleString(Qt.locale(), 'f', 3)
                    }
                }
            }
            Label {
                color: "white"
                text: "Lat, Lang: "
                background: Rectangle {
                    color: "red"
                }
            }

            Repeater {
                model: [cordinate1, cordinate2, cordinate3]
                anchors.leftMargin: 10
                Label {
                    Layout.minimumWidth: hiddenn.implicitWidth
                    id: mp
                    color: "white"
                    background: Rectangle {
                        id: t2
                        color: "blue"
                    }

                    Text {
                        anchors.centerIn: t2
                        text: Number(modelData).toLocaleString(Qt.locale(), 'f', 3)
                    }
                }
            }
            Label {
                color: "white"
                text: "Coordinate: "
                background: Rectangle {
                    color: "blue"
                }
            }

        }
        Rectangle {
            anchors.left: statusbar.left
            Label {
                text: "for message"
                color: "white"

            }
        }
    }
    Label {
        id: hiddenn
        text: "-1,254.000 "
        visible: false
    }
}
