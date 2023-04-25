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
    property var boxColor: "#363739"
    property var valueColor: "#363739"
    property var keyTextColor: "#9b9ca0"
    property var spacee: 5

    Rectangle{
        anchors.fill: parent
        color: "#2F2E2C"
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
                    Layout.alignment: Qt.AlignCenter
                    Layout.maximumWidth: 100
                    clip: true
                    id: headerName
                    text: "-------"
                    color:"yellow"
                    font.pointSize: 20
                }

                Rectangle {
                    width: 1
                    opacity: 0.5
                    height: 30
                    color: "white"
                }

                Text {
                    id: station
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: implicitWidth
                    text: "--------"
                    color:"yellow"
                    font.pointSize: 20
                }
//                Switch {
//                    id: control
//                    ToolTip {
//                        parent: control
//                        y: control.y + control.height
//                        Text{
//                            text: control.checked ? "Click to deactivate Station" : "Click to activate Station"
//                            color: "white"
//                        }

//                        background: Rectangle {
//                            color: "#404040"
//                            radius: 4
//                        }

//                        visible:  control.hovered
//                    }
//                    onToggled: function() {
//                        rootItem.model.activeButtonToggled(checked);
//                    }

//                    Layout.alignment: Qt.AlignRight
//                    indicator: Rectangle {
//                        implicitWidth: 48
//                        implicitHeight: 26
//                        x: control.leftPadding
//                        y: parent.height / 2 - height / 2
//                        radius: 13
//                        color: control.checked ? "#17a81a" : "#a8171a"
//                        border.color: control.checked ? "#17a81a" : "#a8171a"

//                        Rectangle {
//                            x: control.checked ? parent.width - width : 0
//                            width: 26
//                            height: 26
//                            radius: 13
//                            color: control.down ? "#cccccc" : "#ffffff"
//                            border.color: control.checked ? (control.down ? "#17a81a" : "#21be2b") : "#999999"
//                        }
//                    }
//                }
            }

        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollView {
                background: Rectangle {
                    color: boxColor
                }

                clip: true
                ScrollBar.vertical.interactive: false
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
                            target: station
                            property: "text"
                            value: Numberr
                        }
//                        Binding {
//                            target: control
//                            property: "checked"
//                            value: Active
//                        }

                        Binding {
                            target: headerName
                            property: "text"
                            value: MainInfo[0]
                        }

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
                                    source: "qrc:/resources/sinfo.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24

                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                Layout.preferredHeight: implicitHeight
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
                            ColumnLayout {
                                Layout.preferredHeight: implicitHeight
                                Repeater {
                                    model: MainInfo
                                    Label {
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
                                rows: 4
                                columns: 1
                                Layout.fillWidth: true
                                Layout.preferredHeight: implicitHeight
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
                                Layout.fillWidth: true
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
                }
            }
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
