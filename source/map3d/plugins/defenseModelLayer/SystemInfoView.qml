import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.0
import Crystal 1.0

Item {
    id: rootItem
    x: 0
    width: 300
    clip: true

    property SystemInfoModel model
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
                    id: system
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
                        spacing: 0

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        width: rootItem.width

                        Binding {
                            target: system
                            property: "text"
                            value: Name
                        }

                        Binding {
                            target: control
                            property: "checked"
                            value: Active
                        }

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
//                                Layout.fillWidth: true
//                                Layout.preferredWidth: implicitWidth
                                Layout.preferredHeight: implicitHeight
                                rowSpacing: 0
                                columnSpacing: 0
                                GridLayout {
                                    rows: 5
                                    columns: 1
                                    Layout.preferredHeight: implicitHeight
                                    Layout.preferredWidth: wid.implicitWidth
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
                                               statusLayout.Layout.preferredHeight = statusLayout.implicitHeight
                                               imgd.rotation = 180
                                               firstButton.checked = false
                                           }
                                           else {
                                               statusLayout.Layout.preferredHeight = 0
                                               imgd.rotation = -90
                                               firstButton.checked = true
                                           }
                            }

                            color: "#1a75ff"

                            Text {
                                text: "Status Information"
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
                            id: statusLayout
                            spacing: 2
                            clip: true
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: 0
                            Rectangle {
                                Layout.preferredWidth: img3.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    id: img3
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
//                                Layout.fillWidth: true
//                                Layout.preferredWidth: implicitWidth
                                Layout.preferredHeight: implicitHeight
                                rowSpacing: 0
                                columnSpacing: 0
                                GridLayout {
                                    rows: 5
                                    columns: 1
                                    Layout.preferredHeight: implicitHeight
                                    Layout.preferredWidth: wid.implicitWidth
                                    columnSpacing: 0
                                    rowSpacing: 0
                                    Repeater {
                                        model: StatusInfoHeaders
                                        Label {
                                            text: StatusInfoHeaders[index]
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
                                        model: StatusInfo
                                        Label {
                                            text: StatusInfo[index]
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
                            id: secondButton
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: implicitHeight + 25
                            property var checked: true

                            MouseArea {
                                hoverEnabled: true
                                anchors.fill: parent
                                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                onClicked: if (secondButton.checked) {
                                               combatLayout.Layout.preferredHeight = combatLayout.implicitHeight
                                               imgs.rotation = 180
                                               secondButton.checked = false
                                           }
                                           else {
                                               combatLayout.Layout.preferredHeight = 0
                                               imgs.rotation = -90
                                               secondButton.checked = true
                                           }
                            }

                            color: "#1a75ff"

                            Text {
                                text: "Combat Information"
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
                            id: combatLayout
                            spacing: 2
                            clip: true
                            Layout.preferredHeight: 0
                            Layout.preferredWidth: rootItem.width
                            Rectangle {
                                Layout.preferredWidth: img4.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    id: img4
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
//                                Layout.fillWidth: true
//                                Layout.preferredWidth: implicitWidth
                                Layout.preferredHeight: implicitHeight
                                rowSpacing: 0
                                columnSpacing: 0
                                GridLayout {
                                    rows: 5
                                    columns: 1
                                    Layout.preferredHeight: implicitHeight
                                    Layout.preferredWidth: wid.implicitWidth
                                    columnSpacing: 0
                                    rowSpacing: 0
                                    Repeater {
                                        model: CombatInfoHeaders
                                        Label {
                                            text: CombatInfoHeaders[index]
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
                                        model: CombatInfo
                                        Label {
                                            text: CombatInfo[index]
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

//                        GridLayout {
//                            rows: 4
//                            columns: 2
//                            Layout.fillWidth: true
//                            Layout.preferredHeight: implicitHeight

//                            Label {
//                                text: "Name: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                visible: false
//                                Binding {
//                                    target: control
//                                    property: "checked"
//                                    value: Active
//                                }
//                            }
//                            Label {
//                                id: nameLabel
//                                text: Name
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                                Binding {
//                                    target: system
//                                    property: "text"
//                                    value: nameLabel.text
//                                }
//                            }

//                            Label {
//                                text: "Number: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                id: numberLabel
//                                text: Numberr
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "Type: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                id: typeLabel
//                                text: Type
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "Terminal: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                id: terminlLabel
//                                text: Terminal
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                        }
//                        Rectangle {
//                            Layout.preferredWidth: rootItem.width - 150
//                            Layout.leftMargin: 60
//                            Layout.preferredHeight: 0.5
//                            color: "white"
//                        }
//                        GridLayout {
//                            rows: 5
//                            columns: 2
//                            Layout.fillWidth: true
//                            Layout.preferredHeight: implicitHeight
//                            Label {
//                                text: "Latitude: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                id: latitudeLabel
//                                text: Latitude
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "Longitude: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                id: longitudeLabel
//                                text: Longitude
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "Altitude: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                id: altitudeLabel
//                                text: Altitude
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "ViewRange: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                id: viewRangeLabel
//                                text: ViewRange
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "MezRange: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                id: mezRangeLabel
//                                text: MezRange
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                        }

//                        Rectangle {
//                            Layout.preferredWidth: rootItem.width - 30
//                            Layout.preferredHeight: 2
//                            color: "white"

//                        }
//                        Label {
//                            Layout.preferredWidth: rootItem.width - 30
//                            Layout.preferredHeight: implicitHeight
//                            color: "white"
//                            text: "Status Information"
//                            horizontalAlignment: Qt.AlignHCenter
//                        }

//                        GridLayout {
//                            rows: 7
//                            columns: 2
//                            Layout.fillWidth: true
//                            Layout.preferredHeight: implicitHeight

//                            Label {
//                                text: "ReceiveTime: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: ReceiveTime
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }

//                            Label {
//                                text: "Simulation: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: Simulation
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }

//                            Label {
//                                text: "BCCStatus: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: BCCStatus
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "RadarSearch Status: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: RadarSearchStatus
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "Operational: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: Operational
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "MissileCount: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: MissileCount
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "RadarMode: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: RadarMode
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                        }

//                        Rectangle {
//                            Layout.preferredWidth: rootItem.width - 30
//                            Layout.preferredHeight: 2
//                            color: "white"

//                        }
//                        Label {
//                            Layout.preferredWidth: rootItem.width - 30
//                            Layout.preferredHeight: implicitHeight
//                            color: "white"
//                            text: "Combat Information"
//                            horizontalAlignment: Qt.AlignHCenter
//                        }
//                        GridLayout {
//                            rows: 6
//                            columns: 2
//                            Layout.fillWidth: true
//                            Layout.preferredHeight: implicitHeight

//                            Label {
//                                text: "TN: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: TN
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "Acceptance: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: Acceptance
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "Phase: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: Phase
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "Antenna: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: Antenna
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "ChanelNo: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: ChanelNo
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }
//                            Label {
//                                text: "Inrange: "
//                                color: "white"
//                                Layout.preferredWidth: implicitWidth
//                            }
//                            Label {
//                                text: Inrange
//                                color: "white"
//                                Layout.fillWidth: true
//                                horizontalAlignment: Qt.AlignRight
//                            }

//                        }
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
