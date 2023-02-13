import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Crystal 1.0
Item {

    id: rootItem
    property int sHoveredIndex: -1
    property int sSelectedIndex: -1

    property int aHoveredIndex: -1
    property int aSelectedIndex: -1
    property AssignmentModel model
    Rectangle {
        anchors.fill: parent
        color: "#252525"
    }


    RowLayout {
        anchors.fill: parent
        anchors.centerIn: parent
        ColumnLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 50
            Item {
                Layout.fillWidth: true;
                Layout.preferredHeight: 40
                Layout.minimumHeight: 40


                Row {
                    anchors.top: parent.top
                    height: 40
                    width: 4 * (120 + 4)
                    spacing: 2
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                    anchors.left: parent.left

                    Repeater {
                        model: 4
                        Rectangle {
                            width: 120
                            height: 40
//                            radius: 5
                            color: '#4568dc'
                            Text {
                                color: '#FFFFFF'
                                text: rootItem.model ? rootItem.model.headerText(index) : "";
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                TableView {
                    id: aircrafts
                    model: rootItem.model
                    contentWidth: 120
                    delegate: Item {
                        implicitWidth:   rct.implicitWidth
                        implicitHeight:  rct.implicitHeight + 4
                        MouseArea {
                            id: mouseArea
                            hoverEnabled: true
                            anchors.fill: parent
                            onClicked: function() {
                                if (rootItem.model) {

                                }
                            }

                            onContainsMouseChanged: function() {
                                if (mouseArea.containsMouse) {
                                    rootItem.aHoveredIndex = row;
                                } else {
                                    if (rootItem.aHoveredIndex == row) {
                                        rootItem.aHoveredIndex = -1;
                                    }
                                }
                            }
                        }

                        Rectangle {
                            id: rct
                            anchors.centerIn: parent
                            implicitWidth: 120
                            implicitHeight:  txt.implicitHeight + 10
                            color: "transparent"
                            Rectangle {
                                opacity: 0.2
                                color: AirDisp == "" ? "transparent" : (rootItem.aHoveredIndex == row) ? "lightskyblue" : "transparent"
                                anchors.fill: parent
                            }
                            Text {
                                id: txt
                                anchors.centerIn: parent
                                text: AirDisp
                                color: "white"
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
            Layout.preferredWidth: 40
            Image {
                source: "qrc:/resources/arrow.png"
                width: 40
                height: 40
                anchors.centerIn: parent
                rotation: 180
            }
        }
        ColumnLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 50
            Item {
                Layout.fillWidth: true;
                Layout.preferredHeight: 40
                Layout.minimumHeight: 40


                Row {
                    anchors.top: parent.top
                    height: 40
                    width: 4 * (120 + 4)
                    spacing: 2
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                    anchors.left: parent.left

                    Repeater {
                        model: 4
                        Rectangle {
                            width: 120
                            height: 40
//                            radius: 5
                            color: '#4568dc'
                            Text {
                                color: '#FFFFFF'
                                text: rootItem.model ? rootItem.model.headerText(index) : "";
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
            }
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                TableView {
                    id: systems
                    model: rootItem.model
                    contentWidth: 120
                    delegate: Item {
                        implicitWidth:   rct1.implicitWidth
                        implicitHeight:  rct1.implicitHeight + 4
                        MouseArea {
                            id: mouseArea2
                            hoverEnabled: true
                            anchors.fill: parent
                            onClicked: function() {
                                if (rootItem.model) {

                                }
                            }

                            onContainsMouseChanged: function() {
                                if (mouseArea2.containsMouse) {
                                    rootItem.sHoveredIndex = row;
                                } else {
                                    if (rootItem.sHoveredIndex == row) {
                                        rootItem.sHoveredIndex = -1;
                                    }
                                }
                            }
                        }
                        Rectangle {
                            id: rct1
                            anchors.centerIn: parent
                            implicitWidth: 120
                            implicitHeight:  txt1.implicitHeight + 10
                            color: "transparent"
                            Rectangle {
                                opacity: 0.2
                                color: SysDisp == "" ? "transparent" : (rootItem.sHoveredIndex == row) ? "lightskyblue" : "transparent"
                                anchors.fill: parent
                            }
                            Text {
                                id: txt1
                                anchors.centerIn: parent
                                text: SysDisp
                                color: "white"
                            }
                        }
                    }
                }
            }
        }
    }
}
