import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls

import "Components"

Window {
    id: applicationWindow

    property var userManager: undefined
    property var connectionConfigCpp: undefined
    property var loadingPageCpp: undefined
    property var mainPageCpp: undefined

    width: 800
    height: 800
    visible: true

    color: 'royal blue'

    Image {
        source: "qrc:/Resources/login-earth.jpg"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
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
            }

            TabButton {
                text: 'Loading Page'
            }

            TabButton {
                text: 'Map Page'

                //                onClicked: {
                //                    mapPage.mapItem = applicationWindow.mainPageCpp.getMapItem()
                //                }
            }
        }

        StackLayout {
            id: appStack

            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: debugTabbar.currentIndex

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
