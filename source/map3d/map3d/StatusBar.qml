import QtQuick 2.5
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
    property var _lat_color: "transparent"
    property var _map_color: "transparent"

    property var timer: 0
    property var message: ""

    property var fe: ["f", modeMap == "geocentric" ? "f" : "E", modeMap == "geocentric" ? "f" : "E"]

    function showMessage(messages, timerr){
        time.running = false
        timer = timerr
        message = messages
        time.running = true
    }

    function stopp(){
        if(timer != -1)
            message = "Ready"
    }


    Rectangle {
        id: statusbar
        anchors.fill: parent
        color: "#454545"
        GridLayout {
            anchors.leftMargin: 4
            columnSpacing: 0
            anchors.right: parent.right
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            layoutDirection: Qt.RightToLeft
            id: statusLayout
            Repeater {
                id: lon
                model: [altitude, longitude, latitude]
                delegate: Label {
                    Layout.minimumWidth: hiddenn.implicitWidth
                    background: Rectangle {
                        id: t
                        color: _lat_color
                    }
                    Text {
                        color: "white"
                        anchors.right: t.right
                        anchors.rightMargin: 8
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
                    color: "white"
                    anchors.centerIn: t3
                    text: "Long, Lat: "
                    font.family: _font
                }
            }

            Rectangle {
                width: 2
            }

            Repeater {
                id: mp
                model: [coordinate3, coordinate2, coordinate1]
                anchors.leftMargin: 10
                Label {
                    Layout.minimumWidth: hiddenn.implicitWidth
                    color: "black"
                    background: Rectangle {
                        id: t2
                        color: _map_color
                    }
                    Text {
                        color: "white"
                        anchors.right: t2.right
                        anchors.rightMargin: 6
                        text: Number(modelData).toLocaleString(Qt.locale(), fe[index], 3)
                        font.family: _font
                    }
                }
            }
            Label {
                id: coordinateText
                Layout.minimumWidth: hiddenn.implicitWidth
                color: "black"
                background: Rectangle {
                    id: t4
                    color: _map_color
                }

                Text {
                    color: "white"
                    anchors.centerIn: t4
                    text: "Coordinate: "
                    font.family: _font
                }
            }

        }
        Rectangle {
            id: msg
            anchors.left: statusbar.left
            anchors.leftMargin: 5
            width: wnd.width - coordinateText.width*9
            Timer {
                id: time
                interval: timer === -1 ? 100000: timer; repeat: false
                onTriggered: stopp()

            }

            Label {
                id: timeMessage
                text: message
                color: "white"
                font.family: _font
                width: parent.width
                wrapMode: Text.WrapAnywhere
                ToolTip {
                    width: 500
                    height: ty.height + 10
                    Text{
                        id: ty
                        text: message
                        wrapMode: Text.WrapAnywhere
                        width: 500

                    }

                    background: Rectangle {
                        color: "#909090"
                        radius: 4
                    }

                    visible:  hide.tightBoundingRect.width > msg.width + message.length ? ma.containsMouse : false
                }

                MouseArea {
                    id: ma
                    anchors.fill: parent
                    hoverEnabled: true
                }

            }
            Label {
                text: "..."
                color: "white"
                anchors.leftMargin: -5
                anchors.left: timeMessage.right
                visible:  hide.tightBoundingRect.width > msg.width + message.length ? true : false
            }
        }
    }
    Label {
        id: hiddenn
        text: "-1,254.000 "
        visible: false
    }
    TextMetrics {
        id: hide
        text: message
    }
}
