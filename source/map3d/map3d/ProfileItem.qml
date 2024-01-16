import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls

import "style"

Item {
    id:rootItem

    property var model
    readonly property color foregroundColor: Qt.rgba(Style.foregroundColor.r,
                                                     Style.foregroundColor.g,
                                                     Style.foregroundColor.b,
                                                     0.20)

    readonly property color backgroundColor: Qt.rgba(Style.backgroundColor.r,
                                                     Style.backgroundColor.g,
                                                     Style.backgroundColor.b,
                                                     0.30)

    ColumnLayout{
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10 / Style.monitorRatio

        Text{
            id:nameText
            font.pixelSize: 25 / Style.monitorRatio
            color: Style.foregroundColor
        }

        RowLayout{
            Layout.fillWidth: true
            IconImage{
                Layout.preferredWidth: 20 / Style.monitorRatio
                Layout.preferredHeight: 20 / Style.monitorRatio
                source:"qrc:/Resources/draw.png"
            }
            Text{
                text: "Edit Profile"
                font.pixelSize: 20 / Style.monitorRatio
                Layout.fillWidth: true
                color: Style.foregroundColor
            }
        }
        Text{
            text:"Name"
            font.pixelSize: 17 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id: name
            Layout.preferredWidth: 270 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.rightMargin: 15 / Style.monitorRatio
            height: 30 / Style.monitorRatio
            font.pointSize: 15 / Style.monitorRatio
            color: Style.foregroundColor
            background: Rectangle {
                color: foregroundColor
                radius: height / 2
            }
        }

        Text{
            text:"Username"
            font.pixelSize: 17 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id:username
            Layout.preferredWidth: 270 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.rightMargin: 15 / Style.monitorRatio
            height: 30 / Style.monitorRatio
            font.pointSize: 15 / Style.monitorRatio
            color: Style.foregroundColor
            background: Rectangle {
                color: foregroundColor
                radius: height / 2
            }
        }

        Text{
            text:"Password"
            font.pixelSize: 17 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id:password
            Layout.preferredWidth: 270 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.rightMargin: 15 / Style.monitorRatio
            height: 30 / Style.monitorRatio
            font.pointSize: 15 / Style.monitorRatio
            color: Style.foregroundColor
            placeholderText: "Current Password"
            placeholderTextColor : Style.foregroundColor
            background: Rectangle {
                color: foregroundColor
                radius: height / 2
            }
        }

        TextField {
            Layout.preferredWidth: 270 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.rightMargin: 15 / Style.monitorRatio
            height: 30 / Style.monitorRatio
            font.pointSize: 15 / Style.monitorRatio
            color: Style.foregroundColor
            placeholderText:  "New Password"
            placeholderTextColor : Style.foregroundColor
            background: Rectangle {
                color: foregroundColor
                radius: height / 2
            }
        }

        TextField {
            Layout.preferredWidth: 270 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.rightMargin: 15 / Style.monitorRatio
            height: 30 / Style.monitorRatio
            font.pointSize: 15 / Style.monitorRatio
            color: Style.foregroundColor
            placeholderText: "Confirm Password"
            placeholderTextColor : Style.foregroundColor
            background: Rectangle {
                color: foregroundColor
                radius: height / 2
            }
        }

        Button{
            Layout.preferredHeight: 30 / Style.monitorRatio
            Layout.preferredWidth: 270 / Style.monitorRatio
            Layout.rightMargin: 15 / Style.monitorRatio
            Layout.fillWidth: true

            background: Rectangle{

                color: Qt.rgba(Style.foregroundColor.r,
                               Style.foregroundColor.g,
                               Style.foregroundColor.b,
                               0.60)

                radius: height / 2

                Text{
                    anchors.centerIn: parent
                    text: "Save changes"
                    font.pixelSize: 15 / Style.monitorRatio
                    color: Style.foregroundColor
                    }
            }

           onClicked: {
               rootItem.model.setName(name.text)
               rootItem.model.setUsername(username.text)
               rootItem.model.setPassword(password.text)
               nameText.text = name.text
           }
        }

        Button{
            Layout.preferredHeight: 30 / Style.monitorRatio
            Layout.preferredWidth: 270 / Style.monitorRatio
            Layout.rightMargin: 15 / Style.monitorRatio
            Layout.fillWidth: true

            background: Rectangle{
                color: Style.backgroundColor
                radius: height / 2

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
