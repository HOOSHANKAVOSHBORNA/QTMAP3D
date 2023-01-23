import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.0
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
                background: Rectangle {
                    color: boxColor
                }

                ScrollBar.vertical.interactive: false
                clip: true
                anchors.fill: parent
                ListView {
                    id: listview
                    model: rootItem.model
                    delegate: ColumnLayout {
                        id:lay
                        spacing: 1

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        width: rootItem.width

                        Binding {
                            target: airplane
                            property: "text"
                            value: TN
                        }
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
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/resources/aircraftinfo.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24

                                }
                                color: "#474747"
                            }
                            GridLayout {
                                id: wid
                                rows: 9
                                columns: 1
                                Layout.fillWidth: true
                                Layout.preferredWidth: implicitWidth
                                Layout.preferredHeight: implicitHeight
                                columnSpacing: 0
                                rowSpacing: 0
                                Repeater {
                                    model: MainInfoHeaders
                                    Label {
                                        text: MainInfoHeaders[index]
                                        color: keyTextColor
                                        Layout.fillWidth: true
                                        verticalAlignment: Label.AlignVCenter
                                        Layout.leftMargin: 5
                                        leftInset: -5
                                        Layout.preferredHeight: implicitHeight + spacee
                                        background: Rectangle {
                                            color: boxColor
                                        }
                                    }

                                }
                            }
                            GridLayout {
                                rows: 9
                                columns: 1
//                                Layout.fillWidth: true
                                Layout.preferredHeight: implicitHeight
                                columnSpacing: 0
                                rowSpacing: 0
                                Repeater {
                                    model: MainInfo
                                    Label {
                                        id: txt
                                        text: MainInfo[index]
                                        color: "white"
                                        Layout.fillWidth: true
                                        Layout.leftMargin: 5
                                        leftInset: -5
                                        verticalAlignment: Label.AlignVCenter
                                        Layout.preferredHeight: implicitHeight + spacee
                                        background: Rectangle {
                                            color: boxColor
                                        }
                                    }
                                }
                            }
                        }
                        Label {
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: 1
                            verticalAlignment: Label.AlignVCenter
                            color: "white"
                            text: ""
                            horizontalAlignment: Qt.AlignHCenter
                            background: Rectangle {
                                color: "#909090"
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
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/resources/location.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24

                                }
                                color: "#474747"
                            }
                            GridLayout {
                                rows: 5
                                columns: 2
//                                Layout.fillWidth: true
//                                Layout.preferredWidth: implicitWidth
                                Layout.preferredHeight: implicitHeight
                                rowSpacing: 0
                                columnSpacing: 0
                                GridLayout {
                                    rows: 5
                                    columns: 1
                                    Layout.preferredHeight: implicitHeight
                                    Layout.preferredWidth: wid.implicitWidth - 55
                                    columnSpacing: 0
                                    rowSpacing: 0
                                    Repeater {
                                        model: LocationInfoHeaders
                                        Label {
                                            text: LocationInfoHeaders[index]
                                            color: keyTextColor
                                            Layout.fillWidth: true
                                            verticalAlignment: Label.AlignVCenter
                                            Layout.leftMargin: 5
                                            leftInset: -5
                                            Layout.preferredHeight: implicitHeight + spacee
                                            background: Rectangle {
                                                color: boxColor
                                            }
                                        }

                                    }
                                }
                                GridLayout {
                                    rows: 4
                                    columns: 1
//                                    Layout.fillWidth: true
                                    Layout.preferredHeight: implicitHeight
                                    columnSpacing: 0
                                    rowSpacing: 0
                                    Repeater {
                                        model: LocationInfo
                                        Label {
                                            text: LocationInfo[index]
                                            color: "white"
                                            Layout.fillWidth: true
                                            Layout.leftMargin: 5
                                            leftInset: -5
                                            verticalAlignment: Label.AlignVCenter
                                            Layout.preferredHeight: implicitHeight + spacee
                                            background: Rectangle {
                                                color: boxColor
                                            }
                                        }
                                    }
                                }
                            }
                        }


                        Rectangle {
                            id: firstButton
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: implicitHeight + 25
                            property var checked: true

                            MouseArea {
                                hoverEnabled: true
                                anchors.fill: parent
                                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                onClicked: if (firstButton.checked) {
                                               detectionLayout.Layout.preferredHeight = detectionLayout.implicitHeight
                                               imgd.rotation = 180
                                               firstButton.checked = false
                                           }
                                           else {
                                               detectionLayout.Layout.preferredHeight = 0
                                               imgd.rotation = -90
                                               firstButton.checked = true
                                           }
                            }

                            color: "#1a75ff"

                            Text {
                                text: "Detection Systems"
                                color: "white"
                                anchors.centerIn: parent
                                anchors.left: parent.left
                            }
                            Image {
                                id: imgd
                                source: "qrc:/Resources/chevron.png"
                                width: 16
                                height: 16
                                rotation: -90
                                ColorOverlay {
                                    source: imgd
                                    anchors.fill: imgd
                                    color: "white"
                                }
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                            }

                        }



                        RowLayout {
                            id: detectionLayout
                            Layout.preferredHeight: 0
                            spacing: 2
                            clip: true
                            Layout.preferredWidth: rootItem.width
                            Rectangle {
                                Layout.preferredWidth: img3.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    id: img3
                                    verticalAlignment: Image.AlignTop
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/resources/system.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24
                                    rotation: 45
                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                spacing: 0
                                Repeater {
                                    model: DetectionSystems
                                    Label {
                                        Layout.preferredWidth: rootItem.width - 30
                                        Layout.preferredHeight: implicitHeight
                                        Layout.leftMargin: 5
                                        leftInset: -5
                                        Layout.bottomMargin: 3
                                        bottomInset: -3
                                        color: "white"
                                        text: DetectionSystems[index]
                                        horizontalAlignment: Qt.AlignLeft
                                        background: Rectangle {
                                            color: boxColor
                                        }
                                    }

                                }
                            }
                        }


                        Rectangle {
                            id: secondButton
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: implicitHeight + 25
                            property var checked: true

                            MouseArea {
                                hoverEnabled: true
                                anchors.fill: parent
                                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                onClicked: if (secondButton.checked) {
                                               sendsLayout.Layout.preferredHeight = sendsLayout.implicitHeight
                                               imgs.rotation = 180
                                               secondButton.checked = false
                                           }
                                           else {
                                               sendsLayout.Layout.preferredHeight = 0
                                               imgs.rotation = -90
                                               secondButton.checked = true
                                           }
                            }

                            color: "#1a75ff"

                            Text {
                                text: "Sends"
                                color: "white"
                                anchors.centerIn: parent
                                anchors.left: parent.left
                            }
                            Image {
                                id: imgs
                                source: "qrc:/Resources/chevron.png"
                                width: 16
                                height: 16
                                rotation: -90
                                ColorOverlay {
                                    source: imgs
                                    anchors.fill: imgs
                                    color: "white"
                                }
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                            }

                        }

                        RowLayout {
                            id: sendsLayout
                            Layout.preferredHeight: 0
                            spacing: 2
                            clip: true
                            Layout.preferredWidth: rootItem.width

                            Rectangle {
                                Layout.preferredWidth: img4.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    id: img4
                                    verticalAlignment: Image.AlignTop
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/resources/station_lV.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height: 24
                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                spacing: 0
                                Repeater {
                                    model: Sends
                                    Label {
                                        Layout.preferredWidth: rootItem.width - 30
                                        Layout.preferredHeight: implicitHeight
                                        Layout.leftMargin: 5
                                        leftInset: -5
                                        Layout.bottomMargin: 3
                                        bottomInset: -3
                                        color: "white"
                                        text: Sends[index]
                                        horizontalAlignment: Qt.AlignLeft
                                        background: Rectangle {
                                            color: boxColor
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            }
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
