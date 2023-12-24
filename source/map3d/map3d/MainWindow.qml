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
    title: qsTr("MAP3D")

    property Splash splash: Splash {
        onTimeout: mainWindow.show()
    }
    component Splash: Window {
        id: splash
        flags: Qt.SplashScreen
        // the transparent color lets background behind the image edges show through
        color: "transparent"
        modality: Qt.ApplicationModal // in case another application window is showing
        title: "Splash Window" // for the taskbar/dock, task switcher etc.
        visible: true

        // here we use the Screen attached property to center the splash window
        // x: (Screen.width - splashImage.width) / 2
        // y: (Screen.height - splashImage.height) / 2
        width: splashImage.width
        height: splashImage.height

        property int timeoutInterval: 2000
        signal timeout

        Image {
            id: splashImage
            source: "qrc:/Resources/login.png"
        }

        TapHandler {
            onTapped: splash.timeout()
        }

        Timer {
            interval: splash.timeoutInterval; running: true; repeat: false
            onTriggered: {
                splash.visible = false
                splash.timeout()
            }
        }
    }

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
