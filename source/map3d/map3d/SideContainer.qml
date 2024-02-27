import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Crystal

import "style"

Item {
    id: rootItem

    property ListModel sideModel: ListModel {}

    property var toolboxCpp
    property var locationCpp
    property var layerCpp
    property var connectionConfigurationCpp
    property var profileCpp
    property var bookmarkCpp

    property alias currentItemIndex: stackLayout.currentIndex

    visible: currentIndex !== -1

    function setCurrentItemIndex(index) {
        print("something", currentItemIndex, index)
        if (currentItemIndex === index) {
            currentItemIndex = -1
            return false
        } else {
            currentItemIndex = index
            print(stackLayout.currentIndex)
            return true
        }
    }

    //clip: true
    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        anchors.rightMargin: 15 / Style.monitorRatio
        anchors.leftMargin: 15 / Style.monitorRatio

        //        Rectangle {
        //            Layout.fillWidth: true
        //            Layout.fillHeight: true

        //            color: 'silver'
        //        }
        Text {
            text: 'Qarch'
            //            Layout.topMargin: 40 / Style.monitorRatio
            //            Layout.bottomMargin: 30 / Style.monitorRatio
            //            Layout.topMargin: 10 / Style.monitorRatio
            //            Layout.bottomMargin: 5 / Style.monitorRatio
            color: Style.foregroundColor
            font.pixelSize: 30 / Style.monitorRatio
            font.family: Style.fontFamily
            font.weight: 500
        }

        StackLayout {
            id: stackLayout

            Layout.fillHeight: true
            Layout.fillWidth: true
            currentIndex: -1

            DockWindow {
                id: profileItem

                Layout.fillHeight: true
                Layout.fillWidth: true
                windowTitle: sideModel.get(0).name
                isWindow: sideModel.get(0).isWindow

                containerItem: ProfileItem {
                    id: profile
                }

                onWindowClose: {
                    sideModel.get(0).isWindow = false
                }
            }

            DockWindow {
                id: toolBoxDocItem
                windowTitle: sideModel.get(1).name
                isWindow: sideModel.get(1).isWindow
                containerItem: ToolboxManagerItem {
                    id: toolbox
                    toolboxCpp: rootItem.toolboxCpp
                }

                onWindowClose: {
                    sideModel.get(0).isWindow = false
                }
            }

            DockWindow {
                id: layerDocItem
                windowTitle: sideModel.get(2).name
                //                isWindow: sideModel.get(2).isWindow
                containerItem: LayerManagerItem {
                    id: layers
                    layerCpp: rootItem.layerCpp
                }

                onWindowClose: {
                    sideModel.get(1).isWindow = false
                }
            }

            DockWindow {
                id: bookmarkDocItem
                windowTitle: sideModel.get(3).name
                //                isWindow: sideModel.get(3).isWindow
                containerItem: BookmarkItem {
                    id: bookmark
                    bookmarkCpp: rootItem.bookmarkCpp.getBookmarkProxyModel()
                }

                onWindowClose: {
                    sideModel.get(3).isWindow = false
                }
            }

            DockWindow {
                id: locationDocItem
                windowTitle: sideModel.get(4).name
                //                isWindow: sideModel.get(4).isWindow
                containerItem: LocationManagerItem {
                    id: locationManager
                    listModel: rootItem.locationCpp
                }

                onWindowClose: {
                    sideModel.get(4).isWindow = false
                }
            }

            DockWindow {
                id: settingsDocItem
                windowTitle: sideModel.get(5).name

                containerItem: ConnectionConfiguration {
                    id: settingsItem
                    closeBtn.visible: false
                    connectionConfigCpp: applicationWindow.connectionConfigCpp
                    //                    Layout.leftMargin: 50 / Style.monitorRatio
                    //                    Layout.rightMargin: 50 / Style.monitorRatio
                }

                onWindowClose: {
                    sideModel.get(5).isWindow = false
                }
            }
        }
    }
}
