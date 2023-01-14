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
    property var boxColor: "#363739"
    property var valueColor: "#363739"
    property var keyTextColor: "#9b9ca0"
    property var spacee: 5
    Label {
        id: forWidth
        visible: false
        text: " 1401/01/01 00:00 "
    }

    Rectangle{
        anchors.fill: parent
        color: "#2F2E2C"
//        opacity: 0.8
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
                ScrollBar.vertical.interactive: false

                clip: true
                anchors.fill: parent
//                contentWidth: lay.width + 30
//                contentHeight: lay.height + 15
                ListView {
                    id: listview
                    model: rootItem.model
                    delegate: ColumnLayout {
                        id:lay
                        spacing: 1

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        width: rootItem.width


                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 200
                            Image {
                                id: img
                                anchors.fill: parent
                                source: "qrc:///Resources/airplane1.jpg"
                                fillMode: Image.Stretch
                            }
                        }


//                        Rectangle {
//                            Layout.preferredWidth: rootItem.width - 30
//                            Layout.preferredHeight: 2
//                            color: "white"

//                        }
                        RowLayout {
                            spacing: 2
                            Layout.preferredWidth: rootItem.width
                            Rectangle {
                                Layout.preferredWidth: img2.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    id: img2
                                    verticalAlignment: Image.AlignTop
                                    source: "qrc:/resources/aircraftinfo.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24

                                }
                                color: "#474747"
                            }
                            GridLayout {
                                rows: 9
                                columns: 2
                                Layout.fillWidth: true
                                Layout.preferredHeight: implicitHeight
                                columnSpacing: 0
                                rowSpacing: 0

                                Label {
                                    text: "TN "
                                    color: keyTextColor
//                                    Layout.preferredWidth: 135
                                    Layout.fillWidth: true
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: tnLabel
                                    text: TN
                                    color: "white"
//                                    Layout.preferredWidth: 135
                                    Layout.fillWidth: true
                                    horizontalAlignment: Qt.AlignLeft
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee

                                    Binding {
                                        target: airplane
                                        property: "text"
                                        value: tnLabel.text
                                    }
                                    background: Rectangle {
                                        color: valueColor

                                    }
                                }


                                Label {
                                    text: "IFFCode "
                                    color: keyTextColor
//                                    Layout.preferredWidth: 135
                                    Layout.fillWidth: true
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: iffCodeLabel
                                    text: IFFCode
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    horizontalAlignment: Qt.AlignLeft
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }

                                Label {
                                    text: "CallSign "
                                    color: keyTextColor
    //                                Layout.preferredWidth: implicitWidth
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: callSignLabel
                                    text: CallSign
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    horizontalAlignment: Qt.AlignLeft
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }


                                Label {
                                    text: "Type "
                                    color: keyTextColor
    //                                Layout.preferredWidth: implicitWidth
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: typeLabel
                                    text: Type
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    horizontalAlignment: Qt.AlignLeft
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }


                                Label {
                                    text: "Master "
                                    color: keyTextColor
    //                                Layout.preferredWidth: implicitWidth
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: masterLabel
                                    text: Master
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    horizontalAlignment: Qt.AlignLeft
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }

                                Label {
                                    text: "Identification "
                                    color: keyTextColor
    //                                Layout.preferredWidth: implicitWidth
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: identificationLabel
                                    text: Identification
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    horizontalAlignment: Qt.AlignLeft
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }

                                Label {
                                    id: idenLabel
                                    text: "Identif. Method "
                                    color: keyTextColor
//                                    Layout.preferredWidth: implicitWidth
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: 110
                                    Layout.minimumWidth: 110
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: identificationMethodLabel
                                    text: IdentificationMethod
                                    color: "white"
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: 160
                                    Layout.minimumWidth: 160
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: idenLabel.implicitHeight + spacee
                                    horizontalAlignment: Qt.AlignLeft
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }

                                Label {
                                    text: "Time "
                                    color: keyTextColor
    //                                Layout.preferredWidth: implicitWidth
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: timeLabel
                                    text: Time
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
    //                                Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    clip: true
                                    Layout.preferredHeight: implicitHeight + spacee
                                    horizontalAlignment: Qt.AlignLeft
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }

                                Label {
                                    text: "Pos "
                                    color: keyTextColor
    //                                Layout.preferredWidth: implicitWidth
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: posLabel
                                    text: Pos
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    horizontalAlignment: Qt.AlignLeft
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }

                            }
                        }

//                        Rectangle {
//                            Layout.preferredWidth: rootItem.width
//                            Layout.preferredHeight: 2
//                            color: "white"

//                        }

                        Label {
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: 2
                            verticalAlignment: Label.AlignVCenter
                            color: "white"
                            text: ""
                            horizontalAlignment: Qt.AlignHCenter
                            background: Rectangle {
                                color: "#679BE1"
                            }
                        }
                        RowLayout {
                            spacing: 2
                            Layout.preferredWidth: rootItem.width
                            Rectangle {
                                Layout.preferredWidth: img1.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    id: img1
                                    verticalAlignment: Image.AlignTop
                                    horizontalAlignment: Image.AlignHCenter
                                    source: "qrc:/resources/latlong.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24

                                }
                                color: "#474747"
                            }
                            GridLayout {
                                rows: 5
                                columns: 2
                                Layout.fillWidth: true
                                Layout.preferredWidth: implicitWidth
                                Layout.preferredHeight: implicitHeight
                                rowSpacing: 0
                                columnSpacing: 0

                                Label {
                                    text: "Latitude "
                                    color: keyTextColor
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: 110
                                    Layout.minimumWidth: 110
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: latitudeLabel
                                    text: Latitude
                                    color: "white"
//                                    Layout.preferredWidth: tnLabel.implicitWidth
                                    Layout.fillWidth: true
                                    Layout.preferredWidth: 160
                                    Layout.minimumWidth: 160
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: idenLabel.implicitHeight + spacee
                                    horizontalAlignment: Qt.AlignLeft
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }

                                Label {
                                    text: "Longitude "
                                    color: keyTextColor
//                                    Layout.preferredWidth: 80
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: longitudeLabel
                                    text: Longitude
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: idenLabel.implicitHeight + spacee
                                    horizontalAlignment: Qt.AlignLeft
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }


                                Label {
                                    text: "Altitude "
                                    color: keyTextColor
    //                                Layout.preferredWidth: implicitWidth
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: altitudeLabel
                                    text: Altitude
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: idenLabel.implicitHeight + spacee
                                    horizontalAlignment: Qt.AlignLeft
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }



                                Label {
                                    text: "Heading "
                                    color: keyTextColor
    //                                Layout.preferredWidth: implicitWidth
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: headingLabel
                                    text: Heading
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: idenLabel.implicitHeight + spacee
                                    horizontalAlignment: Qt.AlignLeft
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }


                                Label {
                                    text: "Speed "
                                    color: keyTextColor
    //                                Layout.preferredWidth: implicitWidth
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: implicitHeight + spacee
                                    background: Rectangle {
                                        color: boxColor
                                    }
                                }
                                Label {
                                    id: speedLabel
                                    text: Speed
                                    color: "white"
                                    Layout.fillWidth: true
//                                    Layout.preferredWidth: 135
                                    Layout.leftMargin: 5
                                    leftInset: -5
    //                                clip: true
                                    verticalAlignment: Label.AlignVCenter
                                    Layout.preferredHeight: idenLabel.implicitHeight + spacee
                                    horizontalAlignment: Qt.AlignLeft
                                    background: Rectangle {
                                        color: valueColor
                                    }
                                }

                            }
                        }

//                        Rectangle {
//                            Layout.preferredWidth: rootItem.width - 30
//                            Layout.preferredHeight: 2
//                            color: "white"

//                        }

                        Label {
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: implicitHeight + 10
                            verticalAlignment: Label.AlignVCenter
                            color: "white"
                            text: "Detection Systems"
                            horizontalAlignment: Qt.AlignHCenter
                            background: Rectangle {
                                color: "#679BE1"
                            }
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

//                        Rectangle {
//                            Layout.preferredWidth: rootItem.width - 30
//                            Layout.preferredHeight: 2
//                            color: "white"

//                        }

                        Label {
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: implicitHeight + 10
                            verticalAlignment: Label.AlignVCenter
                            color: "white"
                            text: "Sends"
                            horizontalAlignment: Qt.AlignHCenter
                            background: Rectangle {
                                color: "#679BE1"
                            }
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
