import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls.Material 2.12
import QtQuick.Effects
import Crystal 1.0
import "style"

CMainWindow {
    id: mainWindow
    visible: true
    width: 800
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("MAP3D")

    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    property string modeMap: "geocentric"
    readonly property color     _colorHover : "#01AED6"
    readonly property color     _colorPresed : "#003569"

    function addToCenterCenterContainer(item) {
        centerCenterContainer.data.push(item)
    }

    SideBar {
        id: sideBar

        // TODO: 22 is hardcode size of statusbar
        height: mainWindow.height - (22 / Style.monitorRatio)

        mainWindow: mainWindow
        pinContainer: pinContainer
        unpinContainer: unpinContainer
    }

    Item {
        id: unpinContainer

        height: childrenRect.height
        width: childrenRect.width
        z: 1
    }

    SplitView {
        id: mainWindowSplitter
        anchors.fill: parent

        Rectangle {
            id: pinContainer

            visible: sideBar.state === "pin"
            color: "tomato"
            SplitView.preferredHeight: mainWindow.height
            // TODO: don't work with visible and set preferred width as childrenRect size
            SplitView.preferredWidth: sideBar.width
        }

        StackLayout {
            id: centerCenterContainer
            SplitView.fillWidth: true
            SplitView.fillHeight: true
        }
    }

    ContextmenuWidget {
        id: contextmenu
    }

    function showInfoView(item, title) {
        var indx = -1
        for (var i = 0; i < leftContainer.model.count; ++i){
            if (leftContainer.model.get(i).item === item){
                leftContainer.setCurrentIndex(i)
                return
            }
        }
        leftContainer.model.append({item:item, name:title})
    }

    //---------------------for frame---------------------
    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: Style.marginSize
        anchors.topMargin: Style.marginSize
        height: rowl.impliitemClickedcitHeight + 15
        width: rowl.implicitWidth + 20
        visible: false
        color: "transparent"
        Rectangle {
            anchors.fill: parent
            color: "#404040"
            opacity: 0.8
            radius: 10
        }
        MouseArea {
            anchors.fill: parent
        }

        RowLayout {
            id: rowl
            anchors.fill: parent
            spacing: 0
            anchors.rightMargin: 20
            Item {
                Layout.alignment: Qt.AlignCenter

                Layout.preferredWidth: 64
                Layout.preferredHeight: 64

                ColumnLayout {
                    id: col3
                    anchors.centerIn: parent
                    Image {
                        id: img3
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 24
                        Layout.preferredHeight: 24
                        source: "qrc:///Resources/close.png"
                        width: 24
                        height: 24
                    }

                    Label {
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: implicitWidth
                        Layout.preferredHeight: implicitHeight
                        text: "Exit"
                        color: "white"
                        font.family: Style.fontFamily
                        font.pointSize: 11
                    }
                    visible: false
                }

                MouseArea {
                    id: mouseArea3
                    anchors.fill: col3
                    hoverEnabled: true

                    property bool isMouseOnItem: false

                    onEntered: function() {
                        isMouseOnItem = true;
                    }
                    onExited: function() {
                        isMouseOnItem = false;
                    }

                    onClicked: function() {
                        mainWindow.close();
                        //rootItem.clickCallback(index);
                    }
                }

                MultiEffect {
                    anchors.fill: col3
                    source: col3
                    colorization: 1
                    colorizationColor: mouseArea3.isMouseOnItem ?
                                           (mouseArea3.pressed ? _colorPresed: _colorHover) :
                                           (mouseArea3.pressed ? _colorHover : "#FFFFFF");
                }
            }
            Item {
                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: 64
                Layout.preferredHeight: 64

                ColumnLayout {
                    id: col4
                    anchors.centerIn: parent
                    Image {
                        id: img4
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 24
                        Layout.preferredHeight: 24
                        source: "qrc:///Resources/collapse.png"
                        width: 24
                        height: 24
                    }

                    Label {
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: implicitWidth
                        Layout.preferredHeight: implicitHeight
                        text: "Mininnize"
                        color: "white"
                        font.family: Style.fontFamily
                        font.pointSize: 11
                    }
                    visible: false
                }

                MouseArea {
                    id: mouseArea4
                    anchors.fill: col4
                    hoverEnabled: true

                    property bool isMouseOnItem: false

                    onEntered: function() {
                        isMouseOnItem = true;
                    }
                    onExited: function() {
                        isMouseOnItem = false;
                    }

                    onClicked: function() {
                        mainWindow.showMinimized()
                        //rootItem.clickCallback(index);
                    }
                }

                MultiEffect {
                    anchors.fill: col4
                    source: col4
                    colorization: 1
                    colorizationColor: mouseArea4.isMouseOnItem ?
                                           (mouseArea4.pressed ? _colorPresed: _colorHover) :
                                           (mouseArea4.pressed ? _colorHover : "#FFFFFF");
                }
            }
        }
    }
}
