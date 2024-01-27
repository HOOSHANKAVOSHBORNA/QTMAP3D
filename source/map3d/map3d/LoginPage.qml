import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Crystal
import "style"


ApplicationWindow {
    id:signUpWindow
    width: 1080 / Style.monitorRatio
    height: 1920 / Style.monitorRatio
    minimumHeight: 600
    minimumWidth: 800
    visible: !loginPage.windowHidden
    title: qsTr("SignIn/SignUp")

    readonly property color foregroundColor: Qt.rgba(Style.foregroundColor.r,
                                                     Style.foregroundColor.g,
                                                     Style.foregroundColor.b,
                                                     0.50)

//    onSignedIn: (status)=>{

//    }

onClosing:{
 loginPage.onWindowClosed()
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

        Rectangle{
            color: "silver"
            width: 440 / Style.monitorRatio
            height: 464 / Style.monitorRatio
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 0.2 * parent.width
            radius: 20 / Style.monitorRatio

            Text{
                id:logInText
                text: "Log in"
                font.pixelSize: 35 / Style.monitorRatio
                color: Style.foregroundColor
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 50 / Style.monitorRatio
                anchors.topMargin: 60 / Style.monitorRatio
            }

            ColumnLayout{
                id:firstPage
                visible: true
                anchors.top: logInText.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: signInBtn.top
                anchors.leftMargin: 50 / Style.monitorRatio
                anchors.rightMargin: 50 / Style.monitorRatio
                anchors.topMargin: 20 / Style.monitorRatio
                anchors.bottomMargin: 60 / Style.monitorRatio
                spacing: 0

                RowLayout{
                    spacing: 0
                    Layout.topMargin: 30 / Style.monitorRatio
                    IconImage{
                        source: "qrc:/Resources/user.png"
                        Layout.preferredHeight: 25 / Style.monitorRatio
                        Layout.preferredWidth: 25 / Style.monitorRatio
                    }

                    Text {
                        text: "Alireza Nabati"
                        font.pixelSize: 23 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                }

                Text{
                    text: "Select role"
                    Layout.topMargin: 30 / Style.monitorRatio
                    font.pixelSize: 22 / Style.monitorRatio
                    font.bold: true
                    color: Style.foregroundColor
                }

                RadioButton{
                    text: "Administrator"
                    Layout.topMargin: 12 / Style.monitorRatio
                    Layout.preferredHeight: 20 / Style.monitorRatio
                    Layout.fillWidth: true
                    contentItem: Text{
                        text: parent.text
                        font.pixelSize: 20 / Style.monitorRatio
                        color: Style.foregroundColor
                        leftPadding: 20 / Style.monitorRatio
                        verticalAlignment: Text.AlignVCenter
                    }
                    indicator: Image{
                      width: 20 / Style.monitorRatio
                      height: 20 / Style.monitorRatio
                      source: "qrc:/Resources/circle.png"
                    }
                }

                RadioButton{
                    text: "Admin"
                    Layout.topMargin: 12 / Style.monitorRatio
                    Layout.preferredHeight: 20 / Style.monitorRatio
                    Layout.fillWidth: true
                    contentItem: Text{
                        text: parent.text
                        font.pixelSize: 20 / Style.monitorRatio
                        color: Style.foregroundColor
                        leftPadding: 20 / Style.monitorRatio
                        verticalAlignment: Text.AlignVCenter
                    }
                    indicator: Image{
                      width: 20 / Style.monitorRatio
                      height: 20 / Style.monitorRatio
                      source: "qrc:/Resources/circle.png"
                    }
                }

                RadioButton{
                    text: "Reviewer"
                    Layout.topMargin: 12 / Style.monitorRatio
                    Layout.preferredHeight: 20 / Style.monitorRatio
                    Layout.fillWidth: true
                    contentItem: Text{
                        text: parent.text
                        font.pixelSize: 20 / Style.monitorRatio
                        color: Style.foregroundColor
                        leftPadding: 20 / Style.monitorRatio
                        verticalAlignment: Text.AlignVCenter
                    }
                    indicator: Image{
                      width: 20 / Style.monitorRatio
                      height: 20 / Style.monitorRatio
                      source: "qrc:/Resources/circle.png"
                    }
                }

            }

            ColumnLayout{
                id:secondPage
                visible: false
                anchors.top: logInText.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: signInBtn.top
                anchors.leftMargin: 50 / Style.monitorRatio
                anchors.rightMargin: 50 / Style.monitorRatio
                anchors.topMargin: 20 / Style.monitorRatio
                anchors.bottomMargin: 60 / Style.monitorRatio
                spacing: 0

                RowLayout{
                    spacing: 0
                    IconImage{
                        source: "qrc:/Resources/user.png"
                        Layout.preferredHeight: 23 / Style.monitorRatio
                        Layout.preferredWidth: 23 / Style.monitorRatio
                    }
                    Text{
                        id:username
                        text: "Username"
                        font.pixelSize: 20 / Style.monitorRatio
                        Layout.leftMargin: 5 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                }
                TextField {
                    Layout.preferredWidth: 340 / Style.monitorRatio
                    height: 40 / Style.monitorRatio
                    font.pointSize: 17 / Style.monitorRatio
                    leftPadding: 20 / Style.monitorRatio
                    color: foregroundColor
                    placeholderText:"Enter your Username"
                    placeholderTextColor: foregroundColor
                    background: Rectangle {
                        color: foregroundColor
                        radius: height / 2
                    }
                   onAccepted: {

                   }
                }
                RowLayout{
                    spacing: 0
                    IconImage{
                        source: "qrc:/Resources/user.png"
                        Layout.preferredHeight: 23 / Style.monitorRatio
                        Layout.preferredWidth: 23 / Style.monitorRatio
                    }
                    Text{
                        text: "Password"
                        font.pixelSize: 20 / Style.monitorRatio
                        Layout.leftMargin: 5 / Style.monitorRatio
                        color: Style.foregroundColor
                    }
                }
                TextField {
                    id:password
                    Layout.preferredWidth: 340 / Style.monitorRatio
                    height: 40 / Style.monitorRatio
                    font.pointSize: 17 / Style.monitorRatio
                    leftPadding: 20 / Style.monitorRatio
                    color: foregroundColor
                    placeholderText:"Enter your Password"
                    placeholderTextColor: foregroundColor
                    echoMode: TextField.Password
                    background: Rectangle {
                        color: foregroundColor
                        radius: height / 2
                    }
                   onAccepted: {

                   }
                }

            }

            Button{
                id:signInBtn
                height: 40 / Style.monitorRatio
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 50 / Style.monitorRatio
                anchors.rightMargin: 50 / Style.monitorRatio
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 26 / Style.monitorRatio
                hoverEnabled: true
                background: Rectangle{
                    color: Style.foregroundColor
                    radius: width/(Style.monitorRatio * 2)
                }
                contentItem: Text{
                    text: "Sign in"
                    color: parent.hovered ? "#01AED6" : Style.backgroundColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {

                   if(!firstPage.visible && secondPage.visible)
                      loginPage.signIn(username.text, password.text)

                   if(firstPage.visible && !secondPage.visible){
                       firstPage.visible = false
                       secondPage.visible = true
                   }

                }
            }

        }
    }
}
