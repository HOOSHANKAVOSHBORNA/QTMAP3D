import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import Crystal 1.0

Item {
    id: rootItem
    x: 0
    width: 300
    clip: true

    property SystemInfoModel model

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
                    id: system
//                    anchors.centerIn: parent
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: implicitWidth
                    text: "System"
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
                        rootItem.model.activeButtonToggled(checked);
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
                                source: "qrc:///Resources/system.jpg"
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
                                visible: false
                                Binding {
                                    target: control
                                    property: "checked"
                                    value: Active
                                }
                            }
                            Label {
                                id: nameLabel
                                text: Name
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                                Binding {
                                    target: system
                                    property: "text"
                                    value: nameLabel.text
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
                                text: "Terminal: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: terminlLabel
                                text: Terminal
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                        }
                        Rectangle {
                            Layout.preferredWidth: rootItem.width - 150
                            Layout.leftMargin: 60
                            Layout.preferredHeight: 0.5
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
                                text: "ViewRange: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: viewRangeLabel
                                text: ViewRange
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                            Label {
                                text: "MezRange: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                id: mezRangeLabel
                                text: MezRange
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
                            text: "Status Information"
                            horizontalAlignment: Qt.AlignHCenter
                        }

                        GridLayout {
                            rows: 7
                            columns: 2
                            Layout.fillWidth: true
                            Layout.preferredHeight: implicitHeight

                            Label {
                                text: "ReceiveTime: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: ReceiveTime
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                            Label {
                                text: "Simulation: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: Simulation
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                            Label {
                                text: "BCCStatus: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: BCCStatus
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                            Label {
                                text: "RadarSearch Status: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: RadarSearchStatus
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                            Label {
                                text: "Operational: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: Operational
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                            Label {
                                text: "MissileCount: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: MissileCount
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                            Label {
                                text: "RadarMode: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: RadarMode
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
                            text: "Combat Information"
                            horizontalAlignment: Qt.AlignHCenter
                        }
                        GridLayout {
                            rows: 6
                            columns: 2
                            Layout.fillWidth: true
                            Layout.preferredHeight: implicitHeight

                            Label {
                                text: "TN: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: TN
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                            Label {
                                text: "Acceptance: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: Acceptance
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                            Label {
                                text: "Phase: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: Phase
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                            Label {
                                text: "Antenna: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: Antenna
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                            Label {
                                text: "ChanelNo: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: ChanelNo
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }
                            Label {
                                text: "Inrange: "
                                color: "white"
                                Layout.preferredWidth: implicitWidth
                            }
                            Label {
                                text: Inrange
                                color: "white"
                                Layout.fillWidth: true
                                horizontalAlignment: Qt.AlignRight
                            }

                        }
                    }
                }
            }
        }
        Item {
            Layout.minimumHeight: 5
        }

        SystemInfoButtonGroup {
            Layout.fillWidth: true
            Layout.minimumHeight: 60

            onGotoButtonClicked: function() {
                rootItem.model.gotoButtonClicked();
            }

            onRangeButtonClicked: function(check) {
                rootItem.model.rangeButtonClicked(check);
            }
            onWezButtonClicked: function(check) {
                rootItem.model.wezButtonClicked(check);
            }
            onMezButtonClicked: function(check) {
                rootItem.model.mezButtonClicked(check);
            }
            onMoreButtonClicked: function() {
                rootItem.model.moreButtonClicked();
            }
        }
    }
}