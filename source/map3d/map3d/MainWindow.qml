import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls.Material 2.12
import QtQuick.Effects

import "style"

Item {
    id: mainItem

    //--properties--------------------------------------
    property var mainPageCpp: applicationCpp.mainPageCpp ? applicationCpp.mainPageCpp : undefined
    property bool listWindowVisible: false
    property bool connectionConfigVisible: false

    property real widgetsPositionFactor: 1.0
    property bool widgetsVisible: true
    property string modeMap: "geocentric"
    readonly property color _colorHover: "#01AED6"
    readonly property color _colorPresed: "#003569"

    Rectangle {
        anchors.fill: parent

        Item {
            id: unPinContainer
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 5 / Style.monitorRatio
            z: 1
            width: sideBar.minWidth
            height: mainItem.height - (25 + 10) / Style.monitorRatio

            SideBar {
                id: sideBar

                anchors.fill: parent

                sideContainer.locationCpp: mainItem.mainPageCpp ? mainItem.mainPageCpp.getLocationManager(
                                                                      ) : undefined
                sideContainer.toolboxCpp: mainItem.mainPageCpp ? mainItem.mainPageCpp.getToolboxManager(
                                                                     ) : undefined
                sideContainer.layerCpp: mainItem.mainPageCpp ? mainItem.mainPageCpp.getLayerManager(
                                                                   ) : undefined
                sideContainer.bookmarkCpp: mainItem.mainPageCpp ? mainItem.mainPageCpp.getBookmarkManager(
                                                                      ) : undefined

                listWindowVisible: mainItem.listWindowVisible
                onListWindowVisibleChanged: mainItem.listWindowVisible = listWindowVisible

                onConnectionConfigCheckedChanged: mainItem.connectionConfigVisible
                                                  = connectionConfigChecked

                //                pin: true
                onPinCheckedChanged: {
                    if (pinChecked) {
                        parent = pinContainer
                        pinContainer.visible = true
                        unPinContainer.visible = false
                    } else {
                        parent = unPinContainer
                        pinContainer.visible = false
                        unPinContainer.visible = true
                    }
                }
            }
        }

        SplitView {
            id: mainItemSplitter
            anchors.fill: parent

            Item {
                id: pinContainer
                SplitView.preferredWidth: sideBar.minWidth
                SplitView.minimumWidth: sideBar.minWidth
                SplitView.maximumWidth: sideBar.minWidth
                visible: false
            }

            MapControllerItem {
                mapItem: mapControllerCpp
                SplitView.fillWidth: true
                SplitView.fillHeight: true
            }
        }
    }

    ListWindow {
        visible: mainItem.listWindowVisible
        listWindowCpp: mainItem.mainPageCpp ? mainItem.mainPageCpp.getListWindow(
                                                  ) : undefined

        onClosing: {
            listWindowVisible = false
        }
    }

    Window {
        id: connectionConfigWindow

        width: 440 / Style.monitorRatio
        height: 745 / Style.monitorRatio

        visible: mainItem.connectionConfigVisible

        ConnectionConfiguration {
            id: settingsItem
            anchors.fill: parent
            closeBtn.visible: false
            connectionConfigCpp: applicationCpp.connectionConfigCpp
        }

        onClosing: {
            sideBar.connectionConfigChecked = false
        }
    }
}
