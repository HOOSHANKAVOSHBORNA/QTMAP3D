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
    property var sectionColor: "#00587A"
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
            Layout.minimumHeight: 55
            Label {
                Text {
                    Layout.alignment: Qt.AlignCenter
                    id: headerName
                    text: "-------"
                    color:"white"
                    font.pointSize: 16
                    anchors.centerIn: parent
                }
                id:headerNameLbl
                width: parent.width/2
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {
                id:spaceLine
                width: 1
                opacity: 0.5
                height: 35
                color: "white"
                anchors.left: headerNameLbl.right
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                Text {
                    id: system
                    text: "Airplane"
                    color:"white"
                    font.pointSize: 16
                    anchors.centerIn: parent
                }

                width: parent.width/2
                anchors.left: spaceLine.right
                anchors.verticalCenter: parent.verticalCenter
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
                            target: system
                            property: "text"
                            value: Numberr
                        }

                        Binding {
                            target: headerName
                            property: "text"
                            value: MainInfo[0]
                        }

//                        Binding {
//                            target: control
//                            property: "checked"
//                            value: Active
//                        }

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
                            id: mainButton
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: implicitHeight + 25
                            property var checked: true

                            MouseArea {
                                hoverEnabled: true
                                anchors.fill: parent
                                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                onClicked: if (mainButton.checked) {
                                               mainLayout.Layout.preferredHeight = 0
                                               imgm.rotation = -90
                                               mainButton.checked = false
                                           }
                                           else {
                                               mainLayout.Layout.preferredHeight = mainLayout.implicitHeight
                                               imgm.rotation = 180
                                               mainButton.checked = true
                                           }
                            }

                            color: sectionColor

                            Text {
                                text: "Main Information"
                                color: "white"
                                anchors.centerIn: parent
                                anchors.left: parent.left
                            }
                            Image {
                                id: imgm
                                source: "qrc:/Resources/chevron.png"
                                width: 16
                                height: 16
                                rotation: 180
                                ColorOverlay {
                                    source: imgm
                                    anchors.fill: imgm
                                    color: "white"
                                }
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                            }

                        }
                        RowLayout {
                            id: mainLayout
                            spacing: 2
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: implicitHeight
                            clip: true
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
                                id: wid
                                Layout.preferredWidth: implicitWidth
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


                        Rectangle {
                            id: locationButton
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: implicitHeight + 25
                            property var checked: true

                            MouseArea {
                                hoverEnabled: true
                                anchors.fill: parent
                                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                onClicked: if (locationButton.checked) {
                                               locLayout.Layout.preferredHeight = 0
                                               imgl.rotation = -90
                                               locationButton.checked = false
                                           }
                                           else {
                                               locLayout.Layout.preferredHeight = locLayout.implicitHeight
                                               imgl.rotation = 180
                                               locationButton.checked = true
                                           }
                            }

                            color: sectionColor

                            Text {
                                text: "Location Information"
                                color: "white"
                                anchors.centerIn: parent
                                anchors.left: parent.left
                            }
                            Image {
                                id: imgl
                                source: "qrc:/Resources/chevron.png"
                                width: 16
                                height: 16
                                rotation: 180
                                ColorOverlay {
                                    source: imgl
                                    anchors.fill: imgl
                                    color: "white"
                                }
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                            }

                        }

                        RowLayout {
                            id: locLayout
                            spacing: 2
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: implicitHeight
                            clip: true
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
                                    height: 24

                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                Layout.preferredHeight: implicitHeight
                                Layout.preferredWidth: wid.implicitWidth - 10
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
                            ColumnLayout {
                                Layout.preferredHeight: implicitHeight
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

                        Rectangle {
                            id: assignButton
                            Layout.preferredWidth: rootItem.width
                            Layout.preferredHeight: implicitHeight + 25
                            property var checked: true

                            MouseArea {
                                hoverEnabled: true
                                anchors.fill: parent
                                cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                                onClicked: if (assignButton.checked) {
//                                               assignLayout.Layout.preferredHeight = 0
//                                               imga.rotation = -90
//                                               assignButton.checked = false
                                           }
                                           else {
//                                               assignLayout.Layout.preferredHeight = assignLayout.implicitHeight
//                                               imga.rotation = 180
//                                               assignButton.checked = true
                                           }
                            }

                            color: sectionColor

                            Text {
                                text: "Assignment Aircrafts"
                                color: "white"
                                anchors.centerIn: parent
                                anchors.left: parent.left
                            }

                            Image {
                                id: imga
                                source: "qrc:/Resources/chevron.png"
                                width: 16
                                height: 16
                                rotation: 180
                                ColorOverlay {
                                    source: imga
                                    anchors.fill: imga
                                    color: "white"
                                }
                                anchors.right: parent.right
                                anchors.rightMargin: 5
                                anchors.verticalCenter: parent.verticalCenter
                            }

                        }

                        RowLayout {
                            id: assignLayout
                            Layout.preferredHeight: implicitHeight
                            spacing: 2
                            clip: true
                            Layout.preferredWidth: rootItem.width
                            Rectangle {
                                Layout.preferredWidth: img3.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    verticalAlignment: Image.AlignTop
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/resources/aircraftinfo.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24
                                    rotation: 45
                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                Layout.preferredHeight: implicitHeight
                                Layout.fillWidth: true
                                Repeater {
                                    model: AssignAircraftsName
                                    Layout.fillWidth: true
                                    Label {
                                        text: AssignAircraftsName[index]
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
                                Layout.fillWidth: true
                                Repeater {
                                    model: AssignAircraftsType
                                    Layout.fillWidth: true
                                    Label {
                                        text: AssignAircraftsType[index]
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
                                Layout.fillWidth: true
                                Repeater {
                                    model: AssignAircraftsType
                                    Layout.fillWidth: true
                                    Label {
                                        text: CombatInfo[2]
                                        color: rootItem.model ? SystemColor :
                                                                "transparent";
                                        Layout.preferredWidth: implicitWidth
                                        verticalAlignment: Label.AlignVCenter
                                        Layout.leftMargin: 5
                                        Layout.rightMargin: 5
                                        leftInset: -5
                                        font.bold: true
                                        Layout.preferredHeight: implicitHeight + spacee
//                                        background: Rectangle {
//                                            opacity: 0.4
//                                            color: rootItem.model ? SystemColor[index] :
//                                                                    "transparent";
//                                            anchors.centerIn: parent
//                                            width: 40
//                                            height: 23
//                                            radius: 7
//                                        }
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
                                               statusLayout.Layout.preferredHeight = 0
                                               imgd.rotation = -90
                                               firstButton.checked = false
                                           }
                                           else {
                                               statusLayout.Layout.preferredHeight = statusLayout.implicitHeight
                                               imgd.rotation = 180
                                               firstButton.checked = true
                                           }
                            }

                            color: sectionColor

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
                                rotation: 180
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
                            Layout.preferredHeight: implicitHeight
                            Rectangle {
                                Layout.preferredWidth: img3.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    id: img3
                                    verticalAlignment: Image.AlignTop
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/resources/statusinfo.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24

                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                Layout.preferredHeight: implicitHeight
                                Layout.preferredWidth: wid.implicitWidth + 50
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
                            ColumnLayout {
//                                    Layout.fillWidth: true
                                Layout.preferredHeight: implicitHeight
                                Repeater {
                                    model: StatusInfo
                                    Label {
                                        text: StatusInfo[index]
                                        color: index == 2 ? BCCStatusColor : index == 3 ? RadarStatusColor : "white"
                                        font.bold: index == 2 || index == 3
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
                                               combatLayout.Layout.preferredHeight = 0
                                               imgs.rotation = -90
                                               secondButton.checked = false
                                           }
                                           else {
                                               combatLayout.Layout.preferredHeight = combatLayout.implicitHeight
                                               imgs.rotation = 180
                                               secondButton.checked = true
                                           }
                            }

                            color: sectionColor

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
                                rotation: 180
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
                            Layout.preferredHeight: implicitHeight
                            Layout.preferredWidth: rootItem.width
                            Rectangle {
                                Layout.preferredWidth: img4.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    id: img4
                                    verticalAlignment: Image.AlignTop
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/resources/combatinfo.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24

                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                Layout.preferredHeight: implicitHeight
                                Layout.preferredWidth: wid.implicitWidth + 10
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
                            ColumnLayout {
                                Layout.preferredHeight: implicitHeight
                                Repeater {
                                    model: CombatInfo
                                    Label {
                                        text: CombatInfo[index]
                                        color: index == 2 ? SystemColor : "white"
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
