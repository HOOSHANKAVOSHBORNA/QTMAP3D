
import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import Crystal 1.0

Item {
    id: rootItem
//    anchors.fill: parent

    signal filterTextChanged(string txt)
    signal stationDoubleClicked(int Number)

    property int hoveredIndex: -1
    property int selectedIndex: -1

    property var hoverColor: "#add8e6"
    property var headerColor: "#00587A"

    Timer {
        id: signalTimer
        running: false
        repeat: false
        interval: 500
        onTriggered: function() {
            rootItem.filterTextChanged(filterInput.text);
            tableView.contentX = 0;
            tableView.contentY = 0;
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#252525"
    }

    property StationTableModel model

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 10

        RowLayout {
            Layout.alignment: Qt.AlignCenter
            Layout.minimumWidth:   implicitWidth
            Layout.preferredWidth: implicitWidth
            Layout.minimumHeight:   implicitHeight
            Layout.preferredHeight: implicitHeight
            spacing: 10

            Label {
                id: searchLabel
                text: "Search Name : "
                color : "white"
                Layout.minimumWidth: implicitWidth
                Layout.minimumHeight:   implicitHeight
                Layout.alignment: Qt.AlignCenter
            }

            Rectangle {
                color: "transparent"
                border.color: headerColor
                radius: 5
                Layout.minimumWidth:   320
                Layout.preferredWidth: 320
                Layout.minimumHeight:   40
                Layout.preferredHeight: 40


                TextInput {
                    id: filterInput
                    anchors.centerIn: parent
                    width: 300
                    height: 40
                    color: "white"
                    clip: true
                    onTextChanged: function() {
                        if (signalTimer.running === true) {
                            signalTimer.restart();
                        } else {
                            signalTimer.start();
                        }
                    }
                    verticalAlignment: Qt.AlignVCenter

                }
            }
            Item {
                Layout.minimumWidth: searchLabel.implicitWidth
                Layout.minimumHeight: searchLabel.implicitHeight
                Layout.alignment: Qt.AlignCenter
            }

        }

        Item {
            Layout.fillWidth: true;
            Layout.preferredHeight: 40
            Layout.minimumHeight: 40

            Row {
                anchors.top: parent.top
                height: 40
                width: 8 * (160 + 4)
                spacing: 4
                anchors.leftMargin: 2 - tableView.contentX
                anchors.rightMargin: 2
                anchors.left: parent.left

                Repeater {
                    model: 8
                    Rectangle {
                        width: 160
                        height: 40
                        radius: 5
                        color: headerColor
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
                id: tableView
                model: rootItem.model
                contentWidth: 8 * (164)
                clip:true

                delegate: Item {
                    implicitWidth:   rct.implicitWidth
                    implicitHeight:  rct.implicitHeight + 4
                    MouseArea {
                        id: mouseArea
                        hoverEnabled: true
                        anchors.fill: parent
                        onDoubleClicked: function() {
                            if (rootItem.model) {
                                rootItem.stationDoubleClicked(rootItem.model.getNumber(row));
                            }
                        }

                        onContainsMouseChanged: function() {
                            if (mouseArea.containsMouse) {
                                rootItem.hoveredIndex = row;
                            } else {
                                if (rootItem.hoveredIndex == row) {
                                    rootItem.hoveredIndex = -1;
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: rct
                        anchors.centerIn: parent
                        implicitWidth: 164
                        implicitHeight:  txt.implicitHeight + 10
                        color: "transparent"
                        Rectangle {
                            opacity: 0.2
                            color: (rootItem.hoveredIndex == row) ? hoverColor : "transparent"
                            anchors.fill: parent
                        }
                        Text {
                            id: txt
                            anchors.centerIn: parent
                            text: display
                            color: d_txtcolor
                        }
                    }
                }

            }
        }
    }
}
