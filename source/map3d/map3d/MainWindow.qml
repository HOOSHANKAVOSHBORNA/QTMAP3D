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
    // visible: true
    width: 800
    minimumWidth: 800
    minimumHeight: 700
    x: (Screen.width - width) / 2
    y: (Screen.height - height) / 2
    title: qsTr("MAP3D")

    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    property string modeMap: "geocentric"
    readonly property color     _colorHover : "#01AED6"
    readonly property color     _colorPresed : "#003569"

    function addToCenterCenterContainer(item) {
        centerCenterContainer.data.push(item)
    }
    color: Style.backgroundColor
    Item {
        anchors.fill: parent


        Item {
            id: unPinContainer
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 5/Style.monitorRatio
            z: 1
            width: sideBar.minWidth
            height: mainWindow.height - (25 + 10)/Style.monitorRatio

            SideBar {
                id: sideBar
                anchors.fill: parent
                //                pin: true
                onPinChanged: {
                    if(pin){
                        parent = pinContainer
                        pinContainer.visible = true
                        unPinContainer.visible = false
                    }
                    else{
                        parent = unPinContainer
                        pinContainer.visible = false
                        unPinContainer.visible = true
                    }
                }
            }
        }
        SplitView {
            id: mainWindowSplitter
            anchors.fill: parent

            Item {
                id: pinContainer
                SplitView.preferredWidth: sideBar.minWidth
                SplitView.minimumWidth: sideBar.minWidth
                SplitView.maximumWidth: sideBar.minWidth
                visible: false
            }

            StackLayout {
                id: centerCenterContainer
                SplitView.fillWidth: true
            }
        }
    }
}
