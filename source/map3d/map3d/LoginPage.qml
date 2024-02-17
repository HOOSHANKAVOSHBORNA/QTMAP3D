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

    //    onSignedIn: (status)=>{

    //    }
    Image {
        id: backGroundImage
        source: "qrc:/Resources/login-earth.jpg"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }

    Rectangle {
        id: connectionPopUp
        visible: false
        x: 0.5 * parent.width
        //        y: 0.2 * parent.height
        width: 440 / Style.monitorRatio
        height: 687 / Style.monitorRatio
        radius: 20 / Style.monitorRatio
        color: Style.backgroundColor
        MouseArea {
            anchors.fill: parent
            drag.target: connectionPopUp
        }
        ConnectionConfiguration {
            id: connectionPage
            anchors.fill: parent
            connectionConfigCpp: loginPage.connectionConfigCpp
            closeBtn.onClicked: {
                connectionPopUp.visible = false
            }
        }
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
        duration: 5000
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
                connectionPopUp.visible = true
                topToBottomConnection.start()
            }
            signInBtn.onClicked: {
                userManager.signIn(signInPage.usernameTxt,
                                   signInPage.passwordTxt)
                if (userManager.rolePageVisible) {
                    logInPageVisible = false
                    signInPage.visible = false
                    rolePage.visible = true
                    heightIncrease.from = 464 / Style.monitorRatio
                    heightIncrease.to = 504 / Style.monitorRatio
                    heightIncrease.start()
                    topToBottomRole.start()
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
                //                rolePage.visible = false
                connectionPopUp.visible = true
                //                heightIncrease.from = 504 / Style.monitorRatio
                //                heightIncrease.to = 687 / Style.monitorRatio
                //                heightIncrease.start()
                topToBottomConnection.start()
            }
            signInBtn.onClicked: {
                userManager.signIn(selectRole)
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

        //        ConnectionConfiguration {
        //            id: connectionPage
        //            connectionConfigCpp: loginPage.connectionConfigCpp
        //            visible: false
        //            backBtn.onClicked: {
        //                if (logInPageVisible) {
        //                    connectionPage.visible = false
        //                    signInPage.visible = true
        //                    heightDecrease.from = 687 / Style.monitorRatio
        //                    heightDecrease.to = 464 / Style.monitorRatio
        //                    heightDecrease.start()
        //                    topToBottomSignIn.start()
        //                } else {
        //                    connectionPage.visible = false
        //                    heightDecrease.from = 687 / Style.monitorRatio
        //                    heightDecrease.to = 504 / Style.monitorRatio
        //                    heightDecrease.start()
        //                    rolePage.visible = true
        //                    topToBottomRole.start()
        //                }
        //            }
        //        }
    }
}
