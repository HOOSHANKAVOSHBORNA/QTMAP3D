import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

Item {
    clip:true
    id:rootItem

    ScrollView {
        anchors.fill: parent
        contentWidth: lay.width + 30
        contentHeight: lay.height + 15

        ColumnLayout {
            id:lay
            spacing: 10

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            width: rootItem.width - 30


            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: width * (img.sourceSize.height / img.sourceSize.width)
                Image {
                    id: img
                    anchors.fill: parent
                    source: "qrc:///Resources/airplan.jpg"
                    fillMode: Image.Stretch
                }
            }


            Rectangle {
                Layout.preferredWidth: rootItem.width - 30
                Layout.preferredHeight: 2
                color: "white"

            }

            GridLayout {
                rows: 6
                columns: 2
                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight

                Label {
                    text: "Latitude: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Label {
                    text: "Longitude: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Label {
                    text: "Altitude: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Label {
                    text: "Range: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Label {
                    text: "Heading: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }

                Label {
                    text: "Time: "
                    color: "white"
                    Layout.preferredWidth: implicitWidth
                }
                Label {
                    text: "0.0"
                    color: "white"
                    Layout.fillWidth: true
                    horizontalAlignment: Qt.AlignRight
                }
            }

        }
    }
}
