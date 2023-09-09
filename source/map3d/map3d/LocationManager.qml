import QtQuick
import QtQuick.Layouts

Rectangle {
    id: rLocationManager

    color: "teal"

    property alias listModel: lvLocationManger.model
    property int locationColorIconWidth: 10
    property int locationColorIconMargin: 3


    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            border.color: 'grey'
            radius: 20
            color: "transparent"
        }

        // add place button
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            radius: 20

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    console.log(lvLocationManger.model)
                }
            }
        }

        // seperate line
        Rectangle {
            Layout.fillWidth: true
            height: 3
            color: "blue"
        }

        ListView {
            id: lvLocationManger

            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 10
            clip: true

            // model set in main window

            delegate: Rectangle {
                width: lvLocationManger.width
                height: 200
                color: "orange"
                radius: 10

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 5
                    spacing: 0

                    Image {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        source: model.imgsrc
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: locationColorIconMargin

                        Rectangle {
                            width: locationColorIconWidth
                            height: locationColorIconWidth
                            radius: width / 2
                            color: "yellow"
                        }

                        Text {
                            Layout.fillWidth: true
                            text: model.name
                            font.bold: true
                            font.pixelSize: 20
                        }

                        Rectangle {
                            width: 20
                            height: 20
                            color: "black"
                        }

                        Rectangle {
                            width: 20
                            height: 20
                            color: "black"
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.leftMargin: locationColorIconWidth + locationColorIconMargin
                        spacing: 0

                        Text {
                            text: model.where
                        }

                        Text {
                            text: model.lat + ", " + model.lang
                        }
                    }
                }
            }
        }
    }
}
