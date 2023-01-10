import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Extras 1.4
import Crystal 1.0

Item {
    id: rootItem
    x: 0
    width: 300
    clip: true

    property StationInfoModel model
    property bool on: false

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
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 35
                Text {
                    id: station
//                    anchors.centerIn: parent
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredWidth: implicitWidth
                    text: "Station"
                    color:"yellow"
                    font.pointSize: 20
                }
                Switch {
                    id: control
                    ToolTip {
                        parent: control
                        y: control.y + control.height
//                        x: station.x + 50
                        Text{
                            text: control.checked ? "Click to deactivate Station" : "Click to activate Station"
                            color: "white"
                        }

                        background: Rectangle {
                            color: "#404040"
                            radius: 4
                        }

                        visible:  control.hovered
                    }
                    onToggled: function() {
                        console.log(checked)
                    }

                    Layout.alignment: Qt.AlignRight
                    indicator: Rectangle {
                        implicitWidth: 48
                        implicitHeight: 26
                        x: control.leftPadding
                        y: parent.height / 2 - height / 2
                        radius: 13
                        color: control.checked ? "#17a81a" : "#a8171a"
                        border.color: control.checked ? "#17a81a" : "#a8171a"

                        Rectangle {
                            x: control.checked ? parent.width - width : 0
                            width: 26
                            height: 26
                            radius: 13
                            color: control.down ? "#cccccc" : "#ffffff"
                            border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
                        }
                    }
                }
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
                            Layout.preferredHeight: 200
                            Image {
                                id: img
                                anchors.fill: parent
                                source: "qrc:///Resources/station.jpg"
                                fillMode: Image.Stretch
                            }
                        }


                        Rectangle {
                            Layout.preferredWidth: rootItem.width - 30
                            Layout.preferredHeight: 2
                            color: "white"

                        }

                        GridLayout {
                            rows: 4
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
                                    target: station
                                    property: "text"
                                    value: nameLabel.text
                                }
                            }
                            Label {
                                visible: false
                                Binding {
                                    target: control
                                    property: "checked"
                                    value: Active == "Active"
                                }
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
                            }}

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
                                    text: "Radius: "
                                    color: "white"
                                    Layout.preferredWidth: implicitWidth
                                }
                                Label {
                                    id: radiusLabel
                                    text: Radius
                                    color: "white"
                                    Layout.fillWidth: true
                                    horizontalAlignment: Qt.AlignRight
                                }
                                Label {
                                    text: "CycleTime: "
                                    color: "white"
                                    Layout.preferredWidth: implicitWidth
                                }
                                Label {
                                    id: cycleTimeLabel
                                    text: CycleTime
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
        Item {
            Layout.minimumHeight: 5
        }

        StationInfoButtonGroup {
            Layout.fillWidth: true
            Layout.minimumHeight: 60

            onGotoButtonClicked: function() {
                rootItem.model.gotoButtonClicked();
            }

            onRangeButtonClicked: function(check) {
                rootItem.model.rangeButtonClicked(check);
            }
            onVisibleButtonClicked: function(check) {
                rootItem.model.visibleButtonClicked(check);
            }
            onActivateButtonClicked: function(check) {
                rootItem.model.activateButtonClicked(check);
            }
            onMoreButtonClicked: function() {
                rootItem.model.moreButtonClicked();
            }
        }
    }
}
