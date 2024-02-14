import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "style"

ColumnLayout {
    property var roleSelectionCpp: undefined
    property alias connectionStatus: connectionStatus
    property alias signInBtn: signInBtn
    property alias backBtn: backBtn
    property alias usernameTxt: usernameTxt.text

    spacing: 0

    Button {
        id: backBtn
        Layout.preferredHeight: 40 / Style.monitorRatio
        Layout.preferredWidth: 40 / Style.monitorRatio
        Layout.topMargin: 30 / Style.monitorRatio
        background: Image {
            source: "qrc:/Resources/back.png"
        }
    }

    RowLayout {
        id: header
        Layout.fillWidth: true
        Layout.topMargin: 30 / Style.monitorRatio

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
            id: usernameTxt
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

    ListView {
        id: roleSelectionView
        model: roleSelectionModel
        Layout.topMargin: 12 / Style.monitorRatio
        Layout.preferredHeight: 90 / Style.monitorRatio
        Layout.fillWidth: true
        spacing: 10 / Style.monitorRatio
        interactive: false
        delegate: RadioButton {
            id: radioButton
            text: model.display
            height: 20 / Style.monitorRatio
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
                source: radioButton.checked ? "qrc:/Resources/radioButtonCircle.png" : "qrc:/Resources/radioButtonCircleEmpty.png"
            }

            onCheckedChanged: {
                if (radioButton.checked)
                    roleSelectionModel.getSelectedRoleIndex(index)
            }
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
