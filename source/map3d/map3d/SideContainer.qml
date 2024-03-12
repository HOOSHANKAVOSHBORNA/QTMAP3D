import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

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
    property var settingItem

    property alias currentItemIndex: stackLayout.currentIndex

    signal noItem

    //    visible: currentIndex !== -1
    function setCurrentItemIndex(index) {
        if (currentItemIndex === index) {
            currentItemIndex = -1
            noItem()
        } else {
            currentItemIndex = index
        }
    }

    function focusWidnow(index) {
        console.log('there')
        stackLayout.data[index].isWindow = false
        stackLayout.data[index].isWindow = true
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
        RowLayout {
            visible: currentItemIndex !== -1
            Layout.preferredHeight: 20 / Style.monitorRatio
            Layout.fillWidth: true

            Text {
                text: currentItemIndex !== -1 ? sideModel.get(
                                                    currentItemIndex).name : 'you can not see this'

                Layout.topMargin: 20 / Style.monitorRatio
                Layout.bottomMargin: 10 / Style.monitorRatio
                color: Style.foregroundColor
                font.pixelSize: Style.titleFontSize
                font.family: Style.fontFamily
                font.weight: 500 / Style.monitorRatio
                Layout.fillWidth: true
            }

            Rectangle {
                Layout.preferredHeight: 22 / Style.monitorRatio
                Layout.preferredWidth: 22 / Style.monitorRatio
                color: 'transparent'

                //                            padding: 0
                Image {
                    source: "qrc:/Resources/undocker.png"
                    width: 22 / Style.monitorRatio
                    height: 22 / Style.monitorRatio
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        stackLayout.data[currentItemIndex].isWindow = true
                        sideModel.get(currentItemIndex).isWindow = true
                        currentItemIndex = -1
                        noItem()
                    }
                }
            }
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
                    currentItemIndex = 0
                }
            }

            DockWindow {
                id: toolBoxDocItem
                windowTitle: sideModel.get(1).name
                containerItem: ToolboxManagerItem {
                    id: toolbox
                    toolboxCpp: rootItem.toolboxCpp
                }

                onWindowClose: {
                    sideModel.get(1).isWindow = false
                    currentItemIndex = 1
                }
            }

            DockWindow {
                id: layerDocItem
                windowTitle: sideModel.get(2).name
                containerItem: LayerManagerItem {
                    id: layers
                    layerCpp: rootItem.layerCpp
                }

                onWindowClose: {
                    sideModel.get(2).isWindow = false
                    currentItemIndex = 2
                }
            }

            DockWindow {
                id: bookmarkDocItem
                windowTitle: sideModel.get(3).name
                containerItem: BookmarkItem {
                    id: bookmark
                    bookmarkCpp: rootItem.bookmarkCpp.getBookmarkProxyModel()
                }

                onWindowClose: {
                    sideModel.get(3).isWindow = false
                    currentItemIndex = 3
                }
            }

            DockWindow {
                id: locationDocItem
                windowTitle: sideModel.get(4).name
                containerItem: LocationManagerItem {
                    id: locationManager
                    listModel: rootItem.locationCpp
                }

                onWindowClose: {
                    sideModel.get(4).isWindow = false
                    currentItemIndex = 4
                }
            }

            DockWindow {
                id: connectionDocItem
                windowTitle: sideModel.get(5).name
                isWindow: sideModel.get(5).isWindow

                containerItem: ConnectionConfiguration {
                    id: connectionItem
                    closeBtn.visible: false
                    connectionConfigCpp: applicationWindow.connectionConfigCpp
                    //                    Layout.leftMargin: 50 / Style.monitorRatio
                    //                    Layout.rightMargin: 50 / Style.monitorRatio
                }

                onWindowClose: {
                    sideModel.get(5).isWindow = false
                    currentItemIndex = 5
                }
            }


            DockWindow {
                id: settingsDocItem
                windowTitle: sideModel.get(6).name
                isWindow: sideModel.get(6).isWindow

                containerItem: Item{
                    data: rootItem.settingItem
                }

                onWindowClose: {
                    sideModel.get(6).isWindow = false
                    currentItemIndex = 6
                }
            }
        }
    }
}
