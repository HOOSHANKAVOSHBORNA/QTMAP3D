import QtQuick
import QtQuick.Layouts

Rectangle {
    id: rLocationManager

    color: "silver"

    property alias listModel: lvLocationManger.model
    property int locationColorIconWidth: 10
    property int locationColorIconMargin: 3


    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // search bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            border.color: 'grey'
            radius: 20
            color: "transparent"
            clip: true

            TextInput {
                anchors.fill: parent
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                verticalAlignment: Text.AlignVCenter

                onTextChanged: {
                    lvLocationManger.model.searchedName = text
                }
            }
        }

        // add place button
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            radius: 20

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    console.log(lvLocationManger.model.searchedName)
                }
            }

            Text {
                text: "Add Place"
                anchors.centerIn: parent
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            radius: 20

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    lvLocationManger.model.sort(0, Qt.AscendingOrder)
                }
            }

            Text {
                text: "Sort By Name"
                anchors.centerIn: parent
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

            // ---------- this listview model sets in mainwindow.cpp !!!

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
                        source: model.imageSource
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: locationColorIconMargin

                        Rectangle {
                            width: locationColorIconWidth
                            height: locationColorIconWidth
                            radius: width / 2
                            color: model.color
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
                            text: model.description
                        }

                        Text {
                            text: model.lon + ", " + model.lat
                        }

                        Text {
                            text: model.heading + ", " + model.pitch + ", " + model.range
                        }
                    }
                }
            }
        }
    }
}
