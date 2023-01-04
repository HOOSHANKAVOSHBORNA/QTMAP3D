import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import Crystal 1.0

Item {
    id: rootItem
    x: 0
    width: 300
    clip: true

    property StationInfoModel model

    Rectangle{
        anchors.fill: parent
        color: "#404040"
        opacity: 0.8
        radius: 10
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 0
        Item {
            Layout.fillWidth: true
            Layout.minimumHeight: 60

            Text {
                id: airplane
                anchors.centerIn: parent
                text: "Station"
                color:"yellow"
                font.pointSize: 20

            }

        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollView {

                clip: true
                anchors.fill: parent
//                contentWidth: lay.width + 30
//                contentHeight: lay.height + 15
                ListView {
                    id: listview
                    model: rootItem.model
                    delegate: ColumnLayout {
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
                            rows: 9
                            columns: 2
                            Layout.fillWidth: true
                            Layout.preferredHeight: implicitHeight

                            Label {
                                text: "Name: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: nameLabel
                                text: Name
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                                Binding {
                                    target: airplane
                                    property: "text"
                                    value: tnLabel.text
                                }
                            }


                            Label {
                                text: "Type: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: typeLabel
                                text: Type
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                            Label {
                                text: "Primary/Secondry: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: primLabel
                                text: PrimSec
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                            Rectangle {
                                Layout.preferredWidth: rootItem.width - 30
                                Layout.preferredHeight: 2
                                color: "white"

                            }


                            GridLayout {
                                rows: 3
                                columns: 2
                                Layout.fillWidth: true
                                Layout.preferredHeight: implicitHeight

                                Label {
                                    text: "Latitude: "
                                    color: "white"
                                    Layout.preferredWidth: implicitWidth
                                }
                                Label {
                                    id: latLabel
                                    text: Latitude
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
                                    id: longLabel
                                    text: Longitude
                                    color: "white"
                                    Layout.fillWidth: true
                                    horizontalAlignment: Qt.AlignRight
                                }
                                Label {
                                    text: "Number: "
                                    color: "white"
                                    Layout.preferredWidth: implicitWidth
                                }
                                Label {
                                    id: numberLabel
                                    text: Numberr
                                    color: "white"
                                    Layout.fillWidth: true
                                    horizontalAlignment: Qt.AlignRight
                                }
                                Rectangle {
                                    Layout.preferredWidth: rootItem.width - 30
                                    Layout.preferredHeight: 2
                                    color: "white"

                                }

                            }
                        }
                    }
                }
            }


        }
        Item {
            Layout.minimumHeight: 5
        }

        StationInfoButtonGroup {
            Layout.fillWidth: true
            Layout.minimumHeight: 60

            onGotoButtonClicked: function() {
                rootItem.model.gotoButtonClicked();
            }
            onRouteButtonClicked: function(check) {
                rootItem.model.routeButtonClicked(check);
            }
            onTrackButtonClicked: function(check) {
                rootItem.model.trackButtonClicked(check);
            }
            onMoreButtonClicked: function() {
                rootItem.model.moreButtonClicked();
            }
        }
    }
}
