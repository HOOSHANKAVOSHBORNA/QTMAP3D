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
    property int aClicked: -1
    property int sClicked: -1
    property AssignmentModel model
    property AircraftTableModel aircraftModel
    property SystemTableModel systemModel


    signal aircraftDoubleClicked(int TN)
    signal systemDoubleClicked(int Number)
//    Rectangle {
//        anchors.fill: parent
//        color: "#252525"
//    }


    Rectangle{
        parent: splitView
        width: 40
        height: 40
        x: rootItem.width / 2 - 30
//        y: 35
        Image {
            id: img
            source: "qrc:/resources/refresh.png"
            width: 35
            height: 35
            anchors.centerIn: parent
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rootItem.aircraftModel.refresh(3);
                    rootItem.systemModel.refresh(3);
                    rootItem.aClicked = -1
                    rootItem.sClicked = -1
                }
            }
        }
        color: "#252525"
    }



    SplitView {
        y: 400
        id: splitView
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.topMargin: 35
        anchors.centerIn: parent
        handle: Rectangle {
                    id: handleDelegate
                    implicitWidth: 4
                    implicitHeight: 4
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: SplitHandle.pressed ? "#81e889"
                        : (SplitHandle.hovered ? Qt.lighter("#c2f4c6", 1.1) : "#c2f4c6")

                    containmentMask: Item {
                        x: (handleDelegate.width - width) / 2
                        width: 64
                        y: 350
                        height: splitView.height
                    }

                }

        ColumnLayout {
            Layout.fillWidth: true
            SplitView.minimumWidth: 500
            SplitView.fillWidth: true
            Item {
                Layout.preferredHeight: 40
                Layout.minimumHeight: 40


                RowLayout {
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
                                text: rootItem.aircraftModel ? rootItem.aircraftModel.headerText(index) : "";
                                anchors.centerIn: parent
                            }
                        }
                    }

                    }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.topMargin: 15
                TableView {
                    id: aircrafts
                    model: rootItem.aircraftModel
//                    contentWidth: 120
                    columnWidthProvider: function (column) {
                        if (column > 3)
                            return 0
                        return 120
                    }

                    delegate: Item {
                        implicitWidth:   rct.implicitWidth
                        implicitHeight:  rct.implicitHeight + 4
                        MouseArea {
                            id: mouseArea
                            hoverEnabled: true
                            anchors.fill: parent
                            Timer{
                                id:timer
                                interval: 200
                                onTriggered: function () {
                                    if (rootItem.aircraftModel) {
                                        rootItem.aircraftModel.onAircraftClicked(rootItem.aircraftModel.getTN(row))
                                        if (rootItem.systemModel) {
                                            systems.contentX = 0;
                                            systems.contentY = 0;
                                        }
                                        if (rootItem.aircraftModel)
                                            rootItem.aClicked = row
                                    }
                                }
                            }
                            onClicked: function() {
                                if (timer.running){
                                    timer.stop();
                                } else{
                                    timer.restart();

                                }
                            }

                            onDoubleClicked: function() {
                                timer.stop();
                                if (rootItem.aircraftModel) {
                                    rootItem.aircraftDoubleClicked(rootItem.aircraftModel.getTN(row));
                                }
                            }

//                            onContainsPressChanged: function () {
//                                if (rootItem.aircraftModel)
//                                    rootItem.aClicked = row
//                            }

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
                                color: (aClicked == row && rootItem.systemModel.getShowAssigned()) ? "#1010FF" : (rootItem.aHoveredIndex == row) ? "lightskyblue" : "transparent"
                                anchors.fill: parent
                            }
                            Text {
                                id: txt
                                anchors.centerIn: parent
                                text: display
                                color: "white"
                            }
                        }
                    }
                }
            }
        }


        ColumnLayout {
            SplitView.fillWidth: true
            SplitView.minimumWidth: 500
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
                                text: rootItem.systemModel ? rootItem.systemModel.headerText(index) : "";
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
            }
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.topMargin: 15
                TableView {
                    id: systems
                    model: rootItem.systemModel
                    contentWidth: 120
                    delegate: Item {
                        implicitWidth:   rct1.implicitWidth
                        implicitHeight:  rct1.implicitHeight + 4
                        MouseArea {
                            id: mouseArea2
                            hoverEnabled: true
                            anchors.fill: parent
                            onClicked: function() {
                                if (timer2.running){
                                    timer2.stop();
                                } else{
                                    timer2.running = false;
                                    timer2.restart();
                                }
                            }


                            Timer{
                                id:timer2
                                interval: 200
                                onTriggered: function () {
                                    if (rootItem.systemModel) {
                                        rootItem.systemModel.onSystemClicked(rootItem.systemModel.getNumber(row));
                                        if (rootItem.aircraftModel) {
                                            aircrafts.contentX = 0;
                                            aircrafts.contentY = 0;
                                        }
                                        if (rootItem.systemModel)
                                            rootItem.sClicked = row
                                    }
                                }
                            }

                            onDoubleClicked: function() {
                                timer2.stop()
                                if (rootItem.systemModel) {
                                    rootItem.systemDoubleClicked(rootItem.systemModel.getNumber(row));
                                }
                            }

//                            onContainsPressChanged: function () {
//                                if (rootItem.systemModel)
//                                    rootItem.sClicked = row
//                            }

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
                                color: (sClicked == row && rootItem.aircraftModel.getShowAssigned()) ? "#1010FF" : (rootItem.sHoveredIndex == row) ? "lightskyblue" : "transparent"
                                anchors.fill: parent
                            }
                            Text {
                                id: txt1
                                anchors.centerIn: parent
                                text: display
                                color: "white"
                            }
                        }
                    }
                }
            }
        }
    }
}
