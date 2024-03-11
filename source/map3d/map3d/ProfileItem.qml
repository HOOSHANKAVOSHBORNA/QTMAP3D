import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls
import "style"
import "Components"

Item {
    id: rootItem

    readonly property color foregroundColorRect: Qt.rgba(
                                                     Style.foregroundColor.r,
                                                     Style.foregroundColor.g,
                                                     Style.foregroundColor.b,
                                                     0.20)

    readonly property color foregroundColorText: Qt.rgba(
                                                     Style.foregroundColor.r,
                                                     Style.foregroundColor.g,
                                                     Style.foregroundColor.b,
                                                     0.75)

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 15 / Style.monitorRatio
        anchors.leftMargin: 15 / Style.monitorRatio
        spacing: 0

        Text {
            text: "Name: "
            Layout.topMargin: 16 / Style.monitorRatio
            font.pixelSize: Style.regularFontSize
            color: Style.foregroundColor
        }

        Label {
            text: userManager.name
            font.pixelSize: Style.regularFontSize
            Layout.fillWidth: true
            Layout.topMargin: 5 / Style.monitorRatio
            Layout.leftMargin: 15 / Style.monitorRatio
            color: Style.foregroundColor
        }

        Text {
            text: "Username: "
            Layout.topMargin: 10 / Style.monitorRatio
            font.pixelSize: Style.regularFontSize
            color: Style.foregroundColor
        }

        Label {
            text: userManager.userName
            font.pixelSize: Style.regularFontSize
            Layout.fillWidth: true
            Layout.topMargin: 5 / Style.monitorRatio
            Layout.leftMargin: 15 / Style.monitorRatio
            color: Style.foregroundColor
        }

        Text {
            text: "Role: "
            Layout.topMargin: 10 / Style.monitorRatio
            font.pixelSize: Style.regularFontSize
            color: Style.foregroundColor
        }

        Label {
            text: userManager.roleName
            font.pixelSize: Style.regularFontSize
            Layout.fillWidth: true
            Layout.topMargin: 5 / Style.monitorRatio
            Layout.leftMargin: 15 / Style.monitorRatio
            color: Style.foregroundColor
        }

        CustomButton {
            id: logoutBtn
            Layout.preferredHeight: 30 / Style.monitorRatio
            Layout.preferredWidth: 270 / Style.monitorRatio
            Layout.topMargin: 10 / Style.monitorRatio
            Layout.fillWidth: true

            hoverEnabled: true
            iconImageSource: "qrc:/Resources/circular-logout.png"
            iconImageVisible: true
            buttonText: "Log out"
            buttonColor: logoutBtn.hovered ? "#01AED6" : "transparent"
            buttonTextColor: Style.foregroundColor
            buttonBorder.color: Style.foregroundColor

            onClicked: {
                applicationCpp.userManager.signedOut()
            }
        }
    }
}
