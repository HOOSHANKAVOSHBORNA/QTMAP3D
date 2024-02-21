import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls
import QtQml
import Crystal
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
    property alias connectionStatus: connectionStatus
    property alias closeBtn: closeBtn
    property alias saveBtn: saveBtn
    property alias testConnectionBtn: testConnectionBtn
    property alias testConnectionTxt: testConnectionTxt.text
    property alias testConnectionTxtColor: testConnectionTxt.color
    property alias buttonColor: backgroundRec.color
    property alias testConnectionAnimationStatus: testConnectionAnimationStatus
    property alias animationTimer: animationTimer

    Timer {
        id: animationTimer
        interval: 2000
        onTriggered: {
            testConnectionTxt.text = "Connect"
            testConnectionTxt.color = Style.backgroundColor
            buttonColor = Style.foregroundColor
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
            buttonColor.a = 1
            testConnectionBtn.enabled = true
            testConnectionBtn.hoverEnabled = true
        }
    }

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
                font.pixelSize: 35 / Style.monitorRatio
                color: Style.foregroundColor
                Layout.fillWidth: true
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
            onTextChanged: {
                rootItem.connectionConfigCpp.username = username.text
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
            onTextChanged: {
                rootItem.connectionConfigCpp.password = password.text
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
            onTextChanged: {
                rootItem.connectionConfigCpp.ip = ip.text
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
            Layout.preferredWidth: 340 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.topMargin: 48 / Style.monitorRatio
            hoverEnabled: true

            contentItem: Text {
                id: testConnectionTxt
                text: "Connect"
                font.pixelSize: 15 / Style.monitorRatio
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
                testConnectionBtn.enabled = false
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
}
