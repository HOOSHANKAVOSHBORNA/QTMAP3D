import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls

import "style"
import "Components"

Window {
    id: applicationWindow

    // property var userManager: undefined
    // property var connectionConfigCpp: undefined
    // property var loadingPageCpp: undefined
    // property var mainPageCpp: undefined

    //    property int pageIndex: 0
    required property var applicationCpp

    visible: true
    width: 800
    height: 800

    onClosing: {
    }

    Image {
        source: "qrc:/Resources/login-earth.jpg"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
    }

    //    Connections {
    //        target: userManager
    //        function onSignedIn() {
    //            pageIndex = 1
    //        }
    //        function onSignedOut() {
    //            pageIndex = 0
    //        }
    //    }
    //    Connections {
    //        target: applicationCpp
    //        function onPageIndexChanged() {
    //            if(applicationCpp.pageIndex === 1)
    //                applicationCpp.onLoadingPage()
    //        }
    //    }
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
                    applicationCpp.pageIndex = 0
                }
            }

            TabButton {
                text: 'Loading Page'
                onClicked: {
                    applicationCpp.pageIndex = 1
                    applicationCpp.userManager.signedIn()
                }
            }

            TabButton {
                text: 'Map Page'

                onClicked: {
                    applicationCpp.pageIndex = 2
                }
            }
        }

        StackLayout {
            id: appStack

            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: applicationCpp.pageIndex /*debugTabbar.currentIndex*/

            LoginPage {
                userManager: applicationCpp.userManager
                connectionConfigCpp: applicationCpp.connectionConfigCpp
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            LoadingPage {
                // loadingPageCpp: applicationCpp.loadingPageCpp ? applicationCpp.loadingPageCpp : undefined
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            MainWindow {
                id: mapPage
                // mainPageCpp: applicationCpp.mainPageCpp ? applicationCpp.mainPageCpp : undefined
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
