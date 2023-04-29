import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtGraphicalEffects 1.13
import Crystal 1.0
Item {

    id: rootItem
    property int sHoveredIndex: -1
    property int sSelectedIndex: -1

    property int aHoveredIndex: -1
    property int aSelectedIndex: -1
    property int aClicked: -1
    property int sClicked: -1
    property AircraftTableModel aircraftModel
    property SystemTableModel systemModel

    property var refreshx: 0
    signal aircraftDoubleClicked(int TN)
    signal systemDoubleClicked(int Number)

    property var hoverColor: "#add8e6"
    property var headerColor: "#00587A"

    SplitView {
        y: 400
        id: splitView
        anchors.fill: parent
        anchors.topMargin: 35
        anchors.centerIn: parent
        handle: Rectangle {
            id: handleDelegate
            implicitWidth: 4
//            parent.handle.onStatusChanged: function() {
//                 handleDelegate.color =  "#D0C000"
//            }
            MouseArea {
                id: test
                anchors.fill: parent
                hoverEnabled: true

                onPressed: function(mouse) {
                    mouse.accepted = false
                }
                propagateComposedEvents: true
                cursorShape: Qt.SizeHorCursor

            }

            color: test.containsMouse ? "#D0C000" : "white"

            Rectangle {
                anchors.left: handleDelegate.right
//                color: (leftRectArea.containsMouse || rightRect.containsMouse) ? "#81e889"
//                                           : (SplitHandle.hovered ? Qt.lighter("#c2f4c6", 1.1) : "#c2f4c6")
                color: "#252525"
                width: 0
                height: parent.height
                MouseArea {
                    id: rightRect
                    hoverEnabled: true
                    anchors.fill: parent
                    onClicked: refreshimg.clicked(
                                   Qt.LeftButton)
                }
            }
            Rectangle {
                id: leftRect
                anchors.right: handleDelegate.left
//                color: (leftRectArea.containsMouse || rightRect.containsMouse)? "#81e889"
//                                           : (SplitHandle.hovered ? Qt.lighter("#c2f4c6", 1.1) : "#c2f4c6")
                color: "#252525"
                width: 28
                height: parent.height
                MouseArea {
                    id: leftRectArea
                    hoverEnabled: true
                    anchors.fill: parent
                    onClicked: refreshimg.clicked(
                                   Qt.LeftButton)
                }
            }
            Rectangle {
                anchors.right: leftRect.left
                color: "#252525"
                width: 12
                height: parent.height
                MouseArea {
                    anchors.fill: parent
                }
            }

            Rectangle {
                width: 5
                height: 24
                y: img.y
                color: "#252525"
            }
            Image {
                id: img
                source: "qrc:/resources/refresh.png"
                width: 24
                height: 24


                anchors.centerIn: parent
                MouseArea {
                    id: refreshimg
                    anchors.fill: parent
                    onClicked: {
                        rootItem.aircraftModel.refresh(3);
                        rootItem.systemModel.refresh(3);
                        rootItem.aClicked = -1
                        rootItem.sClicked = -1
                    }
                    hoverEnabled: true

                }
            }
            ColorOverlay {
                id: glowimg
                anchors.fill: img
                color: refreshimg.pressed ? "#D0C000" : refreshimg.containsMouse ? "yellow" : "white"
                source: img
                visible: true
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            SplitView.preferredWidth: 500
            SplitView.fillWidth: true
            SplitView.minimumWidth: 300
            clip: true
            Item {
                Layout.preferredHeight: 40
                Layout.minimumHeight: 40
                Layout.leftMargin: 24
                Layout.rightMargin: 32

                RowLayout {
                    anchors.top: parent.top
                    height: 40
                    width: 14 * (113 + 2)
                    spacing: 2
                    anchors.leftMargin: 2 - aircrafts.contentX
                    anchors.rightMargin: 2
                    anchors.left: parent.left

                    Repeater {
                        model: 14
                        Rectangle {
                            width: 113
                            height: 40
                            color: headerColor
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
                Layout.leftMargin: 32
                ScrollBar.vertical: ScrollBar {
                    active: true
                    Component.onCompleted: x = -33
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                }
                TableView {
                    id: aircrafts
                    model: rootItem.aircraftModel
                    columnWidthProvider: function (column) {
                        if (column >= 14)
                            return 0
                        return 115
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
                                        if (rootItem.aircraftModel){
                                            rootItem.aClicked = row;
                                            rootItem.sClicked = -1;
                                        }
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
                            implicitWidth: 115
                            implicitHeight:  txt.implicitHeight + 10
                            color: "transparent"
                            Rectangle {
                                opacity: 1
                                color: rootItem.aircraftModel ? (column == 0 ? AircraftColor:
                                                                               "transparent") : "transparent";
                                anchors.centerIn: parent
                                width: 25
                                height: 23
                                radius: 7
                            }
                            Rectangle {
                                opacity: 0.3
                                color: rootItem.aircraftModel ? (rootItem.aClicked == row ? "lightskyblue" :
                                                                                            rootItem.aHoveredIndex == row ? hoverColor :
                                                                                                                            "transparent") : "transparent";
                                anchors.fill: parent
                            }

                            Rectangle {
                                width: 20
                                height: 20
                                x: -23
                                radius: 10
                                anchors.verticalCenter: parent.verticalCenter
                                color: "transparent"
                                Image {
                                    id: img6
                                    anchors.fill: parent
                                    source: "qrc:/resources/select.png"
                                    rotation: 90
                                    visible: false

                                }
                                ColorOverlay {
                                    anchors.fill: img6
                                    color: (row == rootItem.aClicked && column == 0) ? "#FFFFFF" : (rootItem.aHoveredIndex == row ? "#404040" : "transparent")
                                    source: img6
                                    rotation: 90
                                    visible: column == 0 && (row == aHoveredIndex || row == aClicked)
                                }
                            }

                            Text {
                                id: txt
                                anchors.centerIn: parent
                                text: rootItem.aircraftModel ? display : "";
                                color: column == 0? "black" : "white"
                                font.bold: column == 0
                            }
                        }
                    }
                }
            }
        }


        ColumnLayout {
            id: systemsLayout
            SplitView.fillWidth: true
            SplitView.preferredWidth: 500
            clip: true
            Item {
                Layout.fillWidth: true;
                Layout.preferredHeight: 40
                Layout.minimumHeight: 40
                Layout.leftMargin: 32

                Row {
                    anchors.top: parent.top
                    height: 40
                    width: 22 * (113 + 2)
                    spacing: 2
                    anchors.leftMargin: 2 - systems.contentX
                    anchors.rightMargin: 2
                    anchors.left: parent.left

                    Repeater {
                        model: 22
                        Rectangle {
                            width: 113
                            height: 40
                            color: headerColor
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
                Layout.leftMargin: 32
                TableView {
                    id: systems
                    model: rootItem.systemModel
                    columnWidthProvider: function (column) {
                        if (column > 22)
                            return 0
                        return 115
                    }
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
                                        if (rootItem.systemModel) {
                                            rootItem.sClicked = row
                                            rootItem.aClicked = -1;
                                        }
                                    }
                                }
                            }

                            onDoubleClicked: function() {
                                timer2.stop()
                                if (rootItem.systemModel) {
                                    rootItem.systemDoubleClicked(rootItem.systemModel.getNumber(row));
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
                            implicitWidth: 115
                            implicitHeight:  txt1.implicitHeight + 10
                            color: "transparent"

                            Rectangle {
                                opacity: 1
                                color:  "transparent";
                                anchors.centerIn: parent
                                width: 70
                                height: 23
                                radius: 7
                            }

                            Rectangle {
                                opacity: 0.3
                                color: rootItem.systemModel ? (rootItem.sClicked == row ? "lightskyblue" :
                                                                                          rootItem.sHoveredIndex == row ? hoverColor :
                                                                                                                          "transparent") : "transparent";
                                anchors.fill: parent
                            }

                            Rectangle {
                                width: 20
                                height: 20
                                x: -23
                                radius: 10
                                anchors.verticalCenter: parent.verticalCenter
                                color: "transparent"

                                Image {
                                    id: img7
                                    anchors.fill: parent
                                    source: "qrc:/resources/select.png"
                                    rotation: 90
                                    visible: false

                                }
                                ColorOverlay {
                                    anchors.fill: img7
                                    color: (row == rootItem.sClicked && column == 0) ? "#FFFFFF" : (rootItem.sHoveredIndex == row ? "#404040" : "transparent")
                                    source: img7
                                    rotation: 90
                                    visible: column == 0 && (row == sHoveredIndex || row == sClicked)
                                }
                            }

                            Text {
                                id: txt1
                                anchors.centerIn: parent
                                text: display
                                color: column == 4? SystemColor : d_txtcolor

                                font.bold: column == 4
                            }
                        }
                    }
                }
            }
        }
    }
}
