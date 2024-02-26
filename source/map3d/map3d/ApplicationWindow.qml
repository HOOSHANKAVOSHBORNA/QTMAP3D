import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls
import "style"
import "Components"

Window {
    id: applicationWindow

    property var userManager: undefined
    property var connectionConfigCpp: undefined
    property var loadingPageCpp: undefined
    property var mainPageCpp: undefined
    property int pageIndex: 0

    visible: true
    width: 800
    height: 800

    Image {
        source: "qrc:/Resources/login-earth.jpg"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }



    Connections {
        target: userManager
        onSignedOut: {
            appStack.currentIndex = 0
        }
    }

    ColumnLayout {
        anchors.fill: parent

        // DEBUG
        TabBar {
            id: debugTabbar

            Layout.fillWidth: true
            Layout.preferredHeight: 20

            TabButton {
                text: 'Login Page'
                onClicked: {
                    pageIndex = 0
                }
            }

            TabButton {
                text: 'Loading Page'
                onClicked: {
                    pageIndex = 1
                }
            }

            TabButton {
                text: 'Map Page'

                onClicked: {
                    pageIndex = 2
                }
            }
        }

        StackLayout {
            id: appStack

            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: pageIndex /*debugTabbar.currentIndex*/

            LoginPage {
                userManager: applicationWindow.userManager
                connectionConfigCpp: applicationWindow.connectionConfigCpp
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            LoadingPage {
                loadingPageCpp: applicationWindow.loadingPageCpp
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            MainWindow {
                id: mapPage
                mainPageCpp: applicationWindow.mainPageCpp
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
