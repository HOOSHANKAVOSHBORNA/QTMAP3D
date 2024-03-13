import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls
import QtQml
import "style"
import "Components"

Item {
    id: rootItem
    property var connectionConfigCpp: undefined
    property bool connectionButtonClicked: false
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
    property alias closeBtn: closeBtn
    property alias saveBtn: saveBtn

    ColumnLayout {
        Layout.fillWidth: true
        Layout.maximumHeight: 745 / Style.monitorRatio
        spacing: 0

        Timer {
            id: connectionWaitTimer
            interval: 5000
            onTriggered: {
                connectBtn.loadingTimer.stop()
                connectBtn.enabled = true
                connectBtn.buttonColor = Style.foregroundColor
                if (connectionConfigCpp.isConnected)
                    messageText.text = "Connection Success"
                else
                    messageText.text = "Connection Failure"
            }
        }

        IconButton {
            id: closeBtn
            Layout.preferredHeight: 40 / Style.monitorRatio
            Layout.preferredWidth: 40 / Style.monitorRatio
            Layout.topMargin: 30 / Style.monitorRatio
            Layout.alignment: Qt.AlignRight
            iconImageSource: "qrc:/Resources/close-icon.png"
            backgroundColor: "transparent"
            onClicked: {
                rootItem.connectionConfigCpp.clearUnsavedUiData()
            }
        }

        RowLayout {
            id: header
            Layout.fillWidth: true
            Layout.topMargin: 31 / Style.monitorRatio

            Text {
                id: titleText
                text: "Connection"
                font.pixelSize: Style.titleFontSize
                color: Style.foregroundColor
                Layout.fillWidth: true
            }

            IconImage {
                id: connectionStatus
                source: (connectionConfigCpp
                         && connectionConfigCpp.isConnected) ? "qrc:/Resources/plugged.png" : "qrc:/Resources/unplugged.png"
                Layout.preferredHeight: 39 / Style.monitorRatio
                Layout.preferredWidth: 39 / Style.monitorRatio
            }
        }

        Text {
            text: "Username"
            font.pixelSize: Style.regularFontSize
            Layout.topMargin: 50 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id: username
            Layout.preferredWidth: 320 / Style.monitorRatio
            Layout.topMargin: 5 / Style.monitorRatio
            Layout.preferredHeight: 43 / Style.monitorRatio
            font.pixelSize: Style.regularFontSize
            leftPadding: 20 / Style.monitorRatio
            color: foregroundColorText
            placeholderTextColor: foregroundColorText
            placeholderText: "Username"
            text: rootItem.connectionConfigCpp ? rootItem.connectionConfigCpp.username : ""
            background: Rectangle {
                color: foregroundColorTextBox
                radius: height / 2
            }
            onTextChanged: {
                rootItem.connectionConfigCpp.username = username.text
            }
        }

        Text {
            text: "Password"
            font.pixelSize: Style.regularFontSize
            Layout.topMargin: 20 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id: password
            Layout.preferredWidth: 320 / Style.monitorRatio
            Layout.topMargin: 5 / Style.monitorRatio
            Layout.preferredHeight: 43 / Style.monitorRatio
            font.pixelSize: Style.regularFontSize
            leftPadding: 20 / Style.monitorRatio
            color: foregroundColorText
            placeholderTextColor: foregroundColorText
            placeholderText: "Password"
            text: rootItem.connectionConfigCpp ? rootItem.connectionConfigCpp.password : ""
            background: Rectangle {
                color: foregroundColorTextBox
                radius: height / 2
            }
            onTextChanged: {
                rootItem.connectionConfigCpp.password = password.text
            }
        }

        Text {
            text: "IP"
            font.pixelSize: Style.regularFontSize
            Layout.topMargin: 20 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id: ip
            Layout.preferredWidth: 320 / Style.monitorRatio
            Layout.topMargin: 5 / Style.monitorRatio
            Layout.preferredHeight: 43 / Style.monitorRatio
            font.pixelSize: Style.regularFontSize
            leftPadding: 20 / Style.monitorRatio
            color: foregroundColorText
            placeholderTextColor: foregroundColorText
            placeholderText: "IP"
            text: rootItem.connectionConfigCpp ? rootItem.connectionConfigCpp.ip : ""
            background: Rectangle {
                color: foregroundColorTextBox
                radius: height / 2
            }
            onTextChanged: {
                rootItem.connectionConfigCpp.ip = ip.text
            }
        }

        Text {
            text: "Port"
            font.pixelSize: Style.regularFontSize
            Layout.topMargin: 20 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id: port
            Layout.preferredWidth: 320 / Style.monitorRatio
            Layout.topMargin: 5 / Style.monitorRatio
            Layout.preferredHeight: 43 / Style.monitorRatio
            font.pixelSize: Style.regularFontSize
            leftPadding: 20 / Style.monitorRatio
            color: foregroundColorText
            placeholderTextColor: foregroundColorText
            placeholderText: "port"
            text: rootItem.connectionConfigCpp ? rootItem.connectionConfigCpp.port : ""
            background: Rectangle {
                color: foregroundColorTextBox
                radius: height / 2
            }
            onTextChanged: {
                rootItem.connectionConfigCpp.port = Number(port.text)
            }
        }

        RowLayout {
            spacing: 0
            opacity: messageText.text ? 1 : 0
            Layout.topMargin: 14 / Style.monitorRatio

            Image {
                source: connectionConfigCpp.isConnected ? "qrc:/Resources/error.png" : "qrc:/Resources/error.png"
                Layout.preferredHeight: 20 / Style.monitorRatio
                Layout.preferredWidth: 20 / Style.monitorRatio
            }
            Text {
                id: messageText
                font.pixelSize: Style.regularFontSize
                color: connectionConfigCpp.isConnected ? "green" : "red"
                Layout.leftMargin: 5 / Style.monitorRatio
            }
        }

        LoadingButton {
            id: connectBtn
            Layout.preferredHeight: 43 / Style.monitorRatio
            Layout.preferredWidth: 320 / Style.monitorRatio
            Layout.topMargin: 10 / Style.monitorRatio
            buttonText: "Connect"
            buttonColor: Style.foregroundColor
            button.onClicked: {
                backgroundColorOpacity = 0.5
                connectionButtonClicked = true
                connectionConfigCpp.testConnection()
                connectBtn.startButtonLoading()
                loadingTimer.start()
                connectionWaitTimer.start()
                connectBtn.buttonColor = "silver"
                connectBtn.enabled = false
                messageText.text = ""
            }
        }
        CustomButton {
            id: saveBtn
            padding: 0
            Layout.preferredHeight: 43 / Style.monitorRatio
            Layout.preferredWidth: 320 / Style.monitorRatio
            Layout.topMargin: 25 / Style.monitorRatio
            buttonText: "Save Changes"
            buttonColor: Style.foregroundColor
            onClicked: {
                rootItem.connectionConfigCpp.saveSettings()
            }
        }
    }

    Connections {
        target: applicationCpp.connectionConfigCpp

        function onIsConnectedChanged() {
            if (connectionButtonClicked && connectionConfigCpp.isConnected) {
                connectionButtonClicked = false
            }
            if (connectionButtonClicked && !connectionConfigCpp.isConnected) {
                connectionButtonClicked = false
            }
        }
    }
}
