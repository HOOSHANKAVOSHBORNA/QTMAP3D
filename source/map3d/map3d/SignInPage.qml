import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "style"

ColumnLayout {
    anchors.fill: parent
    anchors.leftMargin: 50 / Style.monitorRatio
    anchors.rightMargin: 50 / Style.monitorRatio
    spacing: 0

    RowLayout {
        id: header
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 50 / Style.monitorRatio

        anchors.topMargin: 60 / Style.monitorRatio

        Text {
            id: titleText
            text: "Log in"
            font.pixelSize: 35 / Style.monitorRatio
            color: Style.foregroundColor
            Layout.fillWidth: true
        }

        Button {
            id: connectionStatus
            checkable: true
            checked: false
            background: Image {
                source: "qrc:/Resources/unplugged.png"
            }
            Layout.preferredHeight: 39 / Style.monitorRatio
            Layout.preferredWidth: 39 / Style.monitorRatio
            Layout.rightMargin: 50 / Style.monitorRatio

            onClicked: {
                leftToRightConnection.start()
                if (connectionStatus.checked) {
                    signInpage.visible = false
                    rolePage.visible = false
                    connectionPage.visible = true
                    signInBtn.visible = false
                    titleText.text = "Connection"
                    leftToRightConnection.start()
                    heightIncrease.start()
                } else {
                    titleText.text = "Log in"
                    connectionPage.visible = false
                    signInBtn.visible = true
                    if (logInPageVisible) {
                        signInpage.visible = true
                        rolePage.visible = false
                        heightDecrease.start()
                        leftToRightSignIn.start()
                    } else {
                        signInpage.visible = false
                        rolePage.visible = true
                        heightDecrease.start()
                        leftToRightRole.start()
                    }
                }
            }
        }
    }

    RowLayout {
        spacing: 0
        IconImage {
            source: "qrc:/Resources/user.png"
            Layout.preferredHeight: 23 / Style.monitorRatio
            Layout.preferredWidth: 23 / Style.monitorRatio
        }
        Text {
            id: username
            text: "Username"
            font.pixelSize: 20 / Style.monitorRatio
            Layout.leftMargin: 5 / Style.monitorRatio
            color: Style.foregroundColor
        }
    }
    TextField {
        id: usernameText
        Layout.preferredWidth: 340 / Style.monitorRatio
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
        id: password
        Layout.preferredWidth: 340 / Style.monitorRatio
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
}
