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
        z: backGroundImage.z + 1
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
            connectionConfigCpp: loginPage.connectionConfigCpp
            closeBtn.onClicked: {
                connectionPopUp.visible = false
                containerRect.enabled = true
            }
            saveBtn.onClicked: {
                connectionPopUp.visible = false
                containerRect.enabled = true
            }
            testConnectionBtn.onClicked: {
                buttonColor.a = 0.5
                connectionConfigCpp.testConnection()
            }

            Connections {
                target: connectionConfigCpp

                function onIsConnectedChanged() {
                    if (connectionConfigCpp.isConnected) {
                        connectionPage.testConnectionTxt = "Connected"
                        connectionPage.testConnectionTxtColor = "#206900"
                        connectionPage.buttonColor = "#206900"
                        connectionPage.testConnectionAnimationStatus.start()
                    } else {
                        connectionPage.testConnectionTxt = "Disconnected"
                        connectionPage.testConnectionTxtColor = "#690000"
                        connectionPage.buttonColor = "#690000"
                        connectionPage.testConnectionAnimationStatus.start()
                    }
                }
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
                }
                function onSignedIn() {
                    signInPage.uiSignedIn()
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
