import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls

import Crystal 1.0
import "style"

Item {
    id:rootItem

    property var model

    readonly property color foregroundColorRect: Qt.rgba(Style.foregroundColor.r,
                                                     Style.foregroundColor.g,
                                                     Style.foregroundColor.b,
                                                     0.20)

    readonly property color foregroundColorText: Qt.rgba(Style.foregroundColor.r,
                                                     Style.foregroundColor.g,
                                                     Style.foregroundColor.b,
                                                     0.75)

    ColumnLayout{
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 15 / Style.monitorRatio
        anchors.leftMargin: 15 / Style.monitorRatio
        spacing: 0

        Text{
            id:nameText
            text: rootItem.model.name
            font.pixelSize: 20 / Style.monitorRatio
            color: Style.foregroundColor
            Layout.topMargin: 46 / Style.monitorRatio
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.topMargin: 36 / Style.monitorRatio
            IconImage{
                Layout.preferredWidth: 20 / Style.monitorRatio
                Layout.preferredHeight: 20 / Style.monitorRatio
                source:"qrc:/Resources/draw.png"
            }
            Text{
                text: "Edit Profile"
                font.pixelSize: 15 / Style.monitorRatio
                Layout.fillWidth: true
                color: Style.foregroundColor
            }
        }
        Text{
            text:"Name"
            Layout.topMargin: 16 / Style.monitorRatio
            font.pixelSize: 13 / Style.monitorRatio
            color: Style.foregroundColor
        }

//        TextField {
//            id: name
//            Layout.preferredWidth: 270 / Style.monitorRatio
//            Layout.topMargin: 5 / Style.monitorRatio
//            Layout.fillWidth: true
//            text: rootItem.model.name
//            height: 30 / Style.monitorRatio
//            font.pixelSize: 15 / Style.monitorRatio
//            color: foregroundColorText
//            background: Rectangle {
//                color: foregroundColorRect
//                radius: 15 / Style.monitorRatio
//            }
//        }
        Label{
            text: rootItem.model.name
            font.pixelSize: 15 / Style.monitorRatio
            Layout.fillWidth: true
            color: Style.foregroundColor
        }

        Text{
            text:"Username"
            Layout.topMargin: 10 / Style.monitorRatio
            font.pixelSize: 13 / Style.monitorRatio
            color: Style.foregroundColor
        }

        Label{
            text: rootItem.model.username
            font.pixelSize: 15 / Style.monitorRatio
            Layout.fillWidth: true
            color: Style.foregroundColor
        }

//        TextField {
//            id:username
//            Layout.preferredWidth: 270 / Style.monitorRatio
//            Layout.topMargin: 5 / Style.monitorRatio
//            Layout.fillWidth: true
//            text: rootItem.model.username
//            height: 30 / Style.monitorRatio
//            font.pointSize: 15 / Style.monitorRatio
//            color: foregroundColorText
//            background: Rectangle {
//                color: foregroundColorRect
//                radius: 15 / Style.monitorRatio
//            }
//        }

//        Text{
//            text:"Password"
//            font.pixelSize: 13 / Style.monitorRatio
//            Layout.topMargin: 10 / Style.monitorRatio
//            color: Style.foregroundColor
//        }

//        TextField {
//            id:password
//            echoMode: TextField.Password
//            Layout.preferredWidth: 270 / Style.monitorRatio
//            Layout.topMargin: 5 / Style.monitorRatio
//            Layout.fillWidth: true
//            height: 30 / Style.monitorRatio
//            font.pointSize: 15 / Style.monitorRatio
//            color: foregroundColorText
//            placeholderText: "Current Password"
//            placeholderTextColor :foregroundColorText
//            background: Rectangle {
//                color: foregroundColorRect
//                radius: 15 / Style.monitorRatio
//            }
//        }

//        TextField {
//            id:newPassword
//            echoMode: TextField.Password
//            Layout.preferredWidth: 270 / Style.monitorRatio
//            Layout.fillWidth: true
//            Layout.topMargin: 10 / Style.monitorRatio
//            height: 30 / Style.monitorRatio
//            font.pointSize: 15 / Style.monitorRatio
//            color: foregroundColorText
//            placeholderText:  "New Password"
//            placeholderTextColor:foregroundColorText
//            background: Rectangle {
//                color: foregroundColorRect
//                radius: 15 / Style.monitorRatio
//            }
//        }

//        TextField {
//            id:confirmPassword
//            echoMode: TextField.Password
//            Layout.preferredWidth: 270 / Style.monitorRatio
//            Layout.fillWidth: true
//            Layout.topMargin: 10 / Style.monitorRatio
//            height: 30 / Style.monitorRatio
//            font.pointSize: 15 / Style.monitorRatio
//            color: foregroundColorText
//            placeholderText: "Confirm Password"
//            placeholderTextColor:foregroundColorText
//            background: Rectangle {
//                color: foregroundColorRect
//                radius: 15 / Style.monitorRatio
//            }
//        }

//        Text{
//            id:warningText
//            text: "Enter Current password as well"
//            font.family: Style.fontFamily
//            font.pixelSize: 12 / Style.monitorRatio
//            color: "transparent"
//        }

//        Button{
//            Layout.preferredHeight: 30 / Style.monitorRatio
//            Layout.preferredWidth: 270 / Style.monitorRatio
//            Layout.topMargin: 20 / Style.monitorRatio
//            Layout.fillWidth: true

//            background: Rectangle{

//                color: Qt.rgba(Style.foregroundColor.r,
//                               Style.foregroundColor.g,
//                               Style.foregroundColor.b,
//                               0.80)

//                radius: 15 / Style.monitorRatio

//                Text{
//                    anchors.centerIn: parent
//                    text: "Save changes"
//                    font.pixelSize: 15 / Style.monitorRatio
//                    color: Style.backgroundColor
//                    }
//            }

//           onClicked: {
//            var  userValidate = rootItem.model.validateUserChanges(name.text,username.text,password.text)
//            var passwordValidate = rootItem.model.validatePasswordChanges(password.text,newPassword.text,confirmPassword.text)

//               password.text = ""
//               newPassword.text = ""
//               confirmPassword.text = ""
//               warningText.color = "red"

//               console.log(rootItem.model)
//           }
//        }

        Button{
            Layout.preferredHeight: 30 / Style.monitorRatio
            Layout.preferredWidth: 270 / Style.monitorRatio
            Layout.topMargin: 5 / Style.monitorRatio
            Layout.fillWidth: true

            background: Rectangle{
                color: "transparent"
                border.width: 2 / Style.monitorRatio
                border.color: Style.foregroundColor
                radius: 15 / Style.monitorRatio

                IconImage{
                    width: 20 / Style.monitorRatio
                    height: 20 / Style.monitorRatio
                    source: "qrc:/Resources/logout.png"
                    anchors.right: logout.left
                    anchors.bottom: logout.bottom
                }

                Text{
                    id:logout
                    anchors.centerIn: parent
                    text: "Log out"
                    font.pixelSize: 15 / Style.monitorRatio
                    color: Style.foregroundColor
                }
            }

            onClicked: {

            }

        }

    }
}
