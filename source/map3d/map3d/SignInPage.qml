import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "style"

ColumnLayout {

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
    property var connectionConfigCpp: undefined
    property var userManager: undefined
    property alias usernameTxt: usernameTxt.text
    property alias passwordTxt: passwordTxt.text
    property alias connectionStatus: connectionStatus
    property alias signInBtn: signInBtn
    property alias serverResponseTimer: serverResponseTimer

    signal signInResponse
    spacing: 0

    onSignInResponse: {
        backgroundRec.color.a = 1
        signInBtn.enabled = true
    }

    Timer {
        id: serverResponseTimer
        interval: 5000
        onTriggered: {
            backgroundRec.color.a = 1
            signInBtn.enabled = true
            userManager.setMessage("No Response")
        }
    }

    RowLayout {
        id: header
        spacing: 0
        Layout.fillWidth: true
        Layout.preferredHeight: 40 / Style.monitorRatio
        Layout.topMargin: 60 / Style.monitorRatio
        Text {
            id: titleText
            text: "Log in"
            font.pixelSize: 35 / Style.monitorRatio
            color: Style.foregroundColor
            Layout.fillWidth: true
        }

        Button {
            id: connectionStatus
            background: Image {
                source: connectionConfigCpp.isConnected ? "qrc:/Resources/plugged.png" : "qrc:/Resources/unplugged.png"
            }
            Layout.preferredHeight: 39 / Style.monitorRatio
            Layout.preferredWidth: 39 / Style.monitorRatio
            onClicked: {

            }
        }
    }

    RowLayout {
        spacing: 0
        Layout.fillWidth: true
        Layout.topMargin: 50 / Style.monitorRatio
        IconImage {
            source: "qrc:/Resources/user.png"
            Layout.preferredHeight: 23 / Style.monitorRatio
            Layout.preferredWidth: 23 / Style.monitorRatio
        }
        Text {
            text: "Username"
            font.pixelSize: 20 / Style.monitorRatio
            Layout.leftMargin: 5 / Style.monitorRatio
            color: Style.foregroundColor
        }
    }
    TextField {
        id: usernameTxt
        Layout.fillWidth: true
        Layout.preferredWidth: 340 / Style.monitorRatio
        Layout.topMargin: 5 / Style.monitorRatio
        Layout.preferredHeight: 40 / Style.monitorRatio
        font.pixelSize: 17 / Style.monitorRatio
        leftPadding: 20 / Style.monitorRatio

        color: foregroundColorText
        text: "test2"
        placeholderText: activeFocus ? "" : "Enter your Username"
        placeholderTextColor: foregroundColorText
        background: Rectangle {
            color: foregroundColorTextBox
            radius: height / 2
            border.width: (userManager.message === "") ? 0 : 1
            border.color: (userManager.message === "") ? "transparent" : "#66ED4337"
        }
    }
    RowLayout {
        spacing: 0
        Layout.fillWidth: true
        Layout.topMargin: 20 / Style.monitorRatio
        IconImage {
            source: "qrc:/Resources/lock-closed.png"
            Layout.preferredHeight: 23 / Style.monitorRatio
            Layout.preferredWidth: 23 / Style.monitorRatio
        }
        Text {
            text: "Password"
            font.pixelSize: 20 / Style.monitorRatio
            Layout.leftMargin: 5 / Style.monitorRatio
            color: Style.foregroundColor
        }
    }
    TextField {
        id: passwordTxt
        Layout.fillWidth: true
        Layout.preferredWidth: 340 / Style.monitorRatio
        Layout.topMargin: 5 / Style.monitorRatio
        Layout.preferredHeight: 40 / Style.monitorRatio
        font.pixelSize: 17 / Style.monitorRatio
        leftPadding: 20 / Style.monitorRatio
        color: foregroundColorText
        placeholderText: activeFocus ? "" : "Enter your Password"
        placeholderTextColor: foregroundColorText
        echoMode: TextField.Password
        text: "1234"
        background: Rectangle {
            color: foregroundColorTextBox
            radius: height / 2
            border.width: (userManager.message === "") ? 0 : 1
            border.color: (userManager.message === "") ? "transparent" : "#66ED4337"
        }
    }

    RowLayout {
        spacing: 0
        opacity: (userManager.message === "") ? 0 : 1
        Layout.topMargin: 14 / Style.monitorRatio

        Image {
            source: "qrc:/Resources/error.png"
            Layout.preferredHeight: 20 / Style.monitorRatio
            Layout.preferredWidth: 20 / Style.monitorRatio
        }
        Text {
            text: userManager.message
            font.pixelSize: 17 / Style.monitorRatio
            color: "#ED4337"
            Layout.leftMargin: 5 / Style.monitorRatio
        }
    }

    Button {
        id: signInBtn
        Layout.preferredHeight: 40 / Style.monitorRatio
        Layout.fillWidth: true
        Layout.topMargin: 14 / Style.monitorRatio
        hoverEnabled: true
        background: Rectangle {
            id: backgroundRec
            color: Style.foregroundColor
            radius: width / (Style.monitorRatio * 2)
        }
        contentItem: Text {
            id: signInBtnTxt
            text: "Sign in"
            color: parent.hovered && parent.enabled
                   && backgroundRec.color.a == 1 ? "#01AED6" : Style.backgroundColor
            font.pixelSize: 20 / Style.monitorRatio
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {
            backgroundRec.color.a = 0.5
            signInBtn.enabled = false
            userManager.setMessage("")
            serverResponseTimer.start()
        }
    }
}
