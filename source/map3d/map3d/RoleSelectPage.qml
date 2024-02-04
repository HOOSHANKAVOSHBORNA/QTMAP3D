import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

ColumnLayout {
    property alias connectionStatus: connectionStatus
    property alias signInBtn: signInBtn
    property alias usernametxt: usernametxt

    spacing: 0

    RowLayout {
        id: header
        Layout.fillWidth: true
        Layout.topMargin: 60 / Style.monitorRatio

        Text {
            id: titleText
            text: "Log in"
            font.pixelSize: 35 / Style.monitorRatio
            color: Style.foregroundColor
            Layout.fillWidth: true
        }

        Button {
            id: connectionStatus
            background: Image {
                source: "qrc:/Resources/unplugged.png"
            }
            Layout.preferredHeight: 39 / Style.monitorRatio
            Layout.preferredWidth: 39 / Style.monitorRatio

            onClicked: {

            }
        }
    }

    RowLayout {
        spacing: 0
        Layout.topMargin: 50 / Style.monitorRatio
        IconImage {
            source: "qrc:/Resources/user.png"
            Layout.preferredHeight: 25 / Style.monitorRatio
            Layout.preferredWidth: 25 / Style.monitorRatio
        }

        Text {
            id:usernametxt
            font.pixelSize: 23 / Style.monitorRatio
            color: Style.foregroundColor
        }
    }

    Text {
        text: "Select role"
        Layout.topMargin: 30 / Style.monitorRatio
        font.pixelSize: 22 / Style.monitorRatio
        font.bold: true
        color: Style.foregroundColor
    }

    RadioButton {
        id: administrator
        text: "Administrator"
        Layout.topMargin: 12 / Style.monitorRatio
        Layout.preferredHeight: 20 / Style.monitorRatio
        Layout.fillWidth: true
        opacity: checked ? 1 : 0.75
        contentItem: Text {
            text: parent.text
            font.pixelSize: 20 / Style.monitorRatio
            color: Style.foregroundColor
            leftPadding: 20 / Style.monitorRatio
            verticalAlignment: Text.AlignVCenter
        }
        indicator: Image {
            width: 20 / Style.monitorRatio
            height: 20 / Style.monitorRatio
            source: administrator.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
        }
    }

    RadioButton {
        id: admin
        text: "Admin"
        Layout.topMargin: 10 / Style.monitorRatio
        Layout.preferredHeight: 20 / Style.monitorRatio
        Layout.fillWidth: true
        opacity: checked ? 1 : 0.75
        contentItem: Text {
            text: parent.text
            font.pixelSize: 20 / Style.monitorRatio
            color: Style.foregroundColor
            leftPadding: 20 / Style.monitorRatio
            verticalAlignment: Text.AlignVCenter
        }
        indicator: Image {
            width: 20 / Style.monitorRatio
            height: 20 / Style.monitorRatio
            source: admin.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
        }
    }

    RadioButton {
        id: reviewer
        text: "Reviewer"
        Layout.topMargin: 10 / Style.monitorRatio
        Layout.preferredHeight: 20 / Style.monitorRatio
        Layout.fillWidth: true
        opacity: checked ? 1 : 0.75
        contentItem: Text {
            text: parent.text
            font.pixelSize: 20 / Style.monitorRatio
            color: Style.foregroundColor
            leftPadding: 20 / Style.monitorRatio
            verticalAlignment: Text.AlignVCenter
        }
        indicator: Image {
            width: 20 / Style.monitorRatio
            height: 20 / Style.monitorRatio
            source: reviewer.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
        }
    }

    Button {
        id: signInBtn
        Layout.preferredHeight: 40 / Style.monitorRatio
        Layout.fillWidth: true
        Layout.topMargin: 23 / Style.monitorRatio
        hoverEnabled: true
        background: Rectangle {
            color: Style.foregroundColor
            radius: width / (Style.monitorRatio * 2)
        }
        contentItem: Text {
            text: "Sign in"
            color: parent.hovered ? "#01AED6" : Style.backgroundColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {

        }
    }
}
