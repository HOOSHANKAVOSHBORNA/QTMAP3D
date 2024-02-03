import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls
import Crystal
import "style"

Item {
    id: rootItem

    property var model: ConnectionConfigurationInstance.getConnectionConfiguration()
    readonly property color foregroundColorTextBox: Qt.rgba(
                                                        Style.foregroundColor.r,
                                                        Style.foregroundColor.g,
                                                        Style.foregroundColor.b,
                                                        0.20)
    readonly property color foregroundColorText: Qt.rgba(
                                                     Style.foregroundColor.r,
                                                     Style.foregroundColor.g,
                                                     Style.foregroundColor.b,
                                                     0.50)

    height: parent ? parent.height : 0
    width: parent ? parent.width : 0

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 50 / Style.monitorRatio
        anchors.rightMargin: 50 / Style.monitorRatio
        spacing: 0

        Text {
            text: "Username"
            font.pixelSize: 20 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id: username
            Layout.preferredWidth: 340 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.topMargin: 5 / Style.monitorRatio
            height: 43 / Style.monitorRatio
            font.pointSize: 17 / Style.monitorRatio
            leftPadding: 20 / Style.monitorRatio
            color: foregroundColorText
            placeholderTextColor: foregroundColorText
            placeholderText: "Username"
            text: rootItem.model ? rootItem.model.username : ""
            background: Rectangle {
                color: foregroundColorTextBox
                radius: height / 2
            }
        }

        Text {
            text: "Password"
            font.pixelSize: 20 / Style.monitorRatio
            Layout.topMargin: 20 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id: password
            Layout.preferredWidth: 340 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.topMargin: 5 / Style.monitorRatio
            height: 43 / Style.monitorRatio
            font.pointSize: 17 / Style.monitorRatio
            leftPadding: 20 / Style.monitorRatio
            color: foregroundColorText
            placeholderTextColor: foregroundColorText
            placeholderText: "Password"
            text: rootItem.model ? rootItem.model.password : ""
            background: Rectangle {
                color: foregroundColorTextBox
                radius: height / 2
            }
        }

        Text {
            text: "IP"
            font.pixelSize: 20 / Style.monitorRatio
            Layout.topMargin: 20 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id: ip
            Layout.preferredWidth: 340 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.topMargin: 5 / Style.monitorRatio
            height: 43 / Style.monitorRatio
            font.pointSize: 17 / Style.monitorRatio
            leftPadding: 20 / Style.monitorRatio
            color: foregroundColorText
            placeholderTextColor: foregroundColorText
            placeholderText: "IP"
            text: rootItem.model ? rootItem.model.ip : ""
            background: Rectangle {
                color: foregroundColorTextBox
                radius: height / 2
            }
        }

        Text {
            text: "Port"
            font.pixelSize: 20 / Style.monitorRatio
            Layout.topMargin: 20 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id: port
            Layout.preferredWidth: 340 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.topMargin: 5 / Style.monitorRatio
            height: 43 / Style.monitorRatio
            font.pointSize: 17 / Style.monitorRatio
            leftPadding: 20 / Style.monitorRatio
            color: foregroundColorText
            placeholderTextColor: foregroundColorText
            placeholderText: "port"
            text: rootItem.model ? rootItem.model.port : ""
            background: Rectangle {
                color: foregroundColorTextBox
                radius: height / 2
            }
        }

        Button {
            id: saveBtn
            padding: 0
            Layout.preferredHeight: 43 / Style.monitorRatio
            Layout.preferredWidth: 340 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.topMargin: 48 / Style.monitorRatio
            hoverEnabled: true

            contentItem: Text {
                text: "Save changes"
                font.pixelSize: 15 / Style.monitorRatio
                color: parent.hovered ? "#01AED6" : Style.backgroundColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                radius: width / (Style.monitorRatio * 2)
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
