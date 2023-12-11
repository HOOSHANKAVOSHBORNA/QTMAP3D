import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import "style"

Item {
    id: rootItem

    anchors.fill: parent

    property int lblWidth: 85
    property int valHeight: 30

    readonly property color fg20: Qt.rgba(Style.foregroundColor.r,
                                          Style.foregroundColor.g,
                                          Style.foregroundColor.b, 0.20)

    // --------------------------------------------------------- Name
    RowLayout {
        id: namerowl

        Layout.fillWidth: true
        spacing: 0

        Text {
            Layout.preferredWidth: lblWidth / Style.monitorRatio
            text: "Name"
            font.pixelSize: 17 / Style.monitorRatio
            color: Style.foregroundColor
        }

        TextField {
            id: nametextf

            Layout.preferredWidth: 200 / Style.monitorRatio
            Layout.minimumWidth: 100 / Style.monitorRatio
            Layout.fillWidth: true
            Layout.rightMargin: 15 / Style.monitorRatio
            Layout.leftMargin: 15 / Style.monitorRatio

            height: valHeight
            text: modelPropertyInterface.name ?? "notInitial"
            font.pointSize: 10 / Style.monitorRatio
            color: "black"

            background: Rectangle {
                color: fg20
                radius: height / 2
            }

            onAccepted: {
                modelPropertyInterface.name = nametextf.text
            }
        }
    }

    Text {
        anchors.centerIn: parent
        text: 'Model Property Item'
    }
}
