import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import Crystal 1.0

Item {
    id: rootItem
    x: 0
    width: 300
    clip: true

    property AircraftInfoModel model

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
                text: "Airplane"
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
                                text: "TN: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: tnLabel
                                text: TN
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
                                text: "IFFCode: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: iffCodeLabel
                                text: IFFCode
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                            Label {
                                text: "CallSign: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: callSignLabel
                                text: CallSign
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
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
                                text: "Master: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: masterLabel
                                text: Master
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                            Label {
                                text: "Identification: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: identificationLabel
                                text: Identification
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                            Label {
                                text: "Identification Method: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: identificationMethodLabel
                                text: IdentificationMethod
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
                                id: timeLabel
                                text: Time
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                            Label {
                                text: "Pos: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: posLabel
                                text: Pos
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                        }

                        Rectangle {
                            Layout.preferredWidth: rootItem.width - 30
                            Layout.preferredHeight: 2
                            color: "white"

                        }

                        GridLayout {
                            rows: 5
                            columns: 2
                            Layout.fillWidth: true
                            Layout.preferredHeight: implicitHeight

                            Label {
                                text: "Latitude: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: latitudeLabel
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
                                id: longitudeLabel
                                text: Longitude
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
                                id: altitudeLabel
                                text: Altitude
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
                                id: headingLabel
                                text: Heading
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }


                            Label {
                                text: "Speed: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: speedLabel
                                text: Speed
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                        }

                        Rectangle {
                            Layout.preferredWidth: rootItem.width - 30
                            Layout.preferredHeight: 2
                            color: "white"

                        }

                        Label {
                            Layout.preferredWidth: rootItem.width - 30
                            Layout.preferredHeight: implicitHeight
                            color: "white"
                            text: "Detection Systems"
                            horizontalAlignment: Qt.AlignHCenter
                        }

                        Repeater {
                            model: DetectionSystems
                            Label {
                                Layout.preferredWidth: rootItem.width - 30
                                Layout.preferredHeight: implicitHeight
                                color: "white"
                                text: DetectionSystems[index]
                                horizontalAlignment: Qt.AlignLeft
                            }

                        }

                        Rectangle {
                            Layout.preferredWidth: rootItem.width - 30
                            Layout.preferredHeight: 2
                            color: "white"

                        }

                        Label {
                            Layout.preferredWidth: rootItem.width - 30
                            Layout.preferredHeight: implicitHeight
                            color: "white"
                            text: "Sends"
                            horizontalAlignment: Qt.AlignHCenter
                        }

                        Repeater {
                            model: Sends
                            Label {
                                Layout.preferredWidth: rootItem.width - 30
                                Layout.preferredHeight: implicitHeight
                                color: "white"
                                text: Sends[index]
                                horizontalAlignment: Qt.AlignLeft
                            }

                        }
                    }
                }
            }


        }
        Item {
            Layout.minimumHeight: 5
        }

        AircraftInfoButtonGroup {
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
