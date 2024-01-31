import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Crystal
import "style"

ApplicationWindow {
    id: signUpWindow
    width: 1920 / Style.monitorRatio
    height: 1080 / Style.monitorRatio
    minimumHeight: 600
    minimumWidth: 800
    visible: !loginPage.windowHidden
    title: qsTr("SignIn/SignUp")

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

    property bool logInPageVisible: true

    //    onSignedIn: (status)=>{

    //    }
    onClosing: {
        loginPage.onWindowClosed()
    }

    Image {
        id: backGroundImage
        source: "qrc:/Resources/login-earth.jpg"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }

    PropertyAnimation {
        id: heightIncrease
        target: containerRect
        easing.type: Easing.OutCirc
        property: "height"
        from: 464 / Style.monitorRatio
        to: 644 / Style.monitorRatio
        duration: 500
    }
    PropertyAnimation {
        id: heightDecrease
        target: containerRect
        easing.type: Easing.OutCirc
        property: "height"
        from: 644 / Style.monitorRatio
        to: 464 / Style.monitorRatio
        duration: 500
    }
    PropertyAnimation {
        id: leftToRightConnection
        target: connectionPage
        easing.type: Easing.OutCirc
        property: "x"
        from: -200
        to: 0
        duration: 500
    }
    PropertyAnimation {
        id: leftToRightSignIn
        target: signInpage
        easing.type: Easing.OutCirc
        property: "x"
        from: -200
        to: 0
        duration: 500
    }
    PropertyAnimation {
        id: leftToRightRole
        target: rolePage
        easing.type: Easing.OutCirc
        property: "x"
        from: -200
        to: 0
        duration: 500
    }

    Rectangle {
        id: containerRect
        color: "silver"
        width: 440 / Style.monitorRatio
        height: 464 / Style.monitorRatio
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 0.2 * parent.width
        radius: 20 / Style.monitorRatio
        clip: true

        Text {
            id: titleText
            text: "Log in"
            font.pixelSize: 35 / Style.monitorRatio
            color: Style.foregroundColor
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 50 / Style.monitorRatio
            anchors.topMargin: 60 / Style.monitorRatio
        }

        Button {

            background: Image {
                id: connectionStatus
                source: "qrc:/Resources/unplugged.png"
            }

            width: 39 / Style.monitorRatio
            height: 39 / Style.monitorRatio

            anchors.right: parent.right
            anchors.rightMargin: 50 / Style.monitorRatio
            anchors.top: parent.top
            anchors.topMargin: 70 / Style.monitorRatio

            onClicked: {
                signInpage.visible = false
                rolePage.visible = false
                connectionPage.visible = true
                signInBtn.visible = false
                titleText.text = "Connection"
                if (containerRect.height === 464 / Style.monitorRatio)
                    heightIncrease.start()
                leftToRightConnection.start()
            }
        }

        Item {
            id: signInpage
            visible: true
            anchors.top: titleText.bottom
            anchors.bottom: signInBtn.top
            anchors.topMargin: 20 / Style.monitorRatio
            anchors.bottomMargin: 60 / Style.monitorRatio
            ColumnLayout {
                anchors.fill: parent
                anchors.leftMargin: 50 / Style.monitorRatio
                anchors.rightMargin: 50 / Style.monitorRatio
                spacing: 0

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
                    placeholderText: "Enter your Username"
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
                    placeholderText: "Enter your Password"
                    placeholderTextColor: foregroundColorText
                    echoMode: TextField.Password
                    background: Rectangle {
                        color: foregroundColorTextBox
                        radius: height / 2
                    }
                }
            }
        }

        Item {
            id: rolePage
            visible: false
            anchors.top: titleText.bottom
            anchors.bottom: signInBtn.top
            anchors.topMargin: 20 / Style.monitorRatio
            anchors.bottomMargin: 60 / Style.monitorRatio
            ColumnLayout {
                anchors.fill: parent
                anchors.leftMargin: 50 / Style.monitorRatio
                anchors.rightMargin: 50 / Style.monitorRatio
                spacing: 0

                RowLayout {
                    spacing: 0
                    Layout.topMargin: 5 / Style.monitorRatio
                    IconImage {
                        source: "qrc:/Resources/user.png"
                        Layout.preferredHeight: 25 / Style.monitorRatio
                        Layout.preferredWidth: 25 / Style.monitorRatio
                    }

                    Text {
                        text: usernameText.text
                        font.pixelSize: 23 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                }

                Text {
                    text: "Select role"
                    Layout.topMargin: 30 / Style.monitorRatio
                    font.pixelSize: 22 / Style.monitorRatio
                    font.bold: true
                    color: Style.foregroundColor
                }

                RadioButton {
                    id: administrator
                    text: "Administrator"
                    Layout.topMargin: 12 / Style.monitorRatio
                    Layout.preferredHeight: 20 / Style.monitorRatio
                    Layout.fillWidth: true
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 20 / Style.monitorRatio
                        color: Style.foregroundColor
                        leftPadding: 20 / Style.monitorRatio
                        verticalAlignment: Text.AlignVCenter
                    }
                    indicator: Image {
                        width: 20 / Style.monitorRatio
                        height: 20 / Style.monitorRatio
                        source: administrator.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
                    }
                }

                RadioButton {
                    id: admin
                    text: "Admin"
                    Layout.topMargin: 12 / Style.monitorRatio
                    Layout.preferredHeight: 20 / Style.monitorRatio
                    Layout.fillWidth: true
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 20 / Style.monitorRatio
                        color: Style.foregroundColor
                        leftPadding: 20 / Style.monitorRatio
                        verticalAlignment: Text.AlignVCenter
                    }
                    indicator: Image {
                        width: 20 / Style.monitorRatio
                        height: 20 / Style.monitorRatio
                        source: admin.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
                    }
                }

                RadioButton {
                    id: reviewer
                    text: "Reviewer"
                    Layout.topMargin: 12 / Style.monitorRatio
                    Layout.preferredHeight: 20 / Style.monitorRatio
                    Layout.fillWidth: true
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 20 / Style.monitorRatio
                        color: Style.foregroundColor
                        leftPadding: 20 / Style.monitorRatio
                        verticalAlignment: Text.AlignVCenter
                    }
                    indicator: Image {
                        width: 20 / Style.monitorRatio
                        height: 20 / Style.monitorRatio
                        source: reviewer.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
                    }
                }
            }
        }

        ConnectionConfiguration {
            id: connectionPage
            anchors.bottom: parent.bottom
            anchors.top: titleText.bottom
            anchors.topMargin: 20 / Style.monitorRatio

            visible: false

            saveBtn.onClicked: {
                titleText.text = "Log in"
                heightDecrease.start()
                signInBtn.visible = true
                if (logInPageVisible) {
                    signInpage.visible = true
                    rolePage.visible = false
                    leftToRightSignIn.start()
                } else {
                    signInpage.visible = false
                    rolePage.visible = true
                    leftToRightRole.start()
                }

                connectionPage.visible = false
                connectionStatus.source = "qrc:/Resources/plugged.png"
            }
        }

        Button {
            id: signInBtn
            height: 40 / Style.monitorRatio
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 50 / Style.monitorRatio
            anchors.rightMargin: 50 / Style.monitorRatio
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 / Style.monitorRatio
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

                if (!signInpage.visible && rolePage.visible)
                    loginPage.signIn(username.text, password.text)

                if (signInpage.visible && !rolePage.visible) {
                    logInPageVisible = false
                    signInpage.visible = false
                    rolePage.visible = true
                    leftToRightRole.start()
                }
            }
        }
    }
}
