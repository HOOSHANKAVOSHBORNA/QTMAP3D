import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Crystal
import "style"

Item {
    id: signUpWindow
    width: 1920 / Style.monitorRatio
    height: 1080 / Style.monitorRatio
    visible: !loginPage.windowHidden

    readonly property color backgroundColor: Qt.rgba(Style.backgroundColor.r,
                                                     Style.backgroundColor.g,
                                                     Style.backgroundColor.b,
                                                     0.60)

    property bool logInPageVisible: true

    //    onSignedIn: (status)=>{

    //    }
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
        to: 687 / Style.monitorRatio
        duration: 500
    }
    PropertyAnimation {
        id: heightDecrease
        target: containerRect
        easing.type: Easing.OutCirc
        property: "height"
        from: 687 / Style.monitorRatio
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
                heightIncrease.from = 464 / Style.monitorRatio
                heightIncrease.to = 687 / Style.monitorRatio
                heightIncrease.start()
            }
            signInBtn.onClicked: {
                logInPageVisible = false
                signInPage.visible = false
                rolePage.visible = true
                heightIncrease.from = 464 / Style.monitorRatio
                heightIncrease.to = 504 / Style.monitorRatio
                heightIncrease.start()
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
                heightIncrease.from = 504 / Style.monitorRatio
                heightIncrease.to = 687 / Style.monitorRatio
                heightIncrease.start()
                topToBottomConnection.start()
            }
            signInBtn.onClicked: {
                loginPage.signIn(signInPage.usernameTxt.text,
                                 signInPage.passwordTxt.text)
            }
            backBtn.onClicked: {
                rolePage.visible = false
                signInPage.visible = true
                heightDecrease.from = 504 / Style.monitorRatio
                heightDecrease.to = 464 / Style.monitorRatio
                heightDecrease.start()
                topToBottomSignIn.start()
            }
        }

        ConnectionConfiguration {
            id: connectionPage
            visible: false
            backBtn.onClicked: {
                if (logInPageVisible) {
                    connectionPage.visible = false
                    signInPage.visible = true
                    heightDecrease.from = 687 / Style.monitorRatio
                    heightDecrease.to = 464 / Style.monitorRatio
                    heightDecrease.start()
                    topToBottomSignIn.start()
                } else {
                    connectionPage.visible = false
                    heightDecrease.from = 687 / Style.monitorRatio
                    heightDecrease.to = 504 / Style.monitorRatio
                    heightDecrease.start()
                    rolePage.visible = true
                    topToBottomRole.start()
                }
            }
        }
    }

    //    Rectangle {
    //        id: loadingContainer
    //        width: parent.width
    //        height: 0.1 * parent.height
    //        y: 0.75 * parent.height
    //        color: "transparent"
    //        clip: true

    //        ScrollView {
    //            anchors.horizontalCenter: parent.horizontalCenter
    //            height: parent.height
    //            width: 200 / Style.monitorRatio

    //            ListView {
    //                model: loadingInfo

    //                delegate: Text {
    //                    id: loadingText
    //                    font.pixelSize: 20 / Style.monitorRatio
    //                    color: acceptionState ? "white" : "red"
    //                    text: display
    //                    leftPadding: 40 / Style.monitorRatio

    //                    //                    PropertyAnimation {
    //                    //                        id: textAnimation
    //                    //                        target: loadingText
    //                    //                        property: "y"
    //                    //                        loops: Animation.Infinite
    //                    //                        from: 1000
    //                    //                        to: -1000
    //                    //                        duration: 5000
    //                    //                        running: true
    //                    //                    }
    //                }
    //            }
    //        }
    //    }
    AnimatedImage {
        source: "qrc:/Resources/loading.gif"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100 / Style.monitorRatio
        anchors.horizontalCenter: parent.horizontalCenter
        width: 40 / Style.monitorRatio
        height: 40 / Style.monitorRatio
    }
}
