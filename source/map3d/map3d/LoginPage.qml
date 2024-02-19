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

    property bool logInPageVisible: true

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
        y: containerRect.y - 100
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
                connectionConfigCpp.setIsConnected(false)
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
                topToBottomConnection.start()
                connectionPopUp.visible = true
                containerRect.enabled = false
            }
            signInBtn.onClicked: {
                userManager.signIn(signInPage.usernameTxt,
                                   signInPage.passwordTxt)
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

    Connections {
        target: userManager
        function onSelectRole() {
            logInPageVisible = false
            signInPage.visible = false
            rolePage.visible = true
            heightIncrease.start()
            topToBottomRole.start()
        }
    }
}
