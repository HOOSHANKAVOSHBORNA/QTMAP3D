import QtQuick 2.13
import QtQuick.Layouts
import QtQuick.Controls

import "Components"

Window {
    id: applicationWindow

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
                text: 'Splash Page'
            }

            TabButton {
                text: 'Connection Page'
            }

            TabButton {
                text: 'Role Page'
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

            //        Rectangle {
            //            Layout.fillWidth: true
            //            Layout.fillHeight: true

            //            color: 'royalblue'

            //            Text {
            //                text: qsTr("Login Page")
            //                anchors.centerIn: parent
            //            }
            //        }
            LoginPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                color: 'royalblue'

                Text {
                    text: qsTr("Splash Page")
                    anchors.centerIn: parent
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                color: 'royalblue'

                Text {
                    text: qsTr("Connection Page")
                    anchors.centerIn: parent
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                color: 'royalblue'

                Text {
                    text: qsTr("Role Page")
                    anchors.centerIn: parent
                }
            }

//            StackLayout {
//                id: something
//                Layout.fillWidth: true
//                Layout.fillHeight: true

////                data: applicationWindow.mainWindow ?? []

//                //                Text {
//                //                    text: qsTr("Map Page")
//                //                    anchors.centerIn: parent
//                //                }
//            }
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
