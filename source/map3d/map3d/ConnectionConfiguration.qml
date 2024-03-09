import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls
import QtQml
import "style"

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

    Timer {
        id: animationTimer
        interval: 2000
        onTriggered: {
            testConnectionTxt.text = "Connect"
            testConnectionTxt.color = Style.backgroundColor
            backgroundRec.color = Style.foregroundColor
            reverseAnimation.start()
        }
    }

    PropertyAnimation {
        id: testConnectionAnimationStatus
        target: testConnectionBtn
        property: "backgroundColorOpacity"
        from: 0
        to: 0.2
        duration: 100
        onFinished: {
            animationTimer.start()
        }
    }

    PropertyAnimation {
        id: reverseAnimation
        target: testConnectionBtn
        property: "backgroundColorOpacity"
        from: 0.2
        to: 1
        duration: 100
        onFinished: {
            backgroundRec.color.a = 1
            testConnectionBtn.enabled = true
            testConnectionBtn.hoverEnabled = true
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.maximumHeight: 745 / Style.monitorRatio
        spacing: 0

        Button {
            id: closeBtn
            Layout.preferredHeight: 40 / Style.monitorRatio
            Layout.preferredWidth: 40 / Style.monitorRatio
            Layout.topMargin: 30 / Style.monitorRatio
            Layout.alignment: Qt.AlignRight
            background: Image {
                source: "qrc:/Resources/close-icon.png"
            }
            onClicked: {
                rootItem.connectionConfigCpp.clearUnsavedUiData()
            }
        }

        RowLayout {
            id: header
            //            Layout.fillWidth: true
            Layout.topMargin: 31 / Style.monitorRatio

            Text {
                id: titleText
                text: "Connection"
                font.pixelSize: Style.titleFontSize
                color: Style.foregroundColor
                //                Layout.fillWidth: true
            }

            IconImage {
                id: connectionStatus
                source: connectionConfigCpp.isConnected ? "qrc:/Resources/plugged.png" : "qrc:/Resources/unplugged.png"
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
            Layout.preferredWidth: 330 / Style.monitorRatio
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
            Layout.preferredWidth: 330 / Style.monitorRatio
            //            Layout.fillWidth: true
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
            Layout.preferredWidth: 330 / Style.monitorRatio
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
            Layout.preferredWidth: 330 / Style.monitorRatio
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

        Button {
            id: testConnectionBtn
            property alias backgroundColorOpacity: backgroundRec.color.a
            property alias textColor: testConnectionTxt.color
            padding: 0
            Layout.preferredHeight: 43 / Style.monitorRatio
            Layout.preferredWidth: 330 / Style.monitorRatio
            Layout.topMargin: 48 / Style.monitorRatio
            hoverEnabled: true

            contentItem: Text {
                id: testConnectionTxt
                text: "Connect"
                font.pixelSize: Style.regularFontSize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                id: backgroundRec
                radius: width / (Style.monitorRatio * 2)
                color: Style.foregroundColor
            }

            Binding {
                target: testConnectionTxt
                property: "color"
                value: {
                    if (animationTimer.running)
                        return connectionConfigCpp.isConnected ? "#206900" : "#690000"
                    else
                        return testConnectionBtn.hovered
                                && backgroundRec.color.a == 1 ? "#01AED6" : Style.backgroundColor
                }
            }

            onClicked: {
                backgroundRec.color.a = 0.5
                connectionButtonClicked = true
                connectionConfigCpp.testConnection()
                testConnectionBtn.enabled = false
            }
        }
        Button {
            id: saveBtn
            padding: 0
            Layout.preferredHeight: 43 / Style.monitorRatio
            Layout.preferredWidth: 330 / Style.monitorRatio
            Layout.topMargin: 25 / Style.monitorRatio
            hoverEnabled: true

            contentItem: Text {
                text: "Save changes"
                font.pixelSize: Style.regularFontSize
                color: saveBtn.hovered ? "#01AED6" : Style.backgroundColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                radius: width / (Style.monitorRatio * 2)
                color: Style.foregroundColor
            }
            onClicked: {
                rootItem.connectionConfigCpp.saveSettings()
            }
        }
    }

    Connections {
        target: connectionConfigCpp

        function onIsConnectedChanged() {
            if (connectionButtonClicked && connectionConfigCpp.isConnected) {
                testConnectionTxt.text = "Connection Success"
                testConnectionTxt.color = "#206900"
                backgroundRec.color = "#206900"
                testConnectionAnimationStatus.start()
                connectionButtonClicked = false
            }
            if (connectionButtonClicked && !connectionConfigCpp.isConnected) {
                testConnectionTxt.text = "Connection Failed"
                testConnectionTxt.color = "#690000"
                backgroundRec.color = "#690000"
                testConnectionAnimationStatus.start()
                connectionButtonClicked = false
            }
        }
    }
}
