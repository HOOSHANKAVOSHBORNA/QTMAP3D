import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls
import Crystal
import "style"

Item {
    id: rootItem

    property var connectionConfigCpp: undefined

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
    property alias connectionStatus: connectionStatus
    property alias closeBtn: closeBtn
    property alias saveBtn: saveBtn

    //    height: parent ? parent.height : 0
    //    width: parent ? parent.width : 0
    ColumnLayout {
        spacing: 0

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 50 / Style.monitorRatio
        anchors.rightMargin: 50 / Style.monitorRatio

        Button {
            id: closeBtn
            Layout.preferredHeight: 40 / Style.monitorRatio
            Layout.preferredWidth: 40 / Style.monitorRatio
            Layout.topMargin: 30 / Style.monitorRatio
            Layout.alignment: Qt.AlignRight
            background: Image {
                source: "qrc:/Resources/close-icon.png"
            }
        }

        RowLayout {
            id: header
            Layout.fillWidth: true
            Layout.topMargin: 31 / Style.monitorRatio

            Text {
                id: titleText
                text: "Connection"
                font.pixelSize: 35 / Style.monitorRatio
                color: Style.foregroundColor
                Layout.fillWidth: true
            }

            IconImage {
                id: connectionStatus
                source: userManager.isConnected ? "qrc:/Resources/plugged.png" : "qrc:/Resources/unplugged.png"
                Layout.preferredHeight: 39 / Style.monitorRatio
                Layout.preferredWidth: 39 / Style.monitorRatio
            }
        }

        Text {
            text: "Username"
            font.pixelSize: 20 / Style.monitorRatio
            Layout.topMargin: 50 / Style.monitorRatio
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
            text: rootItem.connectionConfigCpp ? rootItem.connectionConfigCpp.username : ""
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
            text: rootItem.connectionConfigCpp ? rootItem.connectionConfigCpp.password : ""
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
            text: rootItem.connectionConfigCpp ? rootItem.connectionConfigCpp.ip : ""
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
            text: rootItem.connectionConfigCpp ? rootItem.connectionConfigCpp.port : ""
            background: Rectangle {
                color: foregroundColorTextBox
                radius: height / 2
            }
        }

        Button {
            padding: 0
            Layout.preferredHeight: 43 / Style.monitorRatio
            Layout.preferredWidth: 340 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.topMargin: 48 / Style.monitorRatio
            hoverEnabled: true

            contentItem: Text {
                text: "Test Connection"
                font.pixelSize: 15 / Style.monitorRatio
                color: parent.hovered ? "#01AED6" : Style.backgroundColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                radius: width / (Style.monitorRatio * 2)
                color: Style.foregroundColor
            }
        }
        Button {
            id: saveBtn
            padding: 0
            Layout.preferredHeight: 43 / Style.monitorRatio
            Layout.preferredWidth: 340 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.topMargin: 25 / Style.monitorRatio
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
                rootItem.connectionConfigCpp.setIp(ip.text)
                rootItem.connectionConfigCpp.setPort(port.text)
                rootItem.connectionConfigCpp.setUsername(username.text)
                rootItem.connectionConfigCpp.setPassword(password.text)
                rootItem.connectionConfigCpp.saveSettings()
            }
        }
    }
}
