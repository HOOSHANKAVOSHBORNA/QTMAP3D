import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Crystal
import "style"

ColumnLayout {
    id: rootItem

    property list<string> model: []
    property alias isOpen: stackLayout.visible
    property alias currentItemIndex: stackLayout.currentIndex
    property int previousItemIndex: -1

    signal modelEmpty

    function isInModel(s) {
        for (var i = 0; i < model.length; i++) {
            if (model[i] === s) {
                return true
            }
        }

        return false
    }

    function toggleStackLayoutIndex(index) {
        if (isOpen && currentItemIndex === index) {
            if (previousItemIndex !== -1) {
                stackLayout.currentIndex = previousItemIndex
            } else {
                isOpen = false
            }
        } else {
            if (!isOpen) {
                isOpen = true
            }
            previousItemIndex = stackLayout.currentIndex
            stackLayout.currentIndex = index
        }
    }

    function toggleToolbox() {
        var i = model.indexOf("Toolbox");
        if (i !== -1) {
            model.splice(i, 1);
        } else {
            model.push("Toolbox")
        }

        toggleStackLayoutIndex(0)
        toolbox.listModel = ToolboxInstance
    }

    function toggleLocationManager() {
        var i = model.indexOf("LocationManager");
        if (i !== -1) {
            model.splice(i, 1);
        } else {
            model.push("LocationManager")
        }

        toggleStackLayoutIndex(1)
        locationManager.listModel = LocatoinManagerInstance
    }

    function toggleLayers() {
        var i = model.indexOf("Layers");
        if (i !== -1) {
            model.splice(i, 1);
        } else {
            model.push("Layers")
        }

        toggleStackLayoutIndex(2)
        layers.layersModell = LayersInstance
    }

    function toggleBookmark() {
        var i = model.indexOf("Bookmark");
        if (i !== -1) {
            model.splice(i, 1);
        } else {
            model.push("Bookmark")
        }

        toggleStackLayoutIndex(3)
        bookmark.model = BookmarkInstance
    }

    TabBar {
        id: tabBar

        contentWidth: rootItem.model.count ? parent.width - 40 / Style.monitorRatio : 0
        Layout.leftMargin: rootItem.model.count ? 18 / Style.monitorRatio : 0
        Material.accent: Style.foregroundColor

        background: Rectangle {
            color: Style.disableColor
            height: 2
            anchors.bottom: parent.bottom
        }

        Repeater {
            id: repeater

            model: rootItem.model.length

            TabButton {
                id: tb

                width: {
                    if (rootItem.model.count === 1) {
                        return implicitWidth
                    } else {
                        if (tabBar.currentIndex === index) {
                            return implicitWidth
                        }
//                        else return (tabBar.width - implicitWidth * 2) / rootItem.model.count
                    }
                }

                contentItem: Text {
                    id: txt

                    text: rootItem.model[index] ?? "unknown"
                    font: Style.fontFamily
                    opacity: enabled ? 1.0 : 0.3
                    color: tabBar.currentIndex === index ? Style.foregroundColor : Style.disableColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    color: "transparent"
                }

                onDoubleClicked: {
                    // coder here
                }
            }
        }
    }

    StackLayout {
        id: stackLayout
        Layout.fillHeight: true
        currentIndex: 0
        visible: false

        ToolboxView { id: toolbox }
        LocationManager { id: locationManager }
        LayersWidget { id: layers }
        BookmarkItem { id: bookmark }
    }
}
