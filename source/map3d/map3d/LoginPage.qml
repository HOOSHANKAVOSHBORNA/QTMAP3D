import QtQuick
import QtQuick.Controls
import Crystal

LoginPage {
    width: 800
    height: 600
    minimumHeight: 600
    minimumWidth: 800
    visible: true
    title: qsTr("SignIn/SignUp")

    onSignedIn: (status)=>{

    }

    Rectangle {
        id: rectangle
        anchors.fill: parent
        color: "black"

        Image {
            id: backGroundImage
            anchors.top: parent.top
            source: "qrc:/Resources/login.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 40
            fillMode: Image.PreserveAspectFit
        }

        Column {
            id: fields
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.topMargin: 170
            spacing: 20

            TextField {
                id: usernameInput
                width: 350
                height: 60
                anchors.leftMargin: 20
                placeholderText: "Username"
                placeholderTextColor: "white"
                font.pixelSize: 20
                leftPadding: 20
                background: buttonBackground
                Rectangle {
                    id: buttonBackground
                    color: "#000000"
                    implicitWidth: 100
                    implicitHeight: 40
                    opacity: usernameInput.focus ? 0.65 : 0.3
                    radius: 50
                    border.color: "#ffffff"
                }

            }

            TextField {
                id: passwordInput
                width: 350
                height: 60
                anchors.leftMargin: 20
                echoMode: TextInput.Password
                passwordMaskDelay: 1000
                background: buttonBackground1
                placeholderText: "Password"
                placeholderTextColor: "white"
                font.pixelSize: 20
                leftPadding: 20
                Rectangle {
                    id: buttonBackground1
                    color: "#000000"
                    implicitWidth: 100
                    implicitHeight: 40
                    opacity: passwordInput.focus ? 0.65 : 0.3
                    radius: 50
                    border.color: "#ffffff"
                }

            }
        }

        Column {
            id: buttons
            y: 944
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 50
            spacing: 20

            Button {
                id: signn
                width: 350
                height: 60

                implicitWidth: Math.max(
                                   buttonBackground ? buttonBackground.implicitWidth : 0,
                                   textItem2.implicitWidth + leftPadding + rightPadding)
                implicitHeight: Math.max(
                                    buttonBackground ? buttonBackground.implicitHeight : 0,
                                    textItem2.implicitHeight + topPadding + bottomPadding)
                leftPadding: 4
                rightPadding: 4

                text: "SignIn"
                hoverEnabled: true
                enabled: true

                background: buttonBackground4
                Rectangle {
                    id: buttonBackground4
                    color: "#000000"
                    implicitWidth: 100
                    implicitHeight: 40
                    opacity: signn.pressed ? 1 : signn.hovered ? 0.6 : 0.3
                    radius: 50
                    border.color: "#41cd52"
                }

                onClicked: {
                    signIn(usernameInput.text, passwordInput.text)
                }

                contentItem: textItem2
                Text {
                    id: textItem2
                    width: 500
                    height: 100
                    text: signn.text
                    font.pixelSize: 34

                    opacity: enabled ? 1.0 : 0.3
                    color: "#41cd52"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.family: "Titillium Web ExtraLight"
                }
            }
        }
    }
}
