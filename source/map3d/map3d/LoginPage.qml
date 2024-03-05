import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Crystal
import "style"

Item {
    id: loginPage

    property var userManager: undefined
    property var connectionConfigCpp: undefined

    width: 1920 / Style.monitorRatio
    height: 1080 / Style.monitorRatio

    Rectangle {
        id: blueBackground
        width: parent.width / 2
        height: parent.height
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.rgba(Style.foregroundColor.r,
                               Style.foregroundColor.g,
                               Style.foregroundColor.b, 0.3)
            }

            GradientStop {
                position: 1.0
                color: Qt.rgba(Style.foregroundColor.r,
                               Style.foregroundColor.g,
                               Style.foregroundColor.b, 0.1)
            }
        }
    }
    PropertyAnimation {
        id: heightIncrease
        target: containerRect
        easing.type: Easing.OutCirc
        property: "height"
        from: 464 / Style.monitorRatio
        to: 525 / Style.monitorRatio
        duration: 500
    }
    PropertyAnimation {
        id: heightDecrease
        target: containerRect
        easing.type: Easing.OutCirc
        property: "height"
        from: 525 / Style.monitorRatio
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
        id: connectionPopUp
        visible: false
        x: containerRect.x
        y: (parent.height - connectionPopUp.height) / 2
        z: containerRect.z + 1
        clip: true
        width: 440 / Style.monitorRatio
        height: 745 / Style.monitorRatio
        radius: 20 / Style.monitorRatio
        color: Style.backgroundColor
        MouseArea {
            anchors.fill: parent
            drag.target: connectionPopUp
        }
        ConnectionConfiguration {
            id: connectionPage

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 50 / Style.monitorRatio
            anchors.rightMargin: 50 / Style.monitorRatio
            connectionConfigCpp: loginPage.connectionConfigCpp
            closeBtn.onClicked: {
                connectionPopUp.visible = false
                containerRect.enabled = true
            }
            saveBtn.onClicked: {
                connectionPopUp.visible = false
                containerRect.enabled = true
            }
        }
    }

    Rectangle {
        id: containerRect
        z: 1
        color: Style.backgroundColor
        width: 440 / Style.monitorRatio
        height: 464 / Style.monitorRatio
        anchors.centerIn: blueBackground
        radius: 20 / Style.monitorRatio
        clip: true

        SignInPage {
            id: signInPage
            connectionConfigCpp: loginPage.connectionConfigCpp
            userManager: loginPage.userManager
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 50 / Style.monitorRatio
            anchors.rightMargin: 50 / Style.monitorRatio
            connectionStatus.onClicked: {
                topToBottomConnection.start()
                connectionPopUp.visible = true
                containerRect.enabled = false
            }
            signInBtn.onClicked: {
                userManager.signIn(signInPage.usernameTxt,
                                   signInPage.passwordTxt)
            }

            Connections {
                target: userManager
                function onSelectRole() {
                    signInPage.visible = false
                    rolePage.visible = true
                    heightIncrease.start()
                    topToBottomRole.start()
                    signInPage.signInResponse()
                    signInPage.serverResponseTimer.stop()
                }
                function onSignedIn() {
                    signInPage.signInResponse()
                    signInPage.serverResponseTimer.stop()
                    signInPage.signInBtn.loadingAnimation.stop()
                    signInPage.signInBtn.isWaiting = false
                    signInPage.signInBtn.height = 40 / Style.monitorRatio
                }
                function onSignInFailed() {
                    signInPage.signInResponse()
                    signInPage.serverResponseTimer.stop()
                    signInPage.signInBtn.loadingAnimation.stop()
                    signInPage.signInBtn.isWaiting = false
                    signInPage.signInBtn.height = 40 / Style.monitorRatio
                    userManager.setMessage("")
                }
            }
        }

        RoleSelectPage {
            id: rolePage
            roleSelectionModel: userManager.roleSelectionModel()
            visible: false
            usernameTxt: signInPage.usernameTxt
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 50 / Style.monitorRatio
            anchors.rightMargin: 50 / Style.monitorRatio

            connectionStatus.onClicked: {
                connectionPopUp.visible = true
                topToBottomConnection.start()
                containerRect.enabled = false
            }
            signInBtn.onClicked: {
                userManager.signIn(selectRole)
            }
            backBtn.onClicked: {
                rolePage.visible = false
                signInPage.visible = true
                heightDecrease.start()
                topToBottomSignIn.start()
            }
        }
    }
}
