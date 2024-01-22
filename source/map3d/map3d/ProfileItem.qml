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


        Text{
            text:"Name"
            Layout.topMargin: 16 / Style.monitorRatio
            font.pixelSize: 13 / Style.monitorRatio
            color: Style.foregroundColor
        }

        Label{
            text: rootItem.model.name
            font.pixelSize: 20 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.topMargin: 5 / Style.monitorRatio
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
            font.pixelSize: 20 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.topMargin: 5 / Style.monitorRatio
            color: Style.foregroundColor
        }

        Button{
            id:logoutBtn
            Layout.preferredHeight: 30 / Style.monitorRatio
            Layout.preferredWidth: 270 / Style.monitorRatio
            Layout.topMargin: 10 / Style.monitorRatio
            Layout.fillWidth: true
            hoverEnabled: true

            background: Rectangle{
                color: logoutBtn.hovered ? "#01AED6" : "transparent"
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
