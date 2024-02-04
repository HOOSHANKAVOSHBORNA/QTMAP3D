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
    property alias usernametxt: usernametxt
    property alias passwordtxt: passwordtxt
    property alias connectionStatus: connectionStatus
    property alias signInBtn: signInBtn

    spacing: 0

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
                source: "qrc:/Resources/unplugged.png"
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
        id:usernametxt
        Layout.fillWidth: true
        Layout.preferredWidth: 340 / Style.monitorRatio
        Layout.topMargin: 5 / Style.monitorRatio
        height: 40 / Style.monitorRatio
        font.pointSize: 17 / Style.monitorRatio
        leftPadding: 20 / Style.monitorRatio

        color: foregroundColorText
        placeholderText: activeFocus ? "" : "Enter your Username"
        placeholderTextColor: foregroundColorText
        background: Rectangle {
            color: foregroundColorTextBox
            radius: height / 2
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
        id: passwordtxt
        Layout.fillWidth: true
        Layout.preferredWidth: 340 / Style.monitorRatio
        Layout.topMargin: 5 / Style.monitorRatio
        height: 40 / Style.monitorRatio
        font.pointSize: 17 / Style.monitorRatio
        leftPadding: 20 / Style.monitorRatio
        color: foregroundColorText
        placeholderText: activeFocus ? "" : "Enter your Password"
        placeholderTextColor: foregroundColorText
        echoMode: TextField.Password
        background: Rectangle {
            color: foregroundColorTextBox
            radius: height / 2
        }
    }

    Button {
        id: signInBtn

        Layout.preferredHeight: 40 / Style.monitorRatio
        Layout.fillWidth: true
        Layout.topMargin: 48 / Style.monitorRatio
        hoverEnabled: true
        background: Rectangle {
            color: Style.foregroundColor
            radius: width / (Style.monitorRatio * 2)
        }
        contentItem: Text {
            text: "Sign in"
            color: parent.hovered ? "#01AED6" : Style.backgroundColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {


        }
    }
}
