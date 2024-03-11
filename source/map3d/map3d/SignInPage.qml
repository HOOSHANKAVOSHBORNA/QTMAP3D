import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import "style"
import "Components"

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
        signInBtn.enabled = true
    }

    Timer {
        id: serverResponseTimer
        interval: 5000
        onTriggered: {
            signInBtn.enabled = true
            signInBtn.background.color = Style.foregroundColor
            signInBtn.loadingRec.anchors.leftMargin = 0
            signInBtn.loadingRec.anchors.topMargin = 0
            signInBtn.loadingTimer.stop()
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
            font.pixelSize: Style.titleFontSize
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
            font.pixelSize: Style.regularFontSize
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
        font.pixelSize: Style.regularFontSize
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
            font.pixelSize: Style.regularFontSize
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
        font.pixelSize: Style.regularFontSize
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
            font.pixelSize: Style.regularFontSize
            color: "#ED4337"
            Layout.leftMargin: 5 / Style.monitorRatio
        }
    }

    Item {
        Layout.preferredHeight: 40 / Style.monitorRatio
        Layout.fillWidth: true
        Layout.topMargin: 14 / Style.monitorRatio

        LoadingButton {
            id: signInBtn
            z: 0
            anchors.fill: parent
            hoverEnabled: true

            //            onHoveredChanged: {
            //                if (hovered && enabled)
            //                    shadow.shadowEnabled = true
            //                else
            //                    shadow.shadowEnabled = false
            //            }
            contentItem: Text {
                id: signInBtnTxt
                text: "Sign in"
                color: Style.backgroundColor
                font.pixelSize: Style.regularFontSize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                signInBtn.enabled = false
                signInBtn.background.color = "silver"
                userManager.setMessage("")

                loadingRec.anchors.topMargin = -2 / Style.monitorRatio
                loadingTimer.start()
                serverResponseTimer.start()
            }
        }

        MultiEffect {
            id: shadow
            source: signInBtn
            z: signInBtn.z - 1
            enabled: true
            anchors.fill: signInBtn
            shadowColor: "black"
            shadowEnabled: signInBtn.hovered && signInBtn.enabled ? true : false
            shadowHorizontalOffset: 10 / Style.monitorRatio
            shadowVerticalOffset: 10 / Style.monitorRatio
            shadowBlur: 1
            shadowOpacity: 1
            shadowScale: 0.98
            paddingRect: Qt.rect(signInBtn.x, signInBtn.y, signInBtn.width,
                                 signInBtn.height)
        }
    }
}
