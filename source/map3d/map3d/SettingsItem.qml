import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls

import "style"

Item {
    id:rootItem

    property var model
    readonly property color foregroundColor: Qt.rgba(Style.foregroundColor.r,
                                                     Style.foregroundColor.g,
                                                     Style.foregroundColor.b,
                                                     0.20)
    readonly property color backgroundColor: Qt.rgba(Style.backgroundColor.r,
                                                     Style.backgroundColor.g,
                                                     Style.backgroundColor.b,
                                                     0.30)

    ColumnLayout{
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10 / Style.monitorRatio
        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 40 / Style.monitorRatio
            Layout.leftMargin: 5 / Style.monitorRatio
            Text{
                text: "ip "
                font.pixelSize: 15 / Style.monitorRatio
                color: Style.foregroundColor
                Layout.preferredWidth: 70 / Style.monitorRatio
            }

            TextField {
                id: ip
                Layout.preferredWidth: 200 / Style.monitorRatio
                Layout.minimumWidth: 100 / Style.monitorRatio
                Layout.fillWidth: true
                Layout.rightMargin: 15 / Style.monitorRatio
                height: 40 / Style.monitorRatio
                font.pointSize: 10 / Style.monitorRatio
                color: "black"
                text: rootItem.model.ip
                background: Rectangle {
                    color: foregroundColor
                    radius: height / 2
                }
//                onAccepted: {
//                   rootItem.model.setIp(text)
//                }
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 40 / Style.monitorRatio
            Layout.leftMargin: 5 / Style.monitorRatio
            Text{
                text: "port "
                font.pixelSize: 15 / Style.monitorRatio
                color: Style.foregroundColor
                Layout.preferredWidth: 70 / Style.monitorRatio
            }

            TextField {
                id: port
                Layout.preferredWidth: 200 / Style.monitorRatio
                Layout.minimumWidth: 100 / Style.monitorRatio
                Layout.fillWidth: true
                Layout.rightMargin: 15 / Style.monitorRatio
                height: 40 / Style.monitorRatio
                font.pointSize: 10 / Style.monitorRatio
                color: "black"
                text: rootItem.model.port
                background: Rectangle {
                    color: foregroundColor
                    radius: height / 2
                }
//                onAccepted: {
//                   rootItem.model.setPort(text)
//                }
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 40 / Style.monitorRatio
            Layout.leftMargin: 5 / Style.monitorRatio
            Text{
                text: "username "
                font.pixelSize: 15 / Style.monitorRatio
                color: Style.foregroundColor
                Layout.preferredWidth: 70 / Style.monitorRatio
            }

            TextField {
                id: username
                Layout.preferredWidth: 200 / Style.monitorRatio
                Layout.minimumWidth: 100 / Style.monitorRatio
                Layout.fillWidth: true
                Layout.rightMargin: 15 / Style.monitorRatio
                height: 40 / Style.monitorRatio
                font.pointSize: 10 / Style.monitorRatio
                color: "black"
                text: rootItem.model.username
                background: Rectangle {
                    color: foregroundColor
                    radius: height / 2
                }
//                onAccepted: {
//                   rootItem.model.setUsername(text)
//                }
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: 40 / Style.monitorRatio
            Layout.leftMargin: 5 / Style.monitorRatio
            Text{
                text: "password "
                font.pixelSize: 15 / Style.monitorRatio
                color: Style.foregroundColor
                Layout.preferredWidth: 70 / Style.monitorRatio
            }

            TextField {
                id: password
                Layout.preferredWidth: 200 / Style.monitorRatio
                Layout.minimumWidth: 100 / Style.monitorRatio
                Layout.fillWidth: true
                Layout.rightMargin: 15 / Style.monitorRatio
                height: 40 / Style.monitorRatio
                font.pointSize: 10 / Style.monitorRatio
                color: "black"
                text: rootItem.model.password
                background: Rectangle {
                    color: foregroundColor
                    radius: height / 2
                }
//                onAccepted: {
//                   rootItem.model.setPassword(text)
//                }
            }
        }

        Button {
            id: doneBtn
            padding: 0
            Layout.fillHeight: true
            Layout.preferredWidth: 65 / Style.monitorRatio
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 5 / Style.monitorRatio

            contentItem: Text {
                text: "Confirm"
                font.pixelSize: 15 / Style.monitorRatio
                color: Style.backgroundColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                radius: 10 / Style.monitorRatio
                color: Style.foregroundColor
            }
           onClicked: {
               rootItem.model.setIp(ip.text)
               rootItem.model.setPort(port.text)
               rootItem.model.setUsername(username.text)
               rootItem.model.setPassword(password.text)
               rootItem.model.saveSettings()
           }
        }
    }
}
