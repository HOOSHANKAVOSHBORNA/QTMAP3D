import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls

import "Components"

Window {
    id: applicationWindow

    property var loginPageCpp: undefined
    property var loadingPageCpp: undefined
    property var mainPageCpp: undefined

    width: 800
    height: 800
    visible: true

    color: 'red'

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

                onClicked: {
                    mapPage.mapItem = applicationWindow.mainPageCpp.getMapItem();
                }
            }
        }

        StackLayout {
            id: appStack

            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: debugTabbar.currentIndex

            LoginPage {
                loginPageCpp: applicationWindow.loginPageCpp
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            LoadingInfo {
                loadingPageCpp: applicationWindow.loadingPageCpp
                Layout.fillWidth: true
                Layout.fillHeight: true

            }

            MainWindow {
                id: mapPage
                mainPageCpp: applicationWindow.mainPageCpp
//                mapItem: applicationWindow.mapItem
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

        }
    }
}
