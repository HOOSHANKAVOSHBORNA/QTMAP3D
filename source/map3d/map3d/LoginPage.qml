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

    readonly property color backgroundColor: Qt.rgba(Style.backgroundColor.r,
                                                     Style.backgroundColor.g,
                                                     Style.backgroundColor.b,
                                                     0.60)

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
        id: topToBottomConnection
        target: connectionPage
        easing.type: Easing.OutCirc
        property: "y"
        from: -500
        to: 0
        duration: 500
    }
    PropertyAnimation {
        id: topToBottomSignIn
        target: signInPage
        easing.type: Easing.OutCirc
        property: "y"
        from: -500
        to: 0
        duration: 500
    }
    PropertyAnimation {
        id: topToBottomRole
        target: rolePage
        easing.type: Easing.OutCirc
        property: "y"
        from: -500
        to: 0
        duration: 500
    }

    Rectangle {
        id: containerRect
        color: backgroundColor
        width: 440 / Style.monitorRatio
        height: 464 / Style.monitorRatio
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 0.2 * parent.width
        radius: 20 / Style.monitorRatio
        clip: true

        SignInPage {
            id: signInPage
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 50 / Style.monitorRatio
            anchors.rightMargin: 50 / Style.monitorRatio
            connectionStatus.onClicked: {
                signInPage.visible = false
                connectionPage.visible = true
                topToBottomConnection.start()
                heightIncrease.start()
            }
            signInBtn.onClicked: {
                logInPageVisible = false
                signInPage.visible = false
                rolePage.visible = true
                topToBottomRole.start()
            }
        }

        RoleSelectPage {
            id: rolePage
            visible: false
            usernameTxt: signInPage.usernameTxt
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 50 / Style.monitorRatio
            anchors.rightMargin: 50 / Style.monitorRatio

            connectionStatus.onClicked: {
                rolePage.visible = false
                connectionPage.visible = true
                heightIncrease.start()
                topToBottomConnection.start()
            }
            signInBtn.onClicked: {
                loginPage.signIn(signInPage.usernameTxt.text,
                                 signInPage.passwordTxt.text)
            }
        }

        ConnectionConfiguration {
            id: connectionPage
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 50 / Style.monitorRatio
            anchors.rightMargin: 50 / Style.monitorRatio
            visible: false

            connectionStatus.onClicked: {
                if (logInPageVisible) {
                    connectionPage.visible = false
                    signInPage.visible = true
                    topToBottomSignIn.start()
                } else {
                    connectionPage.visible = false
                    rolePage.visible = true
                    topToBottomRole.start()
                }
                heightDecrease.start()
            }
        }
    }
}
