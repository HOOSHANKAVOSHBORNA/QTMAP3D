import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.0
import Crystal 1.0

Item {
    id: rootItem
    x: 0
    width: 300
    clip: true

    signal titleChanged (string title);
    property AircraftInfoModel model
    property var boxColor: "#363739"
    property var valueColor: "#363739"
    property var keyTextColor: "#9b9ca0"
    property var sectionColor: "#00587A"
    property var spacee: 5
    Label {
        id: forWidth
        visible: false
        text: " 1401/01/01 00:00 "
    }

    Rectangle{
        anchors.fill: parent
        color: "#2F2E2C"
//        opacity: 0.8
        radius: 10
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 0
        Item {
            Layout.fillWidth: true
            Layout.minimumHeight: 55
            /////-----------------header--------------------------
            Rectangle{
                anchors.fill: parent

                radius: 10
                color: "transparent"
                Label {
                    Text {
                        id: headerType
                        text: "--------"
                        color:"white"
                        font.pointSize: 16
                        anchors.centerIn: parent
                    }
                    id:headerTypeLbl
                    width: parent.width/2
                    anchors.verticalCenter: parent.verticalCenter

                }
                Rectangle {
                    id:spaceLine
                    width: 1
                    opacity: 0.5
                    height: 35
                    color: "white"
                    anchors.left: headerTypeLbl.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    Text {
                        id: headerName
                        text: "Airplane"
                        color:"white"
                        font.pointSize: 16
                        anchors.centerIn: parent
                    }

                    width: parent.width/2
                    anchors.left: spaceLine.right
                    anchors.verticalCenter: parent.verticalCenter
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
                        spacing: 1

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                        width: rootItem.width

                        Binding {
                            target: headerName
                            property: "text"
                            value: TN
                        }

                        Binding {
                            target: headerType
                            property: "text"
                            value: MainInfo[4]
                        }
                        //--------------------------------------image----------------
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 200
                            Image {
                                id: img
                                anchors.fill: parent
                                source: MainInfo[4] === "Aircraft" ? "qrc:/resources/boeing-777x.jpg":
                                        MainInfo[4] === "Drone" ? "qrc:/resources/drone.jpg":
                                        MainInfo[4] === "Fighter" ? "qrc:/resources/fighter.jpg":
                                        MainInfo[4] === "Helicopter" ? "qrc:/resources/helicopter.jpg":
                                                                      "qrc:/resources/missile.jpg"
                                fillMode: Image.Stretch
                            }
                        }
                        //----------------------------------maininfo---------------
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
                                    source: "qrc:/resources/aircraftinfo.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24

                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                id: wid
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
                                        color: index == 1 ? rootItem.model.getAircraftColor() : "white"
                                        clip: true
                                        font.bold: index == 1
                                        Layout.fillWidth: true
                                        Layout.leftMargin: 5
                                        leftInset: -5
                                        verticalAlignment: Label.AlignVCenter
                                        Layout.preferredHeight: 20 + spacee
                                        background: Rectangle {
                                            color: boxColor
                                            clip: true
                                        }
                                    }
                                }
                            }
                        }
                        //-----------------location info----------------------------
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
                                    height:24
                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                Layout.preferredHeight: implicitHeight
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
                        //---------------assign info------------------------------
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
                                text: "Assignment Systems"
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
                                    source: "qrc:/resources/system.png"
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
                                    model: AssignedSystemsName
                                    Layout.fillWidth: true
                                    Label {
                                        text: AssignedSystemsName[index]
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
                                    model: AssignedSystemsNumber
                                    Layout.fillWidth: true
                                    Label {
                                        text: AssignedSystemsNumber[index]
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
                                    model: AssignedSystemsPhase
                                    Layout.fillWidth: true
                                    Label {
                                        text: AssignedSystemsPhase[index]
                                        color: rootItem.model ? SystemColor[index] :
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
                                               detectionLayout.Layout.preferredHeight = 0
                                               imgd.rotation = -90
                                               firstButton.checked = false
                                           }
                                           else {
                                               detectionLayout.Layout.preferredHeight = detectionLayout.implicitHeight
                                               imgd.rotation = 180
                                               firstButton.checked = true
                                           }
                            }

                            color: sectionColor

                            Text {
                                text: "Detection Systems"
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
                            id: detectionLayout
                            Layout.preferredHeight: implicitHeight
                            spacing: 2
                            clip: true
                            Layout.preferredWidth: rootItem.width
                            Rectangle {
                                Layout.preferredWidth: img3.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    id: img3
                                    verticalAlignment: Image.AlignTop
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/resources/system.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height:24
                                    rotation: 45
                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                spacing: 0
                                Repeater {
                                    model: DetectionSystems
                                    Label {
                                        Layout.preferredWidth: rootItem.width - 30
                                        Layout.preferredHeight: implicitHeight
                                        Layout.leftMargin: 5
                                        leftInset: -5
                                        Layout.bottomMargin: 3
                                        bottomInset: -3
                                        color: "white"
                                        text: DetectionSystems[index]
                                        horizontalAlignment: Qt.AlignLeft
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
                                               sendsLayout.Layout.preferredHeight = 0
                                               imgs.rotation = -90
                                               secondButton.checked = false
                                           }
                                           else {
                                               sendsLayout.Layout.preferredHeight = sendsLayout.implicitHeight
                                               imgs.rotation = 180
                                               secondButton.checked = true
                                           }
                            }

                            color: sectionColor

                            Text {
                                text: "Sends"
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
                            id: sendsLayout
                            Layout.preferredHeight: implicitHeight
                            spacing: 2
                            clip: true
                            Layout.preferredWidth: rootItem.width

                            Rectangle {
                                Layout.preferredWidth: img4.width + 6
                                Layout.fillHeight: true
                                Image {
                                    anchors.top: parent.top
                                    id: img4
                                    verticalAlignment: Image.AlignTop
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "qrc:/resources/stations.png"
                                    sourceSize: Qt.size(24,24)
                                    width: 24
                                    height: 24
                                }
                                color: "#474747"
                            }
                            ColumnLayout {
                                spacing: 0
                                Repeater {
                                    model: Sends
                                    Label {
                                        Layout.preferredWidth: rootItem.width - 30
                                        Layout.preferredHeight: implicitHeight
                                        Layout.leftMargin: 5
                                        leftInset: -5
                                        Layout.bottomMargin: 3
                                        bottomInset: -3
                                        color: "white"
                                        text: Sends[index]
                                        horizontalAlignment: Qt.AlignLeft
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

        AircraftInfoButtonGroup {
            id: buttonGroup
            Layout.fillWidth: true
            Layout.minimumHeight: 60

            onGotoButtonClicked: function() {
                rootItem.model.gotoButtonClicked();
            }
            onRouteButtonClicked: function(check) {
                rootItem.model.routeButtonClicked(check);
            }
            onTrackButtonClicked: function(check) {
                rootItem.model.trackButtonClicked(check);
            }
            onMoreButtonClicked: function() {
                rootItem.model.moreButtonClicked();
            }
            onLatestPointsToggled: function (check) {
                rootItem.model.latestPointsClicked(check);
            }
        }
    }

    function setTrackOff() {
        buttonGroup.setTrackOff(false);
    }
}
