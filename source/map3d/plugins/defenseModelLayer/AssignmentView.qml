import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Crystal 1.0
Item {

    id: rootItem

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


                        Rectangle {
                            id: rct
                            anchors.centerIn: parent
                            implicitWidth: 120
                            implicitHeight:  txt.implicitHeight + 10
                            color: "transparent"
                            Rectangle {
                                opacity: 0.2
                                color: (rootItem.hoveredIndex == row) ? "lightskyblue" : "transparent"
                                anchors.fill: parent
                            }
                            Text {
                                id: txt
                                anchors.centerIn: parent
                                text: SysDisp
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

                        Rectangle {
                            id: rct1
                            anchors.centerIn: parent
                            implicitWidth: 120
                            implicitHeight:  txt1.implicitHeight + 10
                            color: "transparent"
                            Rectangle {
                                opacity: 0.2
                                color: (rootItem.hoveredIndex == row) ? "lightskyblue" : "transparent"
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
