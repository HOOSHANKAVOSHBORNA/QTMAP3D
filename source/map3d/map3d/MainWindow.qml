import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls.Material 2.12
import QtQuick.Effects
import "Components"
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

                sideContainer.settingItem: mainItem.mainPageCpp ? mainItem.mainPageCpp.settingsItem :  undefined

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

    IconButton {
        id: btnList

        padding: 0
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 20 / Style.monitorRatio
        anchors.topMargin: 40 / Style.monitorRatio
        iconImageSource: "qrc:/Resources/list.png"
        width: Math.ceil(40 / Style.monitorRatio)
        height: Math.ceil(40 / Style.monitorRatio)

        onCheckedChanged: {
            mainItem.listWindowVisible = checked
        }

        display: AbstractButton.IconOnly
        checkable: true
        checked: mainItem.listWindowVisible

        ToolTip {
            y: 35
            x: -btnList.width / 2

            visible: false //btnList.hovered

            contentItem: Text {
                id: tooltipList
                anchors.margins: 7 / Style.monitorRatio
                text: 'List'
                font.pixelSize: Style.regularFontSize
                color: 'white'
            }

            background: Rectangle {
                visible: btnList.hovered
                // height: btnList.height
                // width: tooltipList.contentWidth
                radius: 10 / Style.monitorRatio

                color: "#01AED6"
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
        maximumWidth: 440 / Style.monitorRatio
        minimumWidth: 440 / Style.monitorRatio
        visible: mainItem.connectionConfigVisible

        ConnectionConfiguration {
            id: settingsItem

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 60 / Style.monitorRatio
            closeBtn.visible: false
            connectionConfigCpp: applicationCpp.connectionConfigCpp
        }

        onClosing: {
            sideBar.connectionConfigChecked = false
        }
    }
}
